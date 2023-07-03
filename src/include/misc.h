#pragma once

char* getLocalIPAddr();
bool dir_exists(const char* path);
char* read_file(const char* path);
void execCmdAsync(const char* cmd);
unsigned long get_filesize(const char* path);
void strrplc(char* str, char tkn, char new_tkn);
char** strspl(const char* str, const char tkn, unsigned long* len);
int write_file(const char* path, const char* mode, const char* buff, unsigned long buff_size);
