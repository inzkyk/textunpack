#pragma once

#include "ix.hpp"
#include "ix_Result.hpp"
#include "ix_printf.hpp"

class ix_FileHandle;

enum ix_FileOpenOption : uint8_t
{
    ix_READ_ONLY,
    ix_WRITE_ONLY,
};

ix_Result ix_initialize_stdio();
bool ix_is_file(const char *path);
bool ix_is_directory(const char *path);
ix_Result ix_remove_file(const char *path);
ix_Result ix_create_directory(const char *path);
ix_Result ix_remove_directory(const char *path);
ix_Result ix_ensure_directories(const char *path);
ix_Option<ix_FileHandle> ix_open_file(const char *path, ix_FileOpenOption option);
ix_Option<ix_FileHandle> ix_create_directories_and_file(const char *path);
ix_Result ix_write_to_file(const char *filename, const char *data, size_t data_length);
ix_Result ix_write_string_to_file(const char *filename, const char *str);
ix_Option<ix_UniquePointer<char[]>> ix_load_file(
    const char *filename,
    size_t *data_length_out = nullptr,
    size_t padding_length = 1
);

class ix_FileHandle
{
    friend ix_Result ix_initialize_stdio();
    friend ix_Option<ix_FileHandle> ix_open_file(const char *path, ix_FileOpenOption option);

    union
    {
        int fd;
        HANDLE handle;
    } v;

  public:
    ix_FileHandle();
    ~ix_FileHandle();
    ix_FileHandle(const ix_FileHandle &other) = delete;
    ix_FileHandle &operator=(const ix_FileHandle &) = delete;
    ix_FileHandle(ix_FileHandle &&other);
    ix_FileHandle &operator=(ix_FileHandle &&other);

    void close();
    bool is_valid() const;
    HANDLE get_native_handle() const;
    int get_native_fd() const;
    void write(const void *data, size_t data_length) const;
    void write_string(const char *str) const;
    void write_char(char c) const;
    ix_PRINTF_FORMAT(2, 0) size_t write_stringfv(ix_FORMAT_ARG const char *format, va_list args) const;
    ix_PRINTF_FORMAT(2, 3) size_t write_stringf(ix_FORMAT_ARG const char *format, ...) const;
    size_t read(void *buffer, size_t buffer_length) const;
    ix_Option<ix_UniquePointer<char[]>> read_all(
        size_t *data_length_out = nullptr,
        size_t padding_length = 1,
        size_t first_read_size = 1024
    ) const;
    size_t size() const;
    void substitute(ix_FileHandle &&another_file);
    ix_FileHandle copy() const;

    static ix_FileHandle null(ix_FileOpenOption option = ix_WRITE_ONLY);
    static const ix_FileHandle &of_stdin();
    static const ix_FileHandle &of_stdout();
    static const ix_FileHandle &of_stderr();

  private:
    void invalidate();
};
