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
