#include "allegro5/allegro.h"
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_native_dialog.h"

int main(int argc, char **argv){

   ALLEGRO_DISPLAY *display = NULL;
   ALLEGRO_BITMAP  *image   = NULL;

   al_init();
   al_init_image_addon();

   display = al_create_display(800,600);

   image = al_load_bitmap("doge.jpg");

   al_draw_bitmap(image,200,200,0);

   al_draw_bitmap(image,300,200,0);
   al_flip_display();
   al_rest(2);

   al_destroy_display(display);
   al_destroy_bitmap(image);

   return 0;
}