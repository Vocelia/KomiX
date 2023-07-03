#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "include/misc.h"
#include "include/predefined.h"

static time_t now;
static short backslash = 0; 

int execute_command(const char* cmd) {
  time(&now);
  char* output = new char[512];
  char cmd_buff[strlen(cmd)+5];
  sprintf(cmd_buff, "%s 2>&1", cmd);
  #ifdef _WIN32
    FILE* pfd = _popen(cmd_buff, "r");
  #else 
    FILE* pfd = popen(cmd_buff, "r");
  #endif
  //[Phase]: Error Handling
  if (!pfd) {
    printf("[ERR]: Failed to read from process \"%s\"!\n", cmd);
    return -1;
  }
  //Logging has a reserved section to avoid unnecessary computational complexity
  if (COMMAND_OUTPUT==2) {
    char cmdname[214];
    char filename[256];
    char ctime_plhdr[26];
    unsigned long cmd_len;
    char cmd_out[strlen(cmd)+4];
    char filepath[strlen(LOGS_PATH)+256];
    //[Phase]: Preparing a valid filepath
    char** cmd_spl = strspl(cmd, ' ', &cmd_len);
    #ifdef _WIN32
      const char* tkn = strrchr(cmd_spl[0], '/');
      if (tkn) { tkn++; snprintf(cmdname, 214, "%s", tkn); }
      else {
        tkn = strrchr(cmd_spl[0], '\\');
        if (tkn) { tkn++; snprintf(cmdname, 214, "%s", tkn); backslash = 1; }
        else snprintf(cmdname, 214, "%s", cmd_spl[0]);
      }
    #else
      const char* tkn = strrchr(cmd_spl[0], '/');
      if (tkn) { tkn++; snprintf(cmdname, 214, "%s", tkn); }
      else snprintf(cmdname, 214, "%s", cmd_spl[0]);
    #endif
    strcpy(ctime_plhdr, ctime(&now));
    ctime_plhdr[strlen(ctime(&now))-1] = '\0';
    #ifdef _WIN32
      strrplc(ctime_plhdr, ':', '-'); //Windows filename compatibility
    #endif
    sprintf(filename, "[%s]#%s#%lu.txt", cmdname, ctime_plhdr, clock());
    //214: Command name + 42: Date, Clock, and the rest of the string = 256: Filename limit
    if (!backslash) sprintf(filepath, "%s/%s", LOGS_PATH, filename);
    else sprintf(filepath, "%s\\%s", LOGS_PATH, filename);
    //[Phase]: Writing to the given filepath
    write_file(filepath, "a", ctime(&now), strlen(ctime(&now)));
    sprintf(cmd_out, "[%s]\n", cmd);
    write_file(filepath, "a", cmd_out, strlen(cmd_out));
    write_file(filepath, "a", "-------------------------------------------------\n", 50);
    while (fgets(output, 512, pfd) != NULL) {
      unsigned long output_len = strlen(output);
      if (output[output_len-1] == '\n') output[output_len] = '\0';
      write_file(filepath, "a", output, output_len);
    }
    write_file(filepath, "a", "-------------------------------------------------\n", 50);
  } else {
    switch (COMMAND_OUTPUT) {
      case 0: break;
      case 1:
        while (fgets(output, sizeof(output), pfd) != NULL) printf("%s", output);
        break;
      default:
        printf("[ERR]: Unavailable option: \"%d\" in command_output value!\n", COMMAND_OUTPUT);
        return -1;
        break;
    }
  }
  delete[] output;
  #ifdef _WIN32
    _pclose(pfd);
  #else 
    pclose(pfd);
  #endif
  return 0;
}