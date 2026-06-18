#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include "funciones.h"



int main()
{
  
  al_init();
  al_install_keyboard();
  al_init_primitives_addon();
  al_init_font_addon();
  al_init_ttf_addon();
  al_init_image_addon();

  int redraw = 1;
  mapa mapa[FIL][COL];
  player player;

  //FPS
  ALLEGRO_TIMER *timer = al_create_timer(1.0 / 45.0);
  //Cola de eventos
  ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
  //Imagen del menu
  ALLEGRO_BITMAP *background = al_load_bitmap("HasturPlushie.jpeg");
  //Font
  ALLEGRO_FONT *font = al_load_ttf_font("BoldPixels.ttf",48,0);
  //Variable tipo evento
  ALLEGRO_EVENT event;
  ALLEGRO_KEYBOARD_STATE ks;

  //Le dice a allegro el ancho y largo de la ventana
  ALLEGRO_DISPLAY *disp = al_create_display(DISPLAY_WEIGHT, DISPLAY_HEIGHT);
  
  // Eventos
  al_register_event_source(queue, al_get_keyboard_event_source());
  al_register_event_source(queue, al_get_display_event_source(disp));
  al_register_event_source(queue, al_get_timer_event_source(timer));
  al_start_timer(timer);

  //ciclo del MENU

  if(1 == menu(queue,event,background,font)){
    shutdown_game(disp,timer,queue,font,background);
    exit(0);
  }

  leer_mapa(mapa,&player);
  
  //Ciclo juego main
  while(1)
  {  
    //Redraw return 0,1 o 2. Funcion de movimiento jugador
    redraw = movimiento_jugador(mapa,&player,queue,event,ks,redraw);
    //Función redibujo.
    
    if(redraw == 1 && al_is_event_queue_empty(queue)){
      dibujar_mapa(mapa,player);   
      al_flip_display();
      redraw = 0;
    }

    //Si retorna 2 es porque el usuario presionó escape o cerró la ventana
    if(redraw == 2){
      break;
    }
  }
  

  // Fin del programa
  shutdown_game(disp,timer,queue,font,background);
  
  return 0;
}