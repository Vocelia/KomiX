#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <errno.h>

#ifdef _WIN32
#include <windows.h>
bool dir_exists(const char* path) {
  DWORD attrib = GetFileAttributesA(path);
  return (attrib != INVALID_FILE_ATTRIBUTES && (attrib & FILE_ATTRIBUTE_DIRECTORY));
}
#else
#include <dirent.h>
bool dir_exists(const char* path) {
  DIR* dir = opendir(path);
  if (dir) closedir(dir);
  else if (ENOENT == errno) return false;
  else return false;
  return true;
}
#endif

//str: String to be split
//delim: Delimiter to split upon
//len: Pointer to a length variable
char** strspl(const char* str, const char delim, size_t* len) {
  char** spl;
  unsigned i;
  size_t words = 0;
  char* tmp = (char*)malloc(strlen(str)*sizeof(char));
  memcpy(tmp, str, strlen(str));
  char* bgn_tmp = tmp;
  //Count up the words and splitting them
  while ((tmp=strchr(tmp, delim))!=NULL) {
	  sprintf(tmp, ""); words++; tmp++;
  } words++;
  tmp = bgn_tmp; //Reset offset
  spl = (char**)calloc(1, words*sizeof(char*));
  if (!spl) return NULL;
  //Assign each word to spl members
  for (i=0; i<words; i++) {
	  if (*tmp) spl[i] = tmp;
	  tmp = strchr(tmp, '\0'); tmp++;
  } 
  *len = words;
  return spl;
}

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

int write_file(const char* path, const char* mode, const char* buff, size_t buff_size) {
  FILE* file = fopen(path, mode);
  size_t progress = fwrite(buff, sizeof(buff[0]), buff_size, file);
  fclose(file);
  if (progress != buff_size) {
    printf("[ERR]: Failed to write the entire content of \"%s\"!\n", path);
    return -1;
  }
  return 0;
}
