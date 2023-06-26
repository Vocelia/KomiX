#pragma once
#include <cstdio>

char* read_file(const char* path);
size_t get_filesize(const char* path);
int write_file(const char* path, const char* buff, size_t buff_size);
