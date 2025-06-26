#include "ix_ThreadPool.hpp"
#include "ix_Function.hpp"
#include "ix_Thread.hpp"
#include "ix_atomic.hpp"
#include "ix_bit.hpp"
#include "ix_environment.hpp"
#include "ix_wrap.hpp"

thread_local static ix_Worker *tls_worker;
static constexpr uint64_t MAX_NUM_NATIVE_THREADS = 1024;

ix_JobStealingDeque::ix_JobStealingDeque(size_t initial_capacity)
    : m_initial_capacity(initial_capacity),
      m_top(MAX_NUM_NATIVE_THREADS), // Prevent overflow in `m_bottom - 1`.
      m_bottom(MAX_NUM_NATIVE_THREADS),
      m_job_array(nullptr)
{
    ix_ASSERT(initial_capacity <= ix_SIZE_MAX);
    ix_ASSERT(ix_is_power_of_2(initial_capacity));
}

ix_JobStealingDeque::ix_JobStealingDeque(ix_JobStealingDeque &&other)
    : m_initial_capacity(other.m_initial_capacity),
      m_top(other.m_top),
      m_bottom(other.m_bottom),
      m_job_array(other.m_job_array),
      m_garbages(ix_move(other.m_garbages))
{
    other.m_top = MAX_NUM_NATIVE_THREADS;
    other.m_bottom = MAX_NUM_NATIVE_THREADS;
    other.m_job_array = nullptr;
}

ix_JobStealingDeque &ix_JobStealingDeque::operator=(ix_JobStealingDeque &&other)
{
    if (this == &other)
    {
        return *this;
    }

    free_memory();

    m_initial_capacity = other.m_initial_capacity;
    m_top = other.m_top;
    m_bottom = other.m_bottom;
    m_job_array = other.m_job_array;
    m_garbages = ix_move(other.m_garbages);

    other.m_top = MAX_NUM_NATIVE_THREADS;
    other.m_bottom = MAX_NUM_NATIVE_THREADS;
    other.m_job_array = nullptr;

    return *this;
}

void ix_JobStealingDeque::free_memory()
{
    for (void *garbarge : m_garbages)
    {
        ix_FREE(garbarge);
    }

    if (m_job_array != nullptr)
    {
        ix_FREE(m_job_array->data);
        ix_FREE(m_job_array);
    }
}

ix_JobStealingDeque::~ix_JobStealingDeque()
{
    free_memory();
}

size_t ix_JobStealingDeque::size() const
{
    const uint64_t b = m_bottom;
    const uint64_t t = m_top;
    ix_ASSERT((b < t) || (b - t < ix_SIZE_MAX));
    return (b < t) ? 0 : static_cast<size_t>(b - t);
}

void ix_JobStealingDeque::push_bottom(ix_Job *job)
{
    const uint64_t b = ix_atomic_load(&m_bottom, ix_MEMORY_ORDER_RELAXED);
    const uint64_t t = ix_atomic_load(&m_top, ix_MEMORY_ORDER_ACQUIRE);
    JobArray *job_array = ix_atomic_load(&m_job_array, ix_MEMORY_ORDER_RELAXED);

    uint64_t capacity = (job_array == nullptr) ? 0 : job_array->capacity;
    const bool do_resize = (capacity < b - t + 1);
    if (ix_UNLIKELY(do_resize))
    {
        const uint64_t old_capacity = capacity;
        const uint64_t new_capacity = (old_capacity == 0) ? m_initial_capacity : (2 * old_capacity);
        ix_Job **new_data = ix_ALLOC_ARRAY(ix_Job *, static_cast<size_t>(new_capacity));

        if (job_array != nullptr)
        {
            ix_Job **old_data = job_array->data;
            const uint64_t old_mask = old_capacity - 1;
            const uint64_t new_mask = new_capacity - 1;
            for (uint64_t i = t; i < b; i++)
            {
                new_data[i & new_mask] = old_data[i & old_mask];
            }

            m_garbages.emplace_back(old_data);
            m_garbages.emplace_back(job_array);
        }

        JobArray *new_job_array = ix_ALLOC_STRUCT(JobArray);

        new_job_array->capacity = new_capacity;
        new_job_array->data = new_data;
        ix_atomic_store(&m_job_array, new_job_array, ix_MEMORY_ORDER_RELEASE);

        capacity = new_capacity;
        job_array = new_job_array;
    }

    ix_atomic_store(&job_array->data[b & (capacity - 1)], job, ix_MEMORY_ORDER_RELAXED);
    ix_atomic_memory_barrier(ix_MEMORY_ORDER_RELEASE);
    ix_atomic_store(&m_bottom, b + 1, ix_MEMORY_ORDER_RELAXED);
}

