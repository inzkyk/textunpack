#pragma once

#include <ix.hpp>

class ix_StringView;
class ix_Writer;
class ix_FileHandle;

const char *tup_find_first_entry_path_start(const char *p);
const char *tup_find_next_entry_path_start(const char *p);
size_t tup_calculate_entry_path_length(const char *entry_path_start);

void tup_unpack_entry(
    const ix_StringView &entry_path,
    const char *archive_end,
    ix_Writer *entry_writer,
    const ix_FileHandle *err_handle
);
