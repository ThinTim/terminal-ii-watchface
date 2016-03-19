#pragma once
#include <pebble.h>

typedef enum {
  AppKeyJSReady = 1
} AppKey;

typedef void (*JsReadyCallback)();

char* make_upper_case(char *str, unsigned int len);
char* format_date(char* buffer, unsigned int len, struct tm* time);
void wait_for_js_ready(JsReadyCallback callback);
