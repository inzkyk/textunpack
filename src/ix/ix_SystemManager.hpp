#pragma once

#include "ix.hpp"
#include "ix_HashMapSingleArray.hpp"
#include "ix_Result.hpp"

template <typename T>
class ix_HollowValue;

class ix_SystemManager
{
    ix_StringHashMapSingleArray<ix_Result (*)()> m_system_name_to_deinitializer;

  public:
    static bool is_initialized();
    static ix_SystemManager &initialize();
    static ix_SystemManager &get();
    static void deinitialize();

    ix_SystemManager(const ix_SystemManager &) = delete;
    ix_SystemManager(ix_SystemManager &&) = default;
    ix_SystemManager &operator=(const ix_SystemManager &) = delete;
    ix_SystemManager &operator=(ix_SystemManager &&) = default;

    bool is_initialized(const char *name) const;
    void assert_initialized(const char *name) const;

    // We cannot create `init_system(const char *name)' because that makes it impossible to strip unused init
    // functions from the final executable.
    ix_Result initialize_stdio();
    ix_Result initialize_time();
    ix_Result initialize_logger();
    ix_Result initialize_dummy();
    ix_Result initialize_dummy_nullptr();

  private:
    friend class ix_HollowValue<ix_SystemManager>;

    ix_SystemManager();
    ~ix_SystemManager();
};
