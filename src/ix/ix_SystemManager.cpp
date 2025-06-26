#include "ix_SystemManager.hpp"
#include "ix_HollowValue.hpp"
#include "ix_Logger.hpp"
#include "ix_file.hpp"
#include "ix_time.hpp"

static bool g_system_manager_initialized;
static ix_HollowValue<ix_SystemManager> g_system_manager;

bool ix_SystemManager::is_initialized()
{
    return g_system_manager_initialized;
}

ix_SystemManager &ix_SystemManager::initialize()
{
    ix_ASSERT_FATAL(!g_system_manager_initialized, "ix_SystemManager: Already initialized");
    g_system_manager_initialized = true;
    ix_SystemManager &sm = g_system_manager.construct();
    return sm;
}

ix_SystemManager &ix_SystemManager::get()
{
    return g_system_manager.get();
}

void ix_SystemManager::deinitialize()
{
    g_system_manager.destruct();
}

ix_SystemManager::ix_SystemManager()
{
    m_system_name_to_deinitializer.reserve(8);
}

ix_SystemManager::~ix_SystemManager()
{
    for (const auto &[name, deinitializer] : m_system_name_to_deinitializer)
    {
        if (deinitializer != nullptr)
        {
            const ix_Result res = deinitializer();
            res.assert_ok();
        }
    }
}

#define RETURN_ERROR_IF_INITIALIZED(name)        \
    do                                           \
    {                                            \
        if (is_initialized(name))                \
        {                                        \
            return ix_ERROR_ALREADY_INITIALIZED; \
        }                                        \
    } while (0)

#define RETURN_ERROR_IF_NOT_INITIALIZED(name) \
    do                                        \
    {                                         \
        if (!is_initialized(name))            \
        {                                     \
            return ix_ERROR_NOT_INITIALIZED;  \
        }                                     \
    } while (0)

ix_Result ix_SystemManager::initialize_stdio()
{
    RETURN_ERROR_IF_INITIALIZED("stdio");
    m_system_name_to_deinitializer.emplace("stdio", nullptr);
    return ix_initialize_stdio();
}

ix_Result ix_SystemManager::initialize_time()
{
    RETURN_ERROR_IF_INITIALIZED("time");
    m_system_name_to_deinitializer.emplace("time", nullptr);
    return ix_time_init();
}

ix_Result ix_SystemManager::initialize_logger()
{
    RETURN_ERROR_IF_INITIALIZED("logger");
    RETURN_ERROR_IF_NOT_INITIALIZED("stdio");
    RETURN_ERROR_IF_NOT_INITIALIZED("time");
    m_system_name_to_deinitializer.emplace("logger", []() { return ix_global_logger_deinit(); });
    return ix_global_logger_init();
}

ix_Result ix_SystemManager::initialize_dummy()
{
    RETURN_ERROR_IF_INITIALIZED("dummy");
    m_system_name_to_deinitializer.emplace("dummy", []() -> ix_Result { return ix_OK; });
    return ix_OK;
}

ix_Result ix_SystemManager::initialize_dummy_nullptr()
{
    RETURN_ERROR_IF_INITIALIZED("dummy_nullptr");
    m_system_name_to_deinitializer.emplace("dummy_nullptr", nullptr);
    return ix_OK;
}

bool ix_SystemManager::is_initialized(const char *name) const
{
    const auto *deinitializer = m_system_name_to_deinitializer.find(name);
    return (deinitializer != nullptr);
}

void ix_SystemManager::assert_initialized(const char *name) const
{
    const auto *deinitializer = m_system_name_to_deinitializer.find(name);
    const bool initialzed = deinitializer != nullptr;
    if (initialzed)
    {
        return;
    }

    char buf[64];
    ix_asnprintf(buf, "ix_SystemManager: %s must be initialized", name);
    ix_ASSERT_FATAL(false, buf);
}

#if ix_DO_TEST
#include "ix_test.hpp"

ix_TEST_CASE("ix_SystemManager")
{
    auto &system_manager = ix_SystemManager::get();
    ix_EXPECT(system_manager.initialize_dummy().is_ok());
    ix_EXPECT(system_manager.initialize_dummy_nullptr().is_ok());
    ix_EXPECT(system_manager.initialize_dummy().is_error());
    ix_EXPECT(system_manager.initialize_dummy_nullptr().is_error());

    const auto &sm = ix_SystemManager::get();
    ix_EXPECT(sm.is_initialized("stdio"));
    ix_EXPECT(sm.is_initialized("time"));
    ix_EXPECT(sm.is_initialized("logger"));
}
#endif
