#include <cstdio>
#include <cstring>
#include <cstdlib>

#include "include/misc.h"
#include "include/config.h"
#include "include/predefined.h"
#include "include/wrapper/String.h"

static String html_dump;
static const char* HEAD = "<!DOCTYPE html>\n<html>\n<head>\n<title>KomiX</title>\n \
<meta name=\"viewport\" content=\"width=device-width, initial-scale=1, maximum-scale=1, user-scalable=0\">\n<style>\n";
static const char* STYLE = "</style>\n</head>\n<body>\n";
static const char* SCRIPT = "</body>\n<script>\n";
static const char* FOOTER = "</script>\n</html>";

int create_page() {
  if (CONFIG->get_all_sections()==-1) {
    printf("[ERR]: Failed to get all sections of CONFIGig.ini!\n");
    return -1;
  }
  //Reading from index.css, pico.min.css
  char* css_buff = read_file("./ui/blueprint/index.css");
  char* css_fw_buff = read_file("./ui/blueprint/pico.min.css");
  if (!css_buff || !css_fw_buff) return -1; //Error handling
  html_dump += HEAD;
  html_dump += css_fw_buff;
  html_dump += '\n';
  html_dump += css_buff;
  html_dump += '\n';
  html_dump += STYLE;
  html_dump += "<center>\n";
  //Iteration for the Body section
  for (unsigned long i=0; i<CONFIG->sections.size; i++) {
    char h1_buff[strlen(CONFIG->sections.names[i])+18]; //18 -> HTML snippet's length 
    sprintf(h1_buff, "<h1><u>%s</u></h1>\n", CONFIG->sections.names[i]);
    html_dump += h1_buff;
    html_dump += "<div>\n";
    if (CONFIG->get_all_keys(CONFIG->sections.names[i])==-1) {
      printf("[ERR]: Failed to get all keys of section \"%s\"!\n", CONFIG->sections.names[i]);
    }
    for (unsigned long j=0; j<CONFIG->keys.size; j++) {
      char btn_buff[strlen(CONFIG->keys.names[j])+20+28]; //(10*2) -> unsigned long for i an j | 28 -> HTML snippet's length
      sprintf(btn_buff, "<button id=\"s%lu-k%lu\">%s</button>\n", i, j, CONFIG->keys.names[j]);
      html_dump += btn_buff;
    }
    html_dump += "</div>\n";
  }
  html_dump += "</center>\n";
  html_dump += SCRIPT;
  String js_buff_mod = "let port = ";
  char* PORT_str = new char[6]; //5+1 -> unsigned + \0
  sprintf(PORT_str, "%d", PORT);
  js_buff_mod += PORT_str;
  js_buff_mod += ";\n";
  delete[] PORT_str;
  js_buff_mod += "let socket = new WebSocket(`ws://${window.location.hostname}:${port}`, \"KomiX\");\n";
  //Iteration for the Script section
  for (unsigned long i=0; i<CONFIG->sections.size; i++) {
    if (CONFIG->get_all_keys(CONFIG->sections.names[i])==-1) {
      printf("[ERR]: Failed to get all keys of section \"%s\"!\n", CONFIG->sections.names[i]);
    }
    for (unsigned long j=0; j<CONFIG->keys.size; j++) {
      char* temp_sprintf_buff = new char[66+80]; //(20*4) -> unsigned long 4 times | 66 -> JS snippet's length
      sprintf(temp_sprintf_buff, "document.getElementById(\"s%lu-k%lu\").onclick = () => socket.send(\"%lu-%lu\");\n", i, j, i, j);
      js_buff_mod += temp_sprintf_buff;
      delete[] temp_sprintf_buff;
    }
  }
  html_dump += js_buff_mod;
  html_dump += FOOTER;
  delete[] css_buff;
  delete[] css_fw_buff;
  if (write_file("./ui/index.html", "w", html_dump.toCharArray(), html_dump.length())==-1) {
    printf("[ERR]: Failed to write to \"./ui/index.html\"!\n");
    return -1;
  }
  return 0;
}