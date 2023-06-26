#include <cstdio>

size_t get_filesize(const char* path) {
  FILE* file = fopen(path, "r");
  fseek(file, 0, SEEK_END); //places the cursor at the EOF
  size_t file_size = ftell(file); //gets size of file through EOF
  fclose(file);
  return file_size;
}

char* read_file(const char* path) {
  FILE* file = fopen(path, "r");
  size_t buff_size = get_filesize(path);
  char* buff = new char[buff_size];
  size_t progress = fread(buff, sizeof(buff[0]), buff_size, file);
  fclose(file);
  if (progress != buff_size) {
    printf("[ERR]: Failed to read the entire content of \"%s\"!\n", path);
    return nullptr;
  }
  return buff;
}

int write_file(const char* path, const char* buff, size_t buff_size) {
  FILE* file = fopen(path, "w");
  size_t progress = fwrite(buff, sizeof(buff[0]), buff_size, file);
  fclose(file);
  if (progress != buff_size) {
    printf("[ERR]: Failed to write the entire content of \"%s\"!\n", path);
    return -1;
  }
  return 0;
}