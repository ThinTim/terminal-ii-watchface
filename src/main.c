#include <pebble.h>
#include "terminal.h"
#include "util.h"
#include "owm_weather/owm_weather.h"
#include "secrets.h"

static Window *s_window;
static Layer *s_terminal_layer;
static GFont s_terminal_font;

static TerminalData s_terminal_data;
static TerminalText s_terminal_text;
static time_t s_weather_timestamp;

static char displayed_text_buffer[TERMINAL_BUFFER_SIZE];

static void type_next_character(void *data) {
  layer_mark_dirty(s_terminal_layer);

  if(s_terminal_text.cursor_index >= strlen(s_terminal_text.text_buffer)) {
    s_terminal_text.cursor_index = 0;
  } else {
    s_terminal_text.cursor_index += 1;
    app_timer_register(TYPING_SPEED, (AppTimerCallback)type_next_character, NULL);
  }
}

static void type_terminal_message() {
  if(s_terminal_text.cursor_index > 0) {
    //Typing already. Just need to reset the cursor.
    s_terminal_text.cursor_index = 0;
  } else {
    type_next_character(NULL);
  }
}

static void notify_data_changed() {
  format_terminal_message(&s_terminal_data, &s_terminal_text);
  type_terminal_message();
}

static void update_location_and_weather(OWMWeatherInfo *info, OWMWeatherStatus status) {
  if(status == OWMWeatherStatusAvailable) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Weather and location updated");

    strncpy(s_terminal_data.city, info->name, CITY_BUFFER_SIZE);
    snprintf(s_terminal_data.temperature, TEMPERATURE_BUFFER_SIZE, "%d DEGREES", info->temp_c);
    strncpy(s_terminal_data.forecast, info->description_short, FORECAST_BUFFER_SIZE);

    make_upper_case(s_terminal_data.city, CITY_BUFFER_SIZE);
    make_upper_case(s_terminal_data.forecast, FORECAST_BUFFER_SIZE);

    s_weather_timestamp = info->timestamp;
    notify_data_changed();
  }
}

static void tick_handler(struct tm *tick_time, TimeUnits time_changed) {
  time_t now = time(NULL);

  s_terminal_data.time = now;
  s_terminal_data.step_count = health_service_sum_today(HealthMetricStepCount);
  s_terminal_data.battery_level = battery_state_service_peek().charge_percent;

  notify_data_changed();

  if(s_weather_timestamp > 0 && (now - s_weather_timestamp) > SECONDS_PER_HOUR) {
    owm_weather_fetch(update_location_and_weather);
  }
}

static void terminal_layer_update_proc(Layer *layer, GContext *ctx) {
  if(strcmp(displayed_text_buffer, s_terminal_text.text_buffer)) {
    memset(displayed_text_buffer, 0, sizeof(displayed_text_buffer));
    strncpy(displayed_text_buffer, s_terminal_text.text_buffer, s_terminal_text.cursor_index);

    if(strcmp(displayed_text_buffer, s_terminal_text.text_buffer)) {
      strncat(displayed_text_buffer, CURSOR_SIGN, 1);
    }
  }

  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_context_set_text_color(ctx, GColorGreen);

  GRect bounds = layer_get_bounds(layer);

  graphics_fill_rect(ctx, bounds, 0, GCornerNone);
  graphics_draw_text(ctx, displayed_text_buffer, s_terminal_font, bounds, GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
}

static void on_js_ready() {
  owm_weather_init_with_base_app_key(OWM_API_KEY, 10); //OWM_API_KEY stored in secrets.h
  owm_weather_fetch(update_location_and_weather);
}

static void init(void) {
  //Load font
  s_terminal_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_APPLE_16));

  //Set up main layer
	s_window = window_create();
  Layer *window_layer = window_get_root_layer(s_window);
  GRect bounds = layer_get_bounds(window_layer);
  s_terminal_layer = layer_create(bounds);
  layer_set_update_proc(s_terminal_layer, terminal_layer_update_proc);
	layer_add_child(window_layer, s_terminal_layer);

  s_weather_timestamp = 0;
  s_terminal_text.cursor_index = 0;
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  wait_for_js_ready(on_js_ready);

  //Show the main window
  window_stack_push(s_window, true);
}

static void deinit(void) {
  owm_weather_deinit();
  tick_timer_service_unsubscribe();
  fonts_unload_custom_font(s_terminal_font);
  layer_destroy(s_terminal_layer);
	window_destroy(s_window);
}

int main(void) {
	init();
	app_event_loop();
	deinit();
}
