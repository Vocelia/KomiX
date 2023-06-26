#include <cstdio>
#include <cstdlib>
#include <libwebsockets.h>

#include "include/uigen.h"
#include "include/config.h"
#include "include/predefined.h"

unsigned int PORT;
unsigned int TIMEOUT;

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
      lwsl_notice("[INFO]: The connection to the client has been established!\n");
      break;
    case LWS_CALLBACK_RECEIVE:
      rx.len = len;
      memset(rx.data, 0, sizeof(rx.data)); //Clear rx buffer data
      memcpy(&rx.data[LWS_SEND_BUFFER_PRE_PADDING], in, len); //Copy data message to rx buffer
      lwsl_notice("[DEBUG]: Message -> \"%s\"\n", &rx.data[LWS_SEND_BUFFER_PRE_PADDING]);
      break;
    case LWS_CALLBACK_CLOSED:
      lwsl_notice("[INFO]: The connection to the client has been lost!\n");
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
  printf("Reading from server.ini...\n");
  Config* serv = new Config("./data/server.ini");
  PORT = atoi(serv->ini.GetValue("", "port"));
  TIMEOUT = atoi(serv->ini.GetValue("", "timeout"));
  printf("Initialised server constants successfully!\n");
  if (create_page()==-1) {
    printf("[ERR]: Failed to create a User Interface page!\n");
    return -1;
  }
  printf("Created a User Interface page successfully!\n");
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
  while (1) lws_service(context, TIMEOUT); //Timeout in milliseconds
  lws_context_destroy(context);
  return 0;
}