ix_Job *ix_JobStealingDeque::pop_bottom()
{
    const uint64_t b = ix_atomic_load(&m_bottom, ix_MEMORY_ORDER_RELAXED) - 1;
    JobArray *job_array = ix_atomic_load(&m_job_array, ix_MEMORY_ORDER_RELAXED);

    ix_atomic_store(&m_bottom, b, ix_MEMORY_ORDER_RELAXED);

    ix_atomic_memory_barrier(ix_MEMORY_ORDER_SEQ_CST);

    uint64_t t = ix_atomic_load(&m_top, ix_MEMORY_ORDER_RELAXED);

    const bool no_job_available = (b < t);
    if (no_job_available)
    {
        ix_atomic_store(&m_bottom, b + 1, ix_MEMORY_ORDER_RELAXED);
        return nullptr;
    }

    ix_Job *job = ix_atomic_load(&job_array->data[b & (job_array->capacity - 1)], ix_MEMORY_ORDER_RELAXED);

    const bool only_one_job_is_available = (t == b);
    if (!only_one_job_is_available)
    {
        return job;
    }

    const bool lose = !ix_atomic_cas_strong(&m_top, &t, t + 1, ix_MEMORY_ORDER_SEQ_CST, ix_MEMORY_ORDER_RELAXED);
    if (lose)
    {
        job = nullptr;
    }

    ix_atomic_store(&m_bottom, b + 1, ix_MEMORY_ORDER_RELAXED);

    return job;
}

ix_Job *ix_JobStealingDeque::steal_top()
{
    uint64_t t = ix_atomic_load(&m_top, ix_MEMORY_ORDER_ACQUIRE);
    ix_atomic_memory_barrier(ix_MEMORY_ORDER_SEQ_CST);
    const uint64_t b = ix_atomic_load(&m_bottom, ix_MEMORY_ORDER_ACQUIRE);

    const bool no_job_is_available = (b <= t);
    if (no_job_is_available)
    {
        return nullptr;
    }

    JobArray *job_array = ix_atomic_load(&m_job_array, ix_MEMORY_ORDER_CONSUME);
    ix_Job *job = ix_atomic_load(&job_array->data[t & (job_array->capacity - 1)], ix_MEMORY_ORDER_RELAXED);
    const bool lose = !ix_atomic_cas_strong(&m_top, &t, t + 1, ix_MEMORY_ORDER_SEQ_CST, ix_MEMORY_ORDER_RELAXED);
    if (lose)
    {
        return nullptr;
    }

    ix_ASSERT(job->ttl > 0);

    return job;
}

ix_ThreadPoolOption::ix_ThreadPoolOption()
    : num_worker_threads(ix_hardware_concurrency()),
      initial_job_queue_capacity(ix_OPT_LEVEL(DEBUG) ? 1 : 1024),
      job_pool_size(ix_OPT_LEVEL(DEBUG) ? 4096 : (1024 * 32)),
      spwan_workes_lazily(false)
{
    if (num_worker_threads == 0)
    {
        // 3 is an arbitrary value since we have no way to determine the actual number of cores.
        num_worker_threads = 3;
    }
}

size_t ix_Worker::index() const
{
    return static_cast<size_t>(m_index);
}

