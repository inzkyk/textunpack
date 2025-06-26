#pragma once

#include "ix.hpp"
#include "ix_Mutex.hpp"
#include "ix_Writer.hpp"
#include "ix_file.hpp"

class ix_Logger;

ix_Result ix_global_logger_init();
ix_Result ix_global_logger_deinit();
ix_Logger &ix_get_global_logger();

enum ix_LoggerSeverity : uint8_t
{
    ix_LOGGER_DEBUG,
    ix_LOGGER_WARNING,
    ix_LOGGER_FATAL,
    ix_LOGGER_INFINITE,
};

class ix_Logger
{
    ix_Writer m_writer;
    ix_Mutex m_mutex;
    ix_LoggerSeverity m_min_severity;
    const char *m_header;

    uint64_t m_start_tick;
    uint64_t m_previous_tick;

  public:
    ix_Logger(ix_LoggerSeverity min_severity, const ix_FileHandle *file, const char *header);
    void set_min_severity(ix_LoggerSeverity new_min_severity);
    ix_LoggerSeverity get_min_severity() const;

    ix_PRINTF_FORMAT(3, 0) void log(ix_LoggerSeverity severity, ix_FORMAT_ARG const char *format, va_list args);
    ix_PRINTF_FORMAT(2, 3) void log_fatal(ix_FORMAT_ARG const char *format, ...);
    ix_PRINTF_FORMAT(2, 3) void log_warning(ix_FORMAT_ARG const char *format, ...);
    ix_PRINTF_FORMAT(2, 3) void log_debug(ix_FORMAT_ARG const char *format, ...);

    template <typename F>
    void log_custom(const F &f)
    {
        m_mutex.lock();
        f(m_writer);
        m_mutex.unlock();
    }

  private:
    void print_header(uint64_t now, ix_LoggerSeverity severity);
    ix_PRINTF_FORMAT(3, 0) void print_main(ix_LoggerSeverity severity, const char *format, va_list args);
};
