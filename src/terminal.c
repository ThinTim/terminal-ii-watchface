#include <pebble.h>
#include "terminal.h"
#include "util.h"

void format_terminal_message(TerminalData *data, TerminalText *text) {
  struct tm *local = localtime(&(data->time));

  char date_string[20];
  char day_string[20];
  char time_string[20];

  format_date(date_string, 20, local);
  strftime(day_string, 20, "%A", local);
  clock_copy_time_string(time_string, 20);

  make_upper_case(date_string, 20);
  make_upper_case(day_string, 20);
  make_upper_case(time_string, 20);

  const char * format_string = "%s\n" //City
                               "%s\n" //Temperature
                               "%s\n" //Forecast
                               "%s\n" //Date
                               "%s\n" //Day of week
                               "BATTERY %u\n" //Battery
                               "%u STEPS\n" //Step count
                               "%s"; //Time

  snprintf(text->text_buffer, TERMINAL_BUFFER_SIZE, format_string, data->city,
                                                                   data->temperature,
                                                                   data->forecast,
                                                                   date_string,
                                                                   day_string,
                                                                   data->battery_level,
                                                                   data->step_count,
                                                                   time_string);
}
