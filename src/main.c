#include <pebble.h>

static Window *s_window;
static ScrollLayer *s_scroll_layer;
static TextLayer *s_content_layer;
static ContentIndicator *s_indicator;
static Layer *s_indicator_up_layer, *s_indicator_down_layer;

static char s_content[] = "Cupcake\n\nDonut\n\nEclair\n\nFroyo\n\nGingerbread\n\nHoneycomb\n\nIce Cream Sandwich\n\nJelly Bean\n\nKitKat\n\nLollipop\n\nMarshmallow\n\n";

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  s_scroll_layer = scroll_layer_create(bounds);
  scroll_layer_set_click_config_onto_window(s_scroll_layer, window);
  scroll_layer_set_shadow_hidden(s_scroll_layer, true);
  layer_add_child(window_layer, scroll_layer_get_layer(s_scroll_layer));

  s_content_layer = text_layer_create(GRect(bounds.origin.x, bounds.origin.y, bounds.size.w, 2000));
  text_layer_set_text(s_content_layer, s_content);
  text_layer_set_text_alignment(s_content_layer, GTextAlignmentCenter);
  text_layer_set_font(s_content_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  scroll_layer_add_child(s_scroll_layer, text_layer_get_layer(s_content_layer));

  GSize text_size = text_layer_get_content_size(s_content_layer);
  layer_set_frame(text_layer_get_layer(s_content_layer), 
                  GRect(bounds.origin.x, bounds.origin.y, bounds.size.w, text_size.h));
  scroll_layer_set_content_size(s_scroll_layer, text_size);

  // Get the ContentIndicator from the ScrollLayer
  s_indicator = scroll_layer_get_content_indicator(s_scroll_layer);
  
  // Create two Layers to draw the arrows
  s_indicator_up_layer = layer_create(GRect(bounds.origin.x, bounds.origin.y, 
                                      bounds.size.w, STATUS_BAR_LAYER_HEIGHT));
  s_indicator_down_layer = layer_create(GRect(0, bounds.size.h - STATUS_BAR_LAYER_HEIGHT, 
                                        bounds.size.w, STATUS_BAR_LAYER_HEIGHT));
  layer_add_child(window_layer, s_indicator_up_layer);
  layer_add_child(window_layer, s_indicator_down_layer);

  // Configure the properties of each indicator
  const ContentIndicatorConfig up_config = (ContentIndicatorConfig) {
    .layer = s_indicator_up_layer,
    .times_out = false,
    .alignment = GAlignCenter,
    .colors = {
      .foreground = GColorBlack,
      .background = GColorWhite
    }
  };
  content_indicator_configure_direction(s_indicator, ContentIndicatorDirectionUp, 
                                        &up_config);

  const ContentIndicatorConfig down_config = (ContentIndicatorConfig) {
    .layer = s_indicator_down_layer,
    .times_out = false,
    .alignment = GAlignCenter,
    .colors = {
      .foreground = GColorBlack,
      .background = GColorWhite
    }
  };
  content_indicator_configure_direction(s_indicator, ContentIndicatorDirectionDown, 
                                        &down_config);
}

static void window_unload(Window *window) {
  scroll_layer_destroy(s_scroll_layer);
  text_layer_destroy(s_content_layer);
  layer_destroy(s_indicator_up_layer);
  layer_destroy(s_indicator_down_layer);
}

static void init() {
  s_window = window_create();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  window_stack_push(s_window, true);
}

static void deinit() {
  window_destroy(s_window);
}

int main() {
  init();
  app_event_loop();
  deinit();
}
