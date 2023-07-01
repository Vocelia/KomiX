#include <thread>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <errno.h>

#include "include/exec.h"

#ifdef _WIN32
#include <windows.h>
bool dir_exists(const char* path) {
  DWORD attrib = GetFileAttributesA(path);
  return (attrib != INVALID_FILE_ATTRIBUTES && (attrib & FILE_ATTRIBUTE_DIRECTORY));
}

char* getLocalIPAddr() {
  char* phldr = new char[512];
  char* output = new char[512];
  FILE* pfd = _popen("ipconfig | findstr IPv4", "r");
  while (fgets(output, sizeof(output), pfd)!=NULL) strcat(phldr, output);
  char* tkn = new char[strlen(phldr)];
  tkn = strrchr(phldr, ' '); tkn++;
  tkn[strlen(tkn)-1] = '\0';
  delete[] output;
  _pclose(pfd);
  return tkn;
}
#else
#include <dirent.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
bool dir_exists(const char* path) {
  DIR* dir = opendir(path);
  if (dir) closedir(dir);
  else if (ENOENT == errno) return false;
  else return false;
  return true;
}

char* getLocalIPAddr() {
  struct ifaddrs* ifa;
  struct ifaddrs* ifap;
  char* IP = new char[INET_ADDRSTRLEN];
  getifaddrs(&ifap);
  for (ifa=ifap; ifa!=NULL; ifa=ifa->ifa_next) {
    if (ifa->ifa_addr!=NULL && ifa->ifa_addr->sa_family==AF_INET && strcmp(ifa->ifa_name, "lo")) {
      struct sockaddr_in* addr = (struct sockaddr_in*)ifa->ifa_addr;
      IP = inet_ntoa(addr->sin_addr);
      break;
    }
  }
  freeifaddrs(ifap);
  return IP;
}
#endif

void execCmdAsync(const char* cmd) {
  std::thread thr(execute_command, cmd);
  thr.detach();
}

//str: String to be split
//delim: Delimiter to split upon
//len: Pointer to a length variable
char** strspl(const char* str, const char delim, unsigned long* len) {
  char** spl;
  unsigned long i;
  unsigned long words = 0;
  char* tmp = (char*)malloc(strlen(str)*sizeof(char));
  memcpy(tmp, str, strlen(str));
  char* bgn_tmp = tmp;
  //Count up the words and splitting them
  while ((tmp=strchr(tmp, delim))!=NULL) {
	  *tmp = '\0'; words++; tmp++;
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

unsigned long get_filesize(const char* path) {
  FILE* file = fopen(path, "r");
  fseek(file, 0, SEEK_END); //places the cursor at the EOF
  unsigned long file_size = ftell(file); //gets size of file through EOF
  fclose(file);
  return file_size;
}

char* read_file(const char* path) {
  FILE* file = fopen(path, "r");
  unsigned long buff_size = get_filesize(path);
  char* buff = new char[buff_size];
  unsigned long progress = fread(buff, sizeof(buff[0]), buff_size, file);
  fclose(file);
  if (progress != buff_size) {
    printf("[ERR]: Failed to read the entire content of \"%s\"!\n", path);
    return nullptr;
  }
  return buff;
}

int write_file(const char* path, const char* mode, const char* buff, unsigned long buff_size) {
  FILE* file = fopen(path, mode);
  unsigned long progress = fwrite(buff, sizeof(buff[0]), buff_size, file);
  fclose(file);
  if (progress != buff_size) {
    printf("[ERR]: Failed to write the entire content of \"%s\"!\n", path);
    return -1;
  }
  return 0;
}