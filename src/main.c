#include <pebble.h>

#define DEGREE_SIGN "°"
#define CURSOR_SIGN "Û"

static Window *s_window;
static Layer *s_terminal_layer;
static GFont s_terminal_font;

static void terminal_layer_update_proc(Layer *layer, GContext *ctx) {
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_context_set_text_color(ctx, GColorGreen);

  char *text = "AMSTERDAM\n" "18 DEGREES\n" "THUNDERSTORMS\n" "SEPTEMBER 31\n" "WEDNESDAY\n" "UPTIME 2H45M\n" "BATTERY 80\n" "5500 STEPS\n" "2:45PM" CURSOR_SIGN;

  GRect bounds = layer_get_bounds(layer);

  graphics_fill_rect(ctx, bounds, 0, GCornerNone);
  graphics_draw_text(ctx, text, s_terminal_font, bounds, GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
}

static void init(void) {
	s_window = window_create();
  Layer *window_layer = window_get_root_layer(s_window);
  GRect bounds = layer_get_bounds(window_layer);

  s_terminal_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_APPLE_16));

	s_terminal_layer = layer_create(bounds);
  layer_set_update_proc(s_terminal_layer, terminal_layer_update_proc);

	layer_add_child(window_layer, s_terminal_layer);
	window_stack_push(s_window, true);
}

static void deinit(void) {
  fonts_unload_custom_font(s_terminal_font);

  layer_destroy(s_terminal_layer);

	window_destroy(s_window);
}

int main(void) {
	init();
	app_event_loop();
	deinit();
}
