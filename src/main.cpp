#include <cstdio>
#include <cstdlib>
#include <libwebsockets.h>

#include "include/config.hpp"

//Maximum Receive Buffer is set to 8 bytes
#define MAX_RX_BUFFER_BYTES 8
int PORT;

struct payload {
  size_t len;
  unsigned char data[LWS_SEND_BUFFER_PRE_PADDING+MAX_RX_BUFFER_BYTES+LWS_SEND_BUFFER_POST_PADDING];
} rx;

//wsi: Websockets Instance
//in: data message
static int http_cb(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len) {
  switch(reason) {
    case LWS_CALLBACK_HTTP:
      lws_serve_http_file(wsi, "./ui/index.html", "text/html", NULL, 0);
      break;
    default:
      break;
  }
  return 0;
}

static int komi_cb(struct lws* wsi, enum lws_callback_reasons reason, void* user, void* in, size_t len) {
  switch (reason) {
    case LWS_CALLBACK_ESTABLISHED:
      lwsl_notice("The connection to the client has been established!\n");
      break;
    case LWS_CALLBACK_RECEIVE:
      lwsl_notice("Received client message successfully!\n");
      rx.len = len;
      memset(rx.data, 0, sizeof(rx.data)); //Clear rx buffer data
      memcpy(&rx.data[LWS_SEND_BUFFER_PRE_PADDING], in, len); //Copy data message to rx buffer
      lwsl_notice("[DEBUG] Message: %s\n", &rx.data[LWS_SEND_BUFFER_PRE_PADDING]);
      break;
    case LWS_CALLBACK_CLOSED:
      lwsl_notice("The connection to the client has been lost!\n");
    default:
      break;
  }
  return 0;
}

static struct lws_protocols protocols[] = {
  { "http-only", http_cb, 0, 0 },
  { "KomiX", komi_cb, 0, 0 },
  { NULL, NULL, 0, 0 } //Terminator protocol
};

int main(int argc, char** argv) {
  switch (argc) {
    case 1:
      PORT = 8080;
      break;
    case 2:
      if (!strcmp(argv[1], "--help")) {
        printf("komix [PORT]\nEX: komix 8080\n");
        return 0;
      } else if (!strcmp(argv[1], "-h")) {
        printf("komix [PORT]\nEX: komix 8080\n");
        return 0;
      } else {
        PORT = atoi(argv[1]);
        printf("Listening on port %d...\n", PORT); 
      }
      break;
    default:
      break;
  }
  printf("Reading from config.json...\n");
  std::map<std::string, std::map<std::string, std::string>> conf = getConfig("../config.json");
  printf("Initialising context creation information...\n");
  struct lws_context_creation_info info;
  memset(&info, 0, sizeof(info));
  info.gid = -1;
  info.uid = -1;
  info.port = PORT;
  info.protocols = protocols;
  struct lws_context* context = lws_create_context(&info);
  if (!context) {
    printf("[ERR]: Failed to create context information.\n");
    return -1;
  }
  printf("Created context information successfully!\n");
  while (1) lws_service(context, 1000); //Timeout in milliseconds
  lws_context_destroy(context);
  return 0;
}