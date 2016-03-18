#include <pebble.h>

#define CURSOR_SIGN "`"
#define TERMINAL_BUFFER_SIZE 256
#define CITY_BUFFER_SIZE 50
#define FORECAST_BUFFER_SIZE 50

typedef struct {
  char city[CITY_BUFFER_SIZE];
  int temperature;
  char forecast[FORECAST_BUFFER_SIZE];
  time_t time;
  unsigned int battery_level;
  unsigned int step_count;
} TerminalData;

typedef struct {
  char text_buffer[TERMINAL_BUFFER_SIZE];
  unsigned int cursor_index;
} TerminalText;

void reset_terminal_data(TerminalData *data);
void format_terminal_message(TerminalData *data, TerminalText *text);