ix_Worker::ix_Worker(ix_ThreadPool *thread_pool, size_t index, const ix_ThreadPoolOption &option)
    : m_index(index),
      m_thread_pool(thread_pool),
      m_job_pool(option.job_pool_size),
      m_rng(index),
      m_jobs(option.initial_job_queue_capacity)
{
    m_job_stack.reserve(32);
}

void ix_Worker::execute_job_maybe()
{
    ix_Job *job = get_executable_job();

    if (job == nullptr)
    {
        ix_yield_this_thread();
        return;
    }

    m_job_stack.emplace_back(job);
    job->entry();
    m_job_stack.pop_back();
    finalize_job(job);
}

void ix_Worker::finalize_job(ix_Job *job)
{
    // Finalizer for a finshed job
    ix_Job *j = job;
    while (true)
    {
        const int64_t original_ttl = ix_atomic_fetch_sub(&j->ttl, int64_t{1}, ix_MEMORY_ORDER_RELAXED);
        const bool done = (original_ttl == 1);
        const bool has_parent = (j->parent != nullptr);
        if (!done || !has_parent) // !(done && has_parent)
        {
            break;
        }
        j = j->parent;
    }
}

ix_Job *ix_Worker::get_executable_job()
{
    ix_Job *job = m_jobs.pop_bottom();
    if (job != nullptr)
    {
        ix_ASSERT(job->ttl != 0);
        return job;
    }

    const size_t random_index = m_rng.rand_range<size_t>(0, m_thread_pool->size() - 1);
    const size_t num_workers = m_thread_pool->size();

    for (size_t i = 0; i < num_workers; i++)
    {
        const size_t target_index = (random_index + i) % num_workers;
        if (target_index == m_index)
        {
            continue;
        }

        ix_JobStealingDeque &target_queue = m_thread_pool->m_workers[target_index].m_jobs;
        job = target_queue.steal_top();
        if (job == nullptr)
        {
            continue;
        }

        size_t num_jobs_to_steal = 2 * target_queue.size() / num_workers;
        num_jobs_to_steal = (num_jobs_to_steal == 0) ? 1 : num_jobs_to_steal;
        for (size_t j = 0; j < num_jobs_to_steal; j++)
        {
            ix_Job *stolen_job = target_queue.steal_top();
            if (stolen_job == nullptr)
            {
                break;
            }
            m_jobs.push_bottom(stolen_job);
        }

        ix_ASSERT(job->ttl != 0);
        return job;
    }

    return nullptr;
}

ix_Job *ix_Worker::allocate_job(const ix_FunctionN<16, void()> &job_entry, ix_Job *parent)
{
    static_assert(ix_is_trivially_copy_assignable_v<ix_FunctionN<16, void()>>);
    static_assert(ix_is_trivially_destructible_v<ix_FunctionN<16, void()>>);

    ix_Job *job = allocate_job();
    ix_memcpy(&job->entry, &job_entry, sizeof(job->entry));
    job->parent = parent;
    job->ttl = (parent == nullptr) ? 2 : 1;
    if (parent != nullptr)
    {
        ix_atomic_fetch_add(&parent->ttl, int64_t{1}, ix_MEMORY_ORDER_RELAXED);
    }
    return job;
}

ix_Job *ix_Worker::allocate_job()
{
    ix_Job *first_candidate = &m_job_pool[m_next_job_index % m_job_pool.size()];
    ix_Job *job_pool_begin = m_job_pool.begin();
    ix_Job *job_pool_end = m_job_pool.end();

    ix_Job *job;
    while (true)
    {
        job = first_candidate;
        if (job->ttl == 0)
        {
            break;
        }

        do
        {
            job = ix_wrap_increment(job, job_pool_begin, job_pool_end);
        } while ((job->ttl != 0) && (job != first_candidate));

        const bool job_pool_full = (job == first_candidate);
        if (!job_pool_full)
        {
            ix_ASSERT(job->ttl == 0);
            break;
        }

        execute_job_maybe();
    }

    ix_ASSERT(job->ttl == 0);
    m_next_job_index += 1;

    return job;
}

