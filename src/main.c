#include <pebble.h>
#include "terminal.h"
#include "util.h"

static Window *s_window;
static Layer *s_terminal_layer;
static GFont s_terminal_font;

static TerminalData s_terminal_data;
static TerminalText s_terminal_text;

static char displayed_text_buffer[TERMINAL_BUFFER_SIZE];

static void type_next_character(void *data) {
  layer_mark_dirty(s_terminal_layer);

  if(s_terminal_text.cursor_index >= strlen(s_terminal_text.text_buffer)) {
    s_terminal_text.cursor_index = 0;
  } else {
    s_terminal_text.cursor_index += 1;
    app_timer_register(100, (AppTimerCallback)type_next_character, NULL);
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

static void terminal_layer_update_proc(Layer *layer, GContext *ctx) {
  strncpy(displayed_text_buffer, s_terminal_text.text_buffer, s_terminal_text.cursor_index);

  if(strcmp(displayed_text_buffer, s_terminal_text.text_buffer)) {
    strncat(displayed_text_buffer, CURSOR_SIGN, 1);
  }

  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_context_set_text_color(ctx, GColorGreen);

  GRect bounds = layer_get_bounds(layer);

  graphics_fill_rect(ctx, bounds, 0, GCornerNone);
  graphics_draw_text(ctx, displayed_text_buffer, s_terminal_font, bounds, GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
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

  //TODO: REMOVE - initial state setup
  s_terminal_text.cursor_index = 0;
  reset_terminal_data(&s_terminal_data);
  notify_data_changed();

  //Show the main window
  window_stack_push(s_window, true);
}

static void deinit(void) {
  animation_unschedule_all();
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
