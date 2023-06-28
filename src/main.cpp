#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <libwebsockets.h>

#include "include/misc.h"
#include "include/exec.h"
#include "include/uigen.h"
#include "include/config.h"
#include "include/predefined.h"

unsigned PORT;
Config* CONFIG;
unsigned TIMEOUT;
const char* LOGS_PATH;
unsigned char COMMAND_OUTPUT;

static size_t len_ptr;
static char** id_phldr;

struct payload {
  size_t len;
  char data[LWS_SEND_BUFFER_PRE_PADDING+MAX_RX_BUFFER_BYTES+LWS_SEND_BUFFER_POST_PADDING];
} rx;

//wsi: Websockets Instance
//in: data message
static int http_cb(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len) {
  switch(reason) {
    case LWS_CALLBACK_HTTP:
      lws_serve_http_file(wsi, "./ui/index.html", "text/html", NULL, 0);
      break;
    default: break;
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
      id_phldr = strspl(&rx.data[LWS_SEND_BUFFER_PRE_PADDING], '-', &len_ptr);
      CONFIG->get_all_keys(CONFIG->sections.names[atoi(id_phldr[0])]);
      execute_command(CONFIG->ini.GetValue(CONFIG->sections.names[atoi(id_phldr[0])], CONFIG->keys.names[atoi(id_phldr[1])]));
      free(id_phldr);
      break;
    case LWS_CALLBACK_CLOSED:
      lwsl_notice("[INFO]: The connection to the client has been lost!\n");
    default: break;
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
  LOGS_PATH = serv->ini.GetValue("", "command_output_logs_path");
  COMMAND_OUTPUT = atoi(serv->ini.GetValue("", "command_output"));
  printf("Initialised server constants successfully!\n");
  printf("Checking the validity of LOGS_PATH directory...\n");
  char* LOGS_PATH_RW = new char[strlen(LOGS_PATH)];
  memcpy(LOGS_PATH_RW, LOGS_PATH, strlen(LOGS_PATH));
  char* tkn = strrchr(LOGS_PATH_RW, '/');
  if (tkn==NULL) tkn = strrchr(LOGS_PATH_RW, '\\');
  memset(tkn, 0, strlen(tkn));
  if (!dir_exists(LOGS_PATH_RW)) {
    printf("[ERR]: The path provided isn't valid!\n");
    return -1;
  }
  delete[] LOGS_PATH_RW;
  printf("LOGS_PATH is a valid directory!\n");
  printf("Reading from config.ini...\n");
  CONFIG = new Config("./data/config.ini");
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
    printf("[ERR]: Failed to initialise context creation information.\n");
    return -1;
  }
  printf("Initialised context creation information successfully!\n");
  while (1) lws_service(context, TIMEOUT); //Timeout in milliseconds
  lws_context_destroy(context);
  return 0;
}