#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
 
const float FPS = 60;
const int SCREEN_W = 640;
const int SCREEN_H = 480;
const int doge_SIZE = 500;

int main(int argc, char **argv){
   ALLEGRO_DISPLAY *display = NULL;
   ALLEGRO_EVENT_QUEUE *event_queue = NULL;
   ALLEGRO_TIMER *timer = NULL;
   ALLEGRO_BITMAP *doge = NULL;
   float doge_x = SCREEN_W / 2.0 - doge_SIZE / 2.0;
   float doge_y = SCREEN_H / 2.0 - doge_SIZE / 2.0;
   float doge_dx = 50.0;
//   float doge_dy = 4.0;
   bool redraw = true;

   al_init();
   timer = al_create_timer(1.0 / FPS);
   display = al_create_display(SCREEN_W, SCREEN_H);
   al_init_image_addon();
   doge = al_load_bitmap("doge.jpg");
   al_set_target_bitmap(al_get_backbuffer(display));
   event_queue = al_create_event_queue();
   al_register_event_source(event_queue, al_get_display_event_source(display));
   al_register_event_source(event_queue, al_get_timer_event_source(timer));
   al_clear_to_color(al_map_rgb(0,0,0));
   al_flip_display();
   al_start_timer(timer);
 
   while(1)
   {
      ALLEGRO_EVENT ev;
      al_wait_for_event(event_queue, &ev);
 
      if(ev.type == ALLEGRO_EVENT_TIMER) {
         if( (doge_x + doge_SIZE) >= SCREEN_W ){
             doge_dx = -doge_dx;
         }

         if( (doge_x <= 0)){
             doge_dx = -doge_dx;
         }

         doge_x += doge_dx;         
         redraw = true;
      }
      else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
         break;
      }
 
      if(redraw && al_is_event_queue_empty(event_queue)) {
         redraw = false;

         al_clear_to_color(al_map_rgb(0,0,0));
         al_draw_bitmap(doge, doge_x, doge_y, 0);
         al_flip_display();
      }
   }

   al_destroy_bitmap(doge);
   al_destroy_timer(timer);
   al_destroy_display(display);
   al_destroy_event_queue(event_queue);
 
   return 0;
}