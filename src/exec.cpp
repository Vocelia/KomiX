#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "include/misc.h"
#include "include/predefined.h"

static time_t now;

int execute_command(const char* cmd) {
  char cmd_buff[strlen(cmd)+5];
  sprintf(cmd_buff, "%s 2>&1", cmd);
  #ifdef _WIN32
    FILE* pfd = _popen(cmd_buff, "r");
  #else 
    FILE* pfd = popen(cmd_buff, "r");
  #endif
  char* output = new char[512];
  if (!pfd) {
    printf("[ERR]: Failed to read from process \"%s\"!\n", cmd);
    return -1;
  }
  switch (COMMAND_OUTPUT) {
    case 0: break;
    case 1:
      while (fgets(output, sizeof(output), pfd) != NULL) printf("%s", output);
      break;
    case 2:
      time(&now);
      write_file(LOGS_PATH, "a", ctime(&now), strlen(ctime(&now)));
      write_file(LOGS_PATH, "a", "-------------------------------------------------\n", 50);
      while (fgets(output, 512, pfd) != NULL) {
        size_t output_len = strlen(output);
        if (output[output_len-1] == '\n') output[output_len] = '\0';
        write_file(LOGS_PATH, "a", output, output_len);
      }
      write_file(LOGS_PATH, "a", "-------------------------------------------------\n\n", 51);
      break;
    default:
      printf("[ERR]: Unavailable option: \"%d\" in command_output value!\n", COMMAND_OUTPUT);
      return -1;
      break;
  }
  delete[] output;
  #ifdef _WIN32
    _pclose(pfd);
  #else 
    pclose(pfd);
  #endif
  return 0;
}