#pragma once

bool dir_exists(const char* path);
char* read_file(const char* path);
unsigned long get_filesize(const char* path);
char** strspl(const char* str, const char tkn, unsigned long* len);
int write_file(const char* path, const char* mode, const char* buff, unsigned long buff_size);
