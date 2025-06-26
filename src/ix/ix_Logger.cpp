#include "ix_Logger.hpp"
#include "ix_HollowValue.hpp"
#include "ix_new.hpp"
#include "ix_time.hpp"

static ix_HollowValue<ix_Logger> g_logger;

ix_Result ix_global_logger_init()
{
    g_logger.construct(ix_LOGGER_DEBUG, &ix_FileHandle::of_stderr(), nullptr);
    return ix_OK;
}

ix_Result ix_global_logger_deinit()
{
    g_logger.destruct();
    return ix_OK;
}

ix_Logger &ix_get_global_logger()
{
    return g_logger.get();
}

ix_PRINTF_FORMAT(1, 2) void ix_log_fatal(ix_FORMAT_ARG const char *format, ...)
{
    va_list args;
    va_start(args, format);
    g_logger.get().log(ix_LOGGER_FATAL, format, args);
    va_end(args);
}

ix_PRINTF_FORMAT(1, 2) void ix_log_warning(ix_FORMAT_ARG const char *format, ...)
{
    va_list args;
    va_start(args, format);
    g_logger.get().log(ix_LOGGER_WARNING, format, args);
    va_end(args);
}

ix_PRINTF_FORMAT(1, 2) void ix_log_debug(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    g_logger.get().log(ix_LOGGER_DEBUG, format, args);
    va_end(args);
}

static constexpr const char *severity_strings[] = {
    "DEBUG",   //
    "WARNING", //
    "FATAL",   //
};

ix_Logger::ix_Logger(ix_LoggerSeverity min_severity, const ix_FileHandle *file, const char *header)
    : m_writer(0, file),
      m_min_severity(min_severity),
      m_header(header)
{
    if (file == nullptr)
    {
        m_writer.reserve_buffer_capacity(4096);
    }

    m_start_tick = ix_time_now();
    m_previous_tick = m_start_tick;
}

void ix_Logger::set_min_severity(ix_LoggerSeverity new_min_severity)
{
    m_min_severity = new_min_severity;
}

ix_LoggerSeverity ix_Logger::get_min_severity() const
{
    return m_min_severity;
}

ix_PRINTF_FORMAT(3, 0) void ix_Logger::log(ix_LoggerSeverity severity, ix_FORMAT_ARG const char *format, va_list args)
{
    if (severity < m_min_severity)
    {
        return;
    }

    print_main(severity, format, args);
}

ix_PRINTF_FORMAT(2, 3) void ix_Logger::log_fatal(ix_FORMAT_ARG const char *format, ...)
{
    if (ix_LOGGER_FATAL < m_min_severity)
    {
        return;
    }

    va_list args;
    va_start(args, format);
    print_main(ix_LOGGER_FATAL, format, args);
    va_end(args);
}

ix_PRINTF_FORMAT(2, 3) void ix_Logger::log_warning(ix_FORMAT_ARG const char *format, ...)
{
    if (ix_LOGGER_WARNING < m_min_severity)
    {
        return;
    }

    va_list args;
    va_start(args, format);
    print_main(ix_LOGGER_WARNING, format, args);
    va_end(args);
}

ix_PRINTF_FORMAT(2, 3) void ix_Logger::log_debug(ix_FORMAT_ARG const char *format, ...)
{
    if (ix_LOGGER_DEBUG < m_min_severity)
    {
        return;
    }

    va_list args;
    va_start(args, format);
    print_main(ix_LOGGER_DEBUG, format, args);
    va_end(args);
}

void ix_Logger::print_header(uint64_t now, ix_LoggerSeverity severity)
{
    const double ms_from_start = ix_time_diff_to_ms(now - m_start_tick);
    const double ms_from_previous = ix_time_diff_to_ms(now - m_previous_tick);

    const char *severity_string = severity_strings[static_cast<size_t>(severity)];
    if (m_header != nullptr)
    {
        m_writer.write_stringf(
            "[%09.4f ms] [%09.4f ms] [%s::%s] ",
            ms_from_start,
            ms_from_previous,
            m_header,
            severity_string
        );
    }
    else
    {
        m_writer.write_stringf(
            "[%09.4f ms] [%09.4f ms] [%s] ", //
            ms_from_start,
            ms_from_previous,
            severity_string
        );
    }
}

ix_PRINTF_FORMAT(3, 0) void ix_Logger::print_main(ix_LoggerSeverity severity, const char *format, va_list args)
{
    const uint64_t now = ix_time_now();

    m_mutex.lock();
    print_header(now, severity);
    m_writer.write_stringfv(format, args);
    m_writer.write_char('\n');
    m_mutex.unlock();

    m_previous_tick = now;
}

#if ix_DO_TEST
#include "ix_string.hpp"
#include "ix_test.hpp"

static const char *extract_string_from_logger(ix_Logger &logger)
{
    const char *p;
    logger.log_custom([&p](ix_Writer &writer) {
        writer.end_string();
        p = writer.data();
    });
    return p;
}

ix_TEST_CASE("ix_Logger")
{
    {
        ix_Logger logger(ix_LOGGER_DEBUG, nullptr, "TestLogger");
        logger.log_fatal("fatal");
        logger.log_warning("warning");
        logger.log_debug("debug");
        const char *s = extract_string_from_logger(logger);
        ix_EXPECT(ix_strstr(s, "[TestLogger::FATAL] fatal\n") != nullptr);
        ix_EXPECT(ix_strstr(s, "[TestLogger::WARNING] warning\n") != nullptr);
        ix_EXPECT(ix_strstr(s, "[TestLogger::DEBUG] debug\n") != nullptr);
    }

    {
        ix_Logger logger(ix_LOGGER_DEBUG, nullptr, nullptr);
        logger.log_fatal("fatal");
        logger.log_warning("warning");
        logger.log_debug("debug");
        const char *s = extract_string_from_logger(logger);
        ix_EXPECT(ix_strstr(s, "[FATAL] fatal\n") != nullptr);
        ix_EXPECT(ix_strstr(s, "[WARNING] warning\n") != nullptr);
        ix_EXPECT(ix_strstr(s, "[DEBUG] debug\n") != nullptr);
    }

    {
        ix_Logger logger(ix_LOGGER_DEBUG, nullptr, "TestLogger");
        logger.set_min_severity(ix_LOGGER_FATAL);
        logger.log_fatal("fatal");
        logger.log_warning("warning");
        logger.log_debug("debug");
        const char *s = extract_string_from_logger(logger);
        ix_EXPECT(ix_strstr(s, "[TestLogger::FATAL] fatal\n") != nullptr);
    }

    {
        ix_Logger logger(ix_LOGGER_INFINITE, nullptr, "TestLogger");
        logger.log_fatal("fatal");
        logger.log_warning("warning");
        logger.log_debug("debug");
        const char *s = extract_string_from_logger(logger);
        ix_EXPECT_EQSTR(s, "");
    }

    {
        ix_Logger &logger = ix_get_global_logger();
        const ix_LoggerSeverity old = logger.get_min_severity();
        logger.set_min_severity(ix_LOGGER_INFINITE);
        ix_log_fatal("fatal");
        ix_log_warning("warning");
        ix_log_debug("debug");
        logger.set_min_severity(old);
    }
}
#endif
