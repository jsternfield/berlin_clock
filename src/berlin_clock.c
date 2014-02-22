#include <pebble.h>

#define SCREEN_HEIGHT 168
#define SCREEN_WIDTH  144

static Window *window;

static GBitmap *big_light,   *big_medium,   *big_dark,
               *small_light, *small_medium, *small_dark,
               *circle_light, *circle_dark;


static BitmapLayer *circle_layer,
                   *hour5_layers[4],
		   *hour_layers[4],
		   *minute5_layers[11],
		   *minute_layers[4];

static void load_bitmaps() {
  big_light    = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BIG_LIGHT);
  big_medium   = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BIG_MEDIUM);
  big_dark     = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BIG_DARK);
  small_light  = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_SMALL_LIGHT);
  small_medium = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_SMALL_MEDIUM);
  small_dark   = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_SMALL_DARK);
  circle_light = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_CIRCLE_LIGHT);
  circle_dark  = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_CIRCLE_DARK);
}

static void unload_bitmaps() {
  gbitmap_destroy(big_light);
  gbitmap_destroy(big_medium);
  gbitmap_destroy(big_dark);
  gbitmap_destroy(small_light);
  gbitmap_destroy(small_medium);
  gbitmap_destroy(small_dark);
  gbitmap_destroy(circle_light);
  gbitmap_destroy(circle_dark);
}

static void define_layers(Window *window) {
  Layer *window_layer = window_get_root_layer(window);

  /* circle layer */
  GRect frame = (GRect) {
    .origin = { (SCREEN_WIDTH/2)-15, (SCREEN_HEIGHT/6)-15 },
    .size = { 30, 30 }
  };
  circle_layer = bitmap_layer_create(frame);
  layer_add_child(window_layer, bitmap_layer_get_layer(circle_layer));

  for (int row = 2; row < 6; row++) {
    int num_columns, column_width, column_spacing;

    if ( row == 4 ) {
      num_columns = 11;
      column_width = 8;
      column_spacing = 5;
    } else {
      num_columns = 4;
      column_width = 30;
      column_spacing = 6;
    }

    for (int column = 0; column < num_columns; column++) {
      frame.origin.x = (SCREEN_WIDTH/2)  /* center */
                        - (num_columns*column_width + (num_columns-1)*column_spacing)/2 /* left offset */
		        + column*(column_width+column_spacing); /* column offset */
      frame.origin.y = (SCREEN_HEIGHT/6)*row;
      frame.size.w   = column_width;
      frame.size.h   = 20;

      BitmapLayer *tmp_layer = bitmap_layer_create(frame); 
      layer_add_child(window_layer, bitmap_layer_get_layer(tmp_layer));
      switch (row) {
	case 2:
	  hour5_layers[column] = tmp_layer;
	  break;
	case 3:
	  hour_layers[column] = tmp_layer;
	  break;
	case 4:
	  minute5_layers[column] = tmp_layer;
	  break;
	case 5:
	  minute_layers[column] = tmp_layer;
	  break;
	default:
	  // LOG ERROR MESSAGE
          break;
      }
    }
  }
}
  
static void undefine_layers() {
  bitmap_layer_destroy(circle_layer);

  int i;
  for ( i = 0; i < 4; i++ ) {
    bitmap_layer_destroy(hour5_layers[i]);
    bitmap_layer_destroy(hour_layers[i]);
    bitmap_layer_destroy(minute5_layers[i]);
    bitmap_layer_destroy(minute_layers[i]);
  }
  for ( /* no init */; i < 11; i++ ) {
    bitmap_layer_destroy(minute5_layers[i]);
  }
}

static void draw_time(struct tm *tick_time) {
  int i, num_marks;

  /* 5 hour marks */
  num_marks = tick_time->tm_hour/5;
  for ( i = 0; i < num_marks; i++ ) {
    bitmap_layer_set_bitmap(hour5_layers[i], big_medium);
  }
  for ( /* no init */; i < 4; i++ ) {
    bitmap_layer_set_bitmap(hour5_layers[i], big_dark);
  }

  /* hour marks */
  num_marks = tick_time->tm_hour%5;
  for ( i = 0; i < num_marks; i++ ) {
    bitmap_layer_set_bitmap(hour_layers[i], big_medium);
  }
  for ( /* no init */; i < 4; i++ ) {
    bitmap_layer_set_bitmap(hour_layers[i], big_dark);
  }

  /* 5 minute marks */
  num_marks = tick_time->tm_min/5;
  for ( i = 0; i < num_marks; i++ ) {
    if ( (i%3) == 2 ) { 
      /* 15 minute markers */ 
      bitmap_layer_set_bitmap(minute5_layers[i], small_medium);
    } else {
      bitmap_layer_set_bitmap(minute5_layers[i], small_light);
    }
  }
  for ( /* no init */; i < 11; i++ ) {
    bitmap_layer_set_bitmap(minute5_layers[i], small_dark);
  }

  /* minute marks */
  num_marks = tick_time->tm_min%5;
  for ( i = 0; i < num_marks; i++ ) {
    bitmap_layer_set_bitmap(minute_layers[i], big_light);
  }
  for ( /* no init */; i < 4; i++ ) {
    bitmap_layer_set_bitmap(minute_layers[i], big_dark);
  }
}
    

static void handle_second_tick(struct tm *tick_time, TimeUnits units_changed) {
  static int last_min = 70; /* 70 is impossible, therefore will trigger reset on first display */

  /* Flip the circle color once a second */
  if ((tick_time->tm_sec % 2) == 0) {
    bitmap_layer_set_bitmap(circle_layer, circle_light);
  } else {
    bitmap_layer_set_bitmap(circle_layer, circle_dark);
  }
 
  if ( tick_time->tm_min != last_min ) {
    last_min = tick_time->tm_min;
    draw_time(tick_time);
  }
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  load_bitmaps();
  define_layers(window);

  time_t now = time(NULL);
  struct tm *tick_time = localtime(&now);
  handle_second_tick(tick_time, (TimeUnits) 0);
}

static void window_unload(Window *window) {
  undefine_layers();
  unload_bitmaps();
}

static void init(void) {
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  window_set_background_color(window, GColorBlack);

  const bool animated = true;
  window_stack_push(window, animated);

  tick_timer_service_subscribe(SECOND_UNIT, handle_second_tick);
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

  app_event_loop();
  deinit();
}