[[nodiscard]] ix_Job *ix_Worker::add_empty_job()
{
    ix_Job *job = allocate_job();
    job->parent = nullptr;
    job->ttl = 1;
    // Do not push `job` to `m_jobs` because it is no-op.
    // `job->ttl` gets zero when `wait` is called on the returned job.
    return job;
}

[[nodiscard]] ix_Job *ix_Worker::add_job(const ix_FunctionN<16, void()> &job_entry)
{
    return add_job(nullptr, job_entry);
}

ix_Job *ix_Worker::add_job(ix_Job *parent, const ix_FunctionN<16, void()> &job_entry)
{
    ix_Job *job = allocate_job(job_entry, parent);
    m_jobs.push_bottom(job);
    return job;
}

void ix_Worker::wait(ix_Job *job)
{
    // You can't wait for a job with parent because an `ix_Job` value may be reused when its 'ttl' gets zero.
    // IDEA: Relax this constraint.
    ix_ASSERT_FATAL(
        (job->parent == nullptr) && (job->ttl != 0),
        "ix_Worker::wait(): The specified job is not waitable"
    );

    while (job->ttl != 1)
    {
        execute_job_maybe();
    }
    // `finalize_job` gets called twice for the `job`, but this is OK because:
    // - `job` doesn't have a parent, and such jobs have 'ttl' set to 2 on creation.
    // - 'wait' is relatively cold.
    finalize_job(job);
    ix_ASSERT(job->ttl == 0);
}

ix_Job *ix_Worker::get_current_job() const
{
    ix_Job *job = m_job_stack[m_job_stack.size() - 1];
    ix_ASSERT(job->ttl > 0);
    return job;
}

ix_ThreadPool::ix_ThreadPool(const ix_ThreadPoolOption &option)
    : m_option(option)
{
    const size_t num_worker_threads = option.num_worker_threads;

    m_workers.reserve(num_worker_threads);
    m_extra_worker_threads.reserve(num_worker_threads - 1);

    if (m_option.spwan_workes_lazily)
    {
        return;
    }

    for (size_t i = 0; i < num_worker_threads; i++)
    {
        spawn_worker();
    }
}

void ix_ThreadPool::spawn_worker()
{
    const size_t i = m_workers.size();

    m_workers.emplace_back(this, i, m_option);
    ix_Worker *new_worker = &m_workers[i];
    new_worker->m_thread_pool = this;
    new_worker->m_index = i;

    if (i == 0)
    {
        tls_worker = new_worker;
        return;
    }

    ix_Thread &t = m_extra_worker_threads.emplace_back();
    t.start([worker = new_worker]() {
        // ix_log_debug("Worker %zu started.", worker->index());
        tls_worker = worker;
        while (!worker->m_should_die)
        {
            worker->execute_job_maybe();
        }
        // ix_log_debug("Worker %zu died", worker->index());
    });
}

ix_ThreadPool::~ix_ThreadPool()
{
    shutdown();
}

size_t ix_ThreadPool::size() const
{
    return m_workers.size();
}

ix_Worker *ix_ThreadPool::get_current_worker()
{
    return tls_worker;
}

ix_Job *ix_ThreadPool::get_current_job()
{
    return tls_worker->get_current_job();
}

ix_Job *ix_ThreadPool::add_empty_job()
{
    if (m_workers.empty())
    {
        spawn_worker();
    }
    return m_workers[0].add_empty_job();
}

ix_Job *ix_ThreadPool::add_job(const ix_FunctionN<16, void()> &job_entry)
{
    return add_job(nullptr, job_entry);
}

ix_Job *ix_ThreadPool::add_job(ix_Job *parent, const ix_FunctionN<16, void()> &job_entry)
{
    if (m_workers.size() < m_option.num_worker_threads)
    {
        spawn_worker();
    }
    return m_workers[0].add_job(parent, job_entry);
}

