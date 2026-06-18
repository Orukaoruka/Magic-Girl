#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include "funciones.h"
#include <stdio.h>



void dibujar_mapa(mapa mapa[FIL][COL], player player)
{
  int i,j;
  ALLEGRO_COLOR RIGHT_UI = al_map_rgb(0,0,0);

  al_clear_to_color(al_map_rgb(153, 193, 241));    
  for(i = 0; i < FIL; i++)
  {
    for(j = 0; j < COL; j++)
    {
      if(mapa[i][j].mapa_char == 'm')
      {
        al_draw_filled_rectangle(j*PIXEL, i*PIXEL, (j+1)*PIXEL, (i+1)*PIXEL,RIGHT_UI);
      }
    }
  }
   //Cuadrado
  al_draw_filled_rectangle(player.jposicion_player*PIXEL, player.iposicion_player*PIXEL, (player.jposicion_player+1)*PIXEL, (player.iposicion_player+1)*PIXEL, al_map_rgb(200, 0, 198)); 

  return;
}

int menu(ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_EVENT event, ALLEGRO_BITMAP *background, ALLEGRO_FONT *font)
{
  int select_play = 1, select_quit = 0, done = 0, redraw = 1;

  while(1)
  {
    al_wait_for_event(queue, &event);
    switch(event.type)
    {
      case ALLEGRO_EVENT_KEY_CHAR:
        if(event.keyboard.keycode == ALLEGRO_KEY_UP){
          select_play = 1;
          select_quit = 0;
          redraw = 1;
          break;
        }
        if(event.keyboard.keycode == ALLEGRO_KEY_DOWN){
          select_quit = 1;
          select_play = 0;
          redraw = 1;
          break;
        }

        if(event.keyboard.keycode == ALLEGRO_KEY_ENTER){
          done = 1;
          break;
        }
        break;

      case ALLEGRO_EVENT_DISPLAY_CLOSE:
        done = 0;
        break;
    }

    //if done == true
    if(done == 1)
      break;

    if(redraw == 1){
      dibujar_menu(background, select_play, select_quit, font);
      al_flip_display();
      redraw = 0;
    }
  }
  return select_quit;
}

void dibujar_menu(ALLEGRO_BITMAP *background, int select_play, int select_quit, ALLEGRO_FONT *font) 
{
  ALLEGRO_COLOR color_default = al_map_rgb(255,255,255);
  ALLEGRO_COLOR color_selected = al_map_rgb(255,0,0);

  al_clear_to_color(al_map_rgb(0,0,0));
  al_draw_bitmap(background, 100, 100, 0);
      
  if(select_play == 1){     
    al_draw_text(font, color_selected, 903,464,0, "-> JUGAR");
    al_draw_text(font,color_default, 903,585,0,"SALIR");
  }

  if(select_quit == 1){
    al_draw_text(font, color_default, 903,464,0, "JUGAR");
    al_draw_text(font,color_selected, 903,585,0,"-> SALIR"); 
  }
  return;
}

int movimiento_jugador(mapa mapa[FIL][COL],player *player,ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_EVENT event, ALLEGRO_KEYBOARD_STATE ks,int redraw)
{
  //Espera por un evento que este localizado dentro de la cola (prioridad en orden de llegada)
  al_wait_for_event(queue, &event);

  //event es un tipo de estructura de allegro. != para colisiones (temporalmente)
  switch(event.type)
  {
    case ALLEGRO_EVENT_TIMER:
    
      al_get_keyboard_state(&ks);
      //Arreglar aqi movimiento

          if(al_key_down(&ks, ALLEGRO_KEY_UP) && mapa[(player->iposicion_player)-1][(player->jposicion_player)].mapa_binario != 1){
            player->iposicion_player--;
          }
 
          if(al_key_down(&ks, ALLEGRO_KEY_DOWN) && mapa[(player->iposicion_player)+1][(player->jposicion_player)].mapa_binario != 1){
            player->iposicion_player++;

          }

          if(al_key_down(&ks, ALLEGRO_KEY_LEFT) && mapa[player->iposicion_player][(player->jposicion_player-1)].mapa_binario != 1){

            player->jposicion_player--;
          }
      /*ARREGLARRR no va pal lado y estoy cansada waah*/
          if(al_key_down(&ks, ALLEGRO_KEY_RIGHT) && mapa[(player->iposicion_player)][(player->jposicion_player)+1].mapa_binario != 1){

            player->jposicion_player++;
          }
          if(al_key_down(&ks, ALLEGRO_KEY_ESCAPE)){
            redraw = 2;
            return redraw;
          }

      redraw = 1;
      break;

      case ALLEGRO_EVENT_DISPLAY_CLOSE:
        redraw = 2;
        break;
   }
   
    
  return redraw;
}

void shutdown_game(ALLEGRO_DISPLAY *disp, ALLEGRO_TIMER *timer, ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_FONT *font, ALLEGRO_BITMAP *background)
{
  al_destroy_display(disp);
  al_destroy_timer(timer);
  al_destroy_event_queue(queue);
  al_destroy_font(font);
  al_destroy_bitmap(background);
  //Libera todo
  return;
}

void leer_mapa(mapa mapa[FIL][COL], player *player)
{
  printf("se lee mapa");
  FILE *fan;
  int i,j;
  char aux;
  fan = fopen("map.txt","r");
  
  for(i = 0; i < FIL; i++)
  {
    for(j = 0; j < COL; j++)
    {
      fscanf(fan,"%c",&mapa[i][j].mapa_char);
    }
    fscanf(fan,"%c",&aux);
  }

  for(i = 0; i < FIL; i++)
  {
    for(j = 0; j < COL; j++)
    {
      if(mapa[i][j].mapa_char == '.')
      {
        mapa[i][j].mapa_binario = 0;
      }

      else
      {
        mapa[i][j].mapa_binario = 1;
      }

      if(mapa[i][j].mapa_char == 'p')
      {
        player->iposicion_player = i;
        player->jposicion_player = j;
      }
    }
  }
  return;
}