#include <pebble.h>

Window *my_window;
TextLayer *text_layer;
static int SwitchButton = 0;
AppTimer *timer;
static int myTime = 1000;
static int numberClick = 0;
static int timeSig = 4;
void timer_handle(void){
  static char textSig[200] = "";
  snprintf( textSig, sizeof(textSig) ,"Starting %d \n %d ms",numberClick, myTime );
  text_layer_set_text(text_layer, textSig);
  numberClick = numberClick+1;
  if(numberClick == timeSig+1){
    numberClick = 1;
    vibes_double_pulse();
  }else{
    vibes_short_pulse();
  }
  
  timer =  app_timer_register(myTime, (AppTimerCallback) timer_handle, NULL);
};
void update_text(void){
  if(SwitchButton == 1){
     static char textSig[200] = "";
    snprintf( textSig, sizeof(textSig) ,"Starting %d \n %d ms",numberClick, myTime );
    text_layer_set_text(text_layer, textSig);
  }else{
    static char textSig[200] = "";
    snprintf( textSig, sizeof(textSig) ,"Stopped %d:4",timeSig );
    text_layer_set_text(text_layer, textSig);
  }
}
void select_click_handler(ClickRecognizerRef recognizer, void *context)
{
  if (SwitchButton == 1){
    static char textSig[200] = "";
    snprintf( textSig, sizeof(textSig) ,"Stopped %d:4",timeSig );
    text_layer_set_text(text_layer, textSig);
    app_timer_cancel(timer);
  } else {
    timer =  app_timer_register(myTime, (AppTimerCallback) timer_handle, NULL);
     text_layer_set_text(text_layer, "Starting 1 \n 1000 ms");
    numberClick = 2;
    vibes_short_pulse();
  }
  SwitchButton = ( SwitchButton + 1)%2;
}
 
void down_click_handler(ClickRecognizerRef recognizer, void *context)
{
  if(SwitchButton == 0){
    if(timeSig > 2){
      timeSig = timeSig-1;
      update_text();
    }
  }else{
   if(myTime != 1000){
     myTime = myTime+ 100;
     update_text();
   } 
  }
}
 
void up_click_handler(ClickRecognizerRef recognizer, void *context)
{
  if(SwitchButton == 0){
    if(timeSig < 20){
      timeSig = timeSig+1;
      update_text();
    }
  }else{
    if(myTime != 100){
     myTime = myTime- 100;
      update_text();
   } 
  }
}
void click_config_provider(void *context)
{
    window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
    window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
    window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
}
void handle_init(void) {
  my_window = window_create();
  window_set_click_config_provider(my_window, click_config_provider);
  window_stack_push(my_window, true);
  GRect bounds = layer_get_bounds(window_get_root_layer(my_window));
  text_layer = text_layer_create(GRect(0, PBL_IF_ROUND_ELSE(58, 52), bounds.size.w, 50));
  text_layer_set_background_color(text_layer, GColorClear);
  text_layer_set_text_color(text_layer, GColorBlack);
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD) );
  layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(text_layer));
  update_text();
}

void handle_deinit(void) {
  text_layer_destroy(text_layer);
  window_destroy(my_window);
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