void ix_ThreadPool::wait(ix_Job *job)
{
    m_workers[0].wait(job);
}

void ix_ThreadPool::shutdown()
{
    const size_t num_workers = m_workers.size();
    const size_t num_extra_worker_threads = m_extra_worker_threads.size();
    ix_ASSERT(
        ((num_workers == 0) && (num_extra_worker_threads == 0)) ||
        ((num_workers != 0) && (num_extra_worker_threads + 1 == num_workers))
    );

    for (size_t i = 1; i < num_workers; i++)
    {
        m_workers[i].m_should_die = true;
    }

    while (true)
    {
    JOINABILITY_CHECK:
        for (size_t i = 0; i < num_extra_worker_threads; i++)
        {
            if (!m_extra_worker_threads[i].is_joinable())
            {
                m_workers[0].execute_job_maybe();
                goto JOINABILITY_CHECK;
            }
        }
        break;
    }

    if (num_workers != 0)
    {
        for (size_t i = 0; i < num_extra_worker_threads; i++)
        {
            m_extra_worker_threads[i].join();
        }
    }

    m_workers.clear();
    m_extra_worker_threads.clear();
}

#if ix_DO_TEST
#include "ix_test.hpp"

ix_TEST_CASE("ix_ThreadPool")
{
    static constexpr size_t N = 1024;
    ix_ThreadPool pool;

    ix_Vector<uint64_t> v(N);
    ix_Job *root = pool.add_empty_job();
    size_t count = 0;

    for (size_t i = 0; i < N; i++)
    {
        pool.add_job(root, [&]() {
            const size_t c = ix_atomic_fetch_add(&count, size_t{1}, ix_MEMORY_ORDER_RELAXED);
            v[c] += c * c;
        });
    }

    pool.wait(root);

    for (size_t i = 0; i < N; i++)
    {
        ix_EXPECT(v[i] == i * i);
    }
}

ix_TEST_CASE("ix_ThreadPool: job addition from a job")
{
    static constexpr size_t N = 1024;

    struct
    {
        ix_Vector<uint64_t> v;
        size_t count;
    } s;
    s.v.resize(N);
    s.count = 0;

    ix_FunctionN<16, void()> entry = [&]() {
        ix_Worker *worker = ix_ThreadPool::get_current_worker();
        ix_Job *job = worker->get_current_job();
        const size_t c = ix_atomic_fetch_add(&s.count, size_t{1}, ix_MEMORY_ORDER_RELAXED);

        if (1024 <= c)
        {
            return;
        }

        worker->add_job(job, entry);
        worker->add_job(job, entry);
        s.v[c] = c * c;
    };

    ix_ThreadPool pool;

    ix_Job *root = pool.add_job(entry);
    pool.wait(root);

    for (size_t i = 0; i < N; i++)
    {
        ix_EXPECT(s.v[i] == i * i);
    }
}

ix_TEST_CASE("ix_ThreadPool: wait from a job")
{
    static constexpr size_t NUM_WORKERS = 8;
    static constexpr size_t N = 1024;

    struct
    {
        ix_Vector<uint64_t> v;
        size_t count;
    } s;
    s.v.resize(N);
    s.count = 0;

    ix_FunctionN<16, void()> entry = [&]() {
        ix_Worker *worker = ix_ThreadPool::get_current_worker();
        ix_Job *job = worker->get_current_job();
        const size_t c = ix_atomic_fetch_add(&s.count, size_t{1}, ix_MEMORY_ORDER_RELAXED);

        if (N <= c)
        {
            return;
        }

        worker->add_job(job, entry);
        worker->add_job(job, entry);

        s.v[c] = c * c;
    };

    ix_ThreadPoolOption option;
    option.num_worker_threads = NUM_WORKERS;
    ix_ThreadPool pool(option);

    ix_Job *root = pool.add_empty_job();
    for (size_t i = 0; i < NUM_WORKERS; i++)
    {
        pool.add_job(root, [&]() {
            ix_Worker *worker = ix_ThreadPool::get_current_worker();

            ix_Job *children[3];
            for (size_t j = 0; j < ix_LENGTH_OF(children); j++)
            {
                children[j] = worker->add_job(entry);
                worker->wait(children[j]);
            }
        });
    }
    pool.wait(root);

    for (size_t i = 0; i < N; i++)
    {
        ix_EXPECT(s.v[i] == i * i);
    }
}

