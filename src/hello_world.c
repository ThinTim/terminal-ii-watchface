#include <pebble.h>

#define DEGREE_SIGN "°"
#define CURSOR_SIGN "Û"

static Window *s_window;
static TextLayer *s_text_layer;

static void init(void) {
	// Create a window and get information about the window
	s_window = window_create();
  Layer *window_layer = window_get_root_layer(s_window);
  GRect bounds = layer_get_bounds(window_layer);

  // Create a text layer and set the text
	s_text_layer = text_layer_create(bounds);

	text_layer_set_text(s_text_layer, "AMSTERDAM\n" "THUNDERSTORMS\n" "18 DEGREES\n" "BATTERY 80\n" "SEPTEMBER 31\n" "WEDNESDAY\n" "2:45PM");
  text_layer_set_background_color(s_text_layer, GColorBlack);
  text_layer_set_text_color(s_text_layer, GColorGreen);
  text_layer_set_font(s_text_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_APPLE_16)));
	text_layer_set_text_alignment(s_text_layer, GTextAlignmentLeft);

	layer_add_child(window_get_root_layer(s_window), text_layer_get_layer(s_text_layer));
	window_stack_push(s_window, true);
}

static void deinit(void) {
	// Destroy the text layer
  text_layer_destroy(s_text_layer);

	// Destroy the window
	window_destroy(s_window);
}

int main(void) {
	init();
	app_event_loop();
	deinit();
}
