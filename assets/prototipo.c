#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include "funciones.h"
#define KEY_SEEN 1
#define KEY_DOWN 2

int main()
{
  al_init();
  al_install_keyboard();
  al_init_primitives_addon();

  float x = 640, y = 640, x2 = 680, y2 = 680;
  int i;

  //FPS
  
  ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
  ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();

  //Le dice allegros el ancho y largo de la ventana windows
  ALLEGRO_DISPLAY* disp = al_create_display(1280, 720);
  
  // eventos
  al_register_event_source(queue, al_get_keyboard_event_source());
  al_register_event_source(queue, al_get_display_event_source(disp));
  al_register_event_source(queue, al_get_timer_event_source(timer));


  bool redraw = true;
  bool done = false;
  ALLEGRO_EVENT event;

  unsigned char key[ALLEGRO_KEY_MAX];
  memset(key, 0, sizeof(key));

  al_start_timer(timer);
  while(1)
  {
     al_wait_for_event(queue, & event);
    
     switch(event.type)
     {
        //Si el tipo de evento es de timer ps redraw se mantiene como megatru

        case ALLEGRO_EVENT_TIMER:
          if(key[ALLEGRO_KEY_UP] && y != 0){
            y-=10;
            y2-=10;
          }

          if(key[ALLEGRO_KEY_DOWN] && y2 != 720){
            y+=10;
            y2+=10;
          }

          if(key[ALLEGRO_KEY_LEFT] && x != 320){
            x-=10;
            x2-=10; 
          }

          if(key[ALLEGRO_KEY_RIGHT] && x2 != 960){
            x2+=10;
            x+=10;
          }

          for(int i = 0; i < ALLEGRO_KEY_MAX; i++)
                    key[i] &= ~KEY_SEEN;
          redraw = true;
          break;


        case ALLEGRO_EVENT_KEY_DOWN:
           key[event.keyboard.keycode] = KEY_SEEN | KEY_DOWN;
           break; 
           
        case ALLEGRO_EVENT_KEY_UP:
          key[event.keyboard.keycode] &= ~KEY_DOWN;
          break;

        case ALLEGRO_EVENT_DISPLAY_CLOSE:
          done = true;
          break;
     }
     if(done)
       break;

      //Indica que si redraw y el evento de cola se encuentran vacios, entonces, se mantiene todo papuigual
    if(redraw && al_is_event_queue_empty(queue))
    {
      al_clear_to_color(al_map_rgb(153, 193, 241));
      al_draw_filled_rectangle(0,0,320,720, al_map_rgb(0,0,0));      
      al_draw_filled_rectangle(960,0,1280,720, al_map_rgb(0,0,0));
      al_draw_filled_rectangle(x,y,x2,y2, al_map_rgb(200, 0, 198));
      al_flip_display();
      
      redraw = false;
    }
  }

    
  

  //Fin del programa
  al_destroy_display(disp);
  al_destroy_timer(timer);
  al_destroy_event_queue(queue);
  return 0;
}

// gcc prototipo.c -o 'magic-girl' $(pkg-config --libs --cflags allegro-5 allegro_main-5 allegro_primitives-5)