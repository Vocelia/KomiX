#include <libwebsockets.h>

unsigned int PORT;
unsigned int TIMEOUT;
//Maximum Receive Buffer is set to 8 bytes
#define MAX_RX_BUFFER_BYTES 8

struct payload {
  size_t len;
  unsigned char data[LWS_SEND_BUFFER_PRE_PADDING+MAX_RX_BUFFER_BYTES+LWS_SEND_BUFFER_POST_PADDING];
} rx;

const char* head = "<!DOCTYPE html> \
<html> \
<head> \
<title>KomiX</title> \
<link rel=\"stylesheet\" href=\"index.css\"> \
<link rel=\"stylesheet\" href=\"pico.min.css\"> \
<meta name=\"viewport\" content=\"width=device-width, initial-scale=1, maximum-scale=1, user-scalable=0\"> \
</head> \
<body>\n";
const char* script = "</body>\n<script>\n";
const char* footer = "</script>\n</html>";