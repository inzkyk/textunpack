#pragma once

#include "ix.hpp"
#include "ix_Function.hpp"
#include "ix_Thread.hpp"
#include "ix_Vector.hpp"
#include "ix_random.hpp"

// TODO: Benchmark (with realistic workloads)
// TODO: Remove busy-wait
// TODO: Implement the caching scheme described in the paper

class ix_Job;
class ix_Worker;

class ix_JobStealingDeque
{
    // cf.
    // - "Dynamic Circular Work-Stealing Deque" by David Chase and Yossi Lev
    // - "Correct and Efficient Work-Stealing for Weak Memory Models" by Nhat Minh Lê et. al.
    // - "Job System 2.0: Lock-Free Work Stealing" by Stefan Reinalter

    struct JobArray
    {
        uint64_t capacity;
        ix_Job **data;
    };

    ix_DISABLE_MSVC_WARNING_BEGIN
    ix_DISABLE_MSVC_WARNING(4324) // structure was padded due to alignment specifier
    alignas(64) uint64_t m_initial_capacity;
    alignas(64) uint64_t m_top;
    alignas(64) uint64_t m_bottom;
    alignas(64) JobArray *m_job_array;
    alignas(64) ix_Vector<void *> m_garbages;
    ix_DISABLE_MSVC_WARNING_END

  public:
    ix_JobStealingDeque(const ix_JobStealingDeque &) = delete;
    ix_JobStealingDeque &operator=(const ix_JobStealingDeque &) = delete;
    ix_JobStealingDeque(ix_JobStealingDeque &&other);
    ix_JobStealingDeque &operator=(ix_JobStealingDeque &&other);

    explicit ix_JobStealingDeque(size_t initial_capacity);
    ~ix_JobStealingDeque();
    void free_memory();
    size_t size() const;
    void push_bottom(ix_Job *job);
    ix_Job *pop_bottom();
    ix_Job *steal_top();
};

struct ix_ThreadPoolOption
{
    size_t num_worker_threads;
    size_t initial_job_queue_capacity;
    size_t job_pool_size;
    bool spwan_workes_lazily;

    ix_ThreadPoolOption();
};

class ix_ThreadPool
{
    friend class ix_Worker;

    ix_ThreadPoolOption m_option;
    ix_Vector<ix_Thread> m_extra_worker_threads;
    ix_Vector<ix_Worker> m_workers;

  public:
    explicit ix_ThreadPool(const ix_ThreadPoolOption &option = ix_ThreadPoolOption());
    ~ix_ThreadPool();

    ix_ThreadPool(const ix_ThreadPool &) = delete;
    ix_ThreadPool(ix_ThreadPool &&) = delete;
    ix_ThreadPool &operator=(const ix_ThreadPool &) = delete;
    ix_ThreadPool &operator=(ix_ThreadPool &&) = delete;

    size_t size() const;
    void wait(ix_Job *job);
    void shutdown();
    static ix_Worker *get_current_worker();
    static ix_Job *get_current_job();

    [[nodiscard]] ix_Job *add_empty_job();
    [[nodiscard]] ix_Job *add_job(const ix_FunctionN<16, void()> &job_entry);
    ix_Job *add_job(ix_Job *parent, const ix_FunctionN<16, void()> &job_entry);

  private:
    void spawn_worker();
};

class ix_Job
{
    friend class ix_ThreadPool;
    friend class ix_Worker;
    friend class ix_JobStealingDeque;

    ix_FunctionN<16, void()> entry;
    int64_t ttl = 0;
    ix_Job *parent;
};

class ix_Worker
{
    friend class ix_ThreadPool;
    friend class ix_Vector<ix_Worker>;

    uint64_t m_next_job_index = 0;
    uint64_t m_index;
    bool m_should_die = false;
    ix_ThreadPool *m_thread_pool;
    ix_Vector<ix_Job> m_job_pool;
    ix_Vector<ix_Job *> m_job_stack;
    ix_Xoshiro256PlusPlus m_rng;
    ix_JobStealingDeque m_jobs;

  public:
    size_t index() const;
    void wait(ix_Job *job);
    ix_Job *get_current_job() const;

    [[nodiscard]] ix_Job *add_empty_job();
    [[nodiscard]] ix_Job *add_job(const ix_FunctionN<16, void()> &job_entry);
    ix_Job *add_job(ix_Job *parent, const ix_FunctionN<16, void()> &job_entry);

  private:
    ix_Worker(ix_ThreadPool *thread_pool, size_t index, const ix_ThreadPoolOption &option);
    ix_Job *get_executable_job();
    ix_Job *allocate_job();
    ix_Job *allocate_job(const ix_FunctionN<16, void()> &job_entry, ix_Job *parent);
    void execute_job_maybe();
    static void finalize_job(ix_Job *job);
};
