#include <pebble.h>
#include "util.h"
#include <ctype.h>

//Converts a string to uppercase in place
char* make_upper_case(char *str, unsigned int len) {

  char *current = str;
  while(*current != 0 && current < (str + len)) {
    *current = toupper((unsigned char)*current);
    current++;
  }

  return str;
}

//Writes a date like "November 3" into the given buffer
char* format_date(char* buffer, unsigned int len, struct tm* time) {
  char day_of_month_string[3];
  char month_string[10];

  snprintf(day_of_month_string, 3, "%u", time->tm_mday);
  strftime(month_string, 10, "%B", time);
  snprintf(buffer, len, "%s %s", month_string, day_of_month_string);

  return buffer;
}

static JsReadyCallback s_js_ready;

static void inbox_received_handler(DictionaryIterator *iter, void *context) {
  Tuple *ready_tuple = dict_find(iter, AppKeyJSReady);
  if(ready_tuple) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "JS ready message received");
    s_js_ready();
  }
}

void wait_for_js_ready(JsReadyCallback callback) {
  s_js_ready = callback;

  app_message_deregister_callbacks();
  app_message_register_inbox_received(inbox_received_handler);
  app_message_open(2026, 656);
}
