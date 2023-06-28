#pragma once

#include "config.h"

//Maximum Receive Buffer is set to 22 bytes
#define MAX_RX_BUFFER_BYTES 22

extern unsigned PORT;
extern Config* CONFIG;
extern unsigned TIMEOUT;
extern const char* LOGS_PATH;
extern unsigned char COMMAND_OUTPUT;