#pragma once
#include <cstdio>

bool dir_exists(const char* path);
char* read_file(const char* path);
size_t get_filesize(const char* path);
char** strspl(const char* str, const char tkn, size_t* len);
int write_file(const char* path, const char* mode, const char* buff, size_t buff_size);