ix_TEST_CASE("ix_ThreadPool: worker index")
{
    ix_ThreadPoolOption option;
    option.num_worker_threads = 1;
    ix_ThreadPool pool(option);
    ix_Job *root = pool.add_job([]() { ix_EXPECT(ix_ThreadPool::get_current_worker()->index() == 0); });
    pool.wait(root);
}

ix_TEST_CASE("ix_ThreadPool: job pool gets full")
{
    const size_t job_pool_size = 128;
    ix_ThreadPoolOption option;
    option.num_worker_threads = 1;
    option.job_pool_size = job_pool_size;
    ix_ThreadPool pool(option);

    ix_Job *root = pool.add_empty_job();

    for (size_t i = 0; i < 2 * job_pool_size; i++)
    {
        pool.add_job(root, []() {});
    }

    pool.wait(root);
}

ix_TEST_CASE("ix_ThreadPool: add_empty_job")
{
    ix_ThreadPool pool;
    ix_Job *root = pool.add_empty_job();
    pool.wait(root);
}

ix_TEST_CASE("ix_ThreadPool: shutdown empty pool")
{
    ix_ThreadPool pool;
    pool.shutdown();
}

ix_TEST_CASE("ix_ThreadPool: lazy spawn")
{
    ix_ThreadPoolOption option;
    option.job_pool_size = 128;
    option.spwan_workes_lazily = true;

    ix_Job *root;
    ix_ThreadPool pool(option);
    root = pool.add_empty_job();
    pool.wait(root);

    pool.shutdown();
    root = pool.add_empty_job();
    for (size_t i = 0; i < 512; i++)
    {
        pool.add_job(root, []() {});
    }

    pool.wait(root);

    pool.shutdown();
    root = pool.add_job([&]() {
        ix_Worker *worker = ix_ThreadPool::get_current_worker();

        for (size_t i = 0; i < 512; i++)
        {
            worker->add_job(root, []() {});
        }
    });
    pool.wait(root);
}

ix_TEST_CASE("ix_ThreadPool: many reallcoation")
{
    // See thread_pool_stress_test for serious test.
    for (size_t i = 0; i < 32; i++)
    {
        ix_ThreadPoolOption option;
        option.num_worker_threads = 3; // Make reallocation-related problems more likely.
        ix_ThreadPool pool(option);

        ix_Job *root = pool.add_empty_job();
        for (size_t j = 0; j < 16; j++)
        {
            pool.add_job(root, []() {});
        }
    }
}

ix_TEST_CASE("ix_ThreadPool: many reallcoation with many children")
{
    // See thread_pool_stress_test for serious test.
    for (size_t i = 0; i < 16; i++)
    {
        ix_ThreadPoolOption option;
        option.num_worker_threads = 3; // Make reallocation-related problems more likely.
        ix_ThreadPool pool(option);

        ix_Job *root = pool.add_empty_job();
        for (size_t j = 0; j < 16; j++)
        {
            pool.add_job(root, []() {
                ix_Worker *worker_0 = ix_ThreadPool::get_current_worker();
                ix_Job *job_0 = worker_0->get_current_job();
                worker_0->add_job(job_0, []() {
                    ix_Worker *worker_1 = ix_ThreadPool::get_current_worker();
                    ix_Job *job_1 = worker_1->get_current_job();
                    worker_1->add_job(job_1, []() { //
                        ix_Worker *worker_2 = ix_ThreadPool::get_current_worker();
                        ix_Job *job_2 = worker_2->get_current_job();
                        worker_2->add_job(job_2, []() {});
                    });
                });
            });
        }
    }
}
#endif
