
#include "commons.h"

void dibujar_mapa(ALLEGRO_BITMAP *heart_ui, ALLEGRO_BITMAP *player_sprite, mapa mapa[FIL][COL], player player)
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
  al_draw_rectangle(player.jposicion_player, player.iposicion_player, player.jposicion_player+PIXEL, player.iposicion_player+PIXEL, al_map_rgb(200, 0, 198),0); 
  redraw_balas_jugador(player);
  al_draw_bitmap_region(player_sprite,0,0,36,36,player.jposicion_player, player.iposicion_player, 0);
  //health bar
  al_draw_scaled_bitmap(heart_ui,0,47,115,80,950,660,115*2.5,80*2.5,0);
  al_draw_scaled_bitmap(heart_ui,0,0,115,47,950,608,115*2.5,47*2.5,0);


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

int movimiento_jugador(player *player, ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_EVENT event, ALLEGRO_KEYBOARD_STATE ks, int redraw)
{
    al_wait_for_event(queue, &event);

    switch(event.type)
    {
        case ALLEGRO_EVENT_TIMER:
            al_get_keyboard_state(&ks);

            // --- MOVIMIENTO VERTICAL ---
            if(al_key_down(&ks, ALLEGRO_KEY_UP)) {
                player->iposicion_player -= PASO;
                // Colisión con el borde superior
                if(player->iposicion_player < 0) {
                    player->iposicion_player = 0; 
                }
            }

            if(al_key_down(&ks, ALLEGRO_KEY_DOWN)) {
                player->iposicion_player += PASO;
                // Colisión con el borde inferior
                if(player->iposicion_player > DISPLAY_HEIGHT - PIXEL) {
                    player->iposicion_player = DISPLAY_HEIGHT - PIXEL;
                }
            }

            // --- MOVIMIENTO HORIZONTAL ---
            if(al_key_down(&ks, ALLEGRO_KEY_LEFT)) {
                player->jposicion_player -= PASO;
                // Colisión con el borde izquierdo
                if(player->jposicion_player < 0) {
                    player->jposicion_player = 0;
                }
            }

            if(al_key_down(&ks, ALLEGRO_KEY_RIGHT)) {
                player->jposicion_player += PASO;
                // Colisión con el borde derecho
                if(player->jposicion_player > DISPLAY_WEIGHT - PIXEL) {
                    player->jposicion_player = DISPLAY_WEIGHT - PIXEL;
                }
            }

            if(al_key_down(&ks, ALLEGRO_KEY_ESCAPE)){
                return 2;
            }

            redraw = 1;
            break;

        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            redraw = 2;
            break;
    }
    
    return redraw;
}

void shutdown_game(ALLEGRO_DISPLAY *disp, ALLEGRO_TIMER *timer, ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_FONT *font, ALLEGRO_BITMAP *background, ALLEGRO_BITMAP *player_sprite, ALLEGRO_BITMAP *heart_ui)
{
  al_destroy_display(disp);
  al_destroy_timer(timer);
  al_destroy_event_queue(queue);
  al_destroy_font(font);
  al_destroy_bitmap(background);
  al_destroy_bitmap(player_sprite);
  al_destroy_bitmap(heart_ui);
  //Libera todo
  return;
}

void leer_mapa(mapa mapa[FIL][COL], player *player)
{
  printf("se lee mapa");
  FILE *fan;
  int i,j;
  char aux;
  fan = fopen("src/map.txt","r");
  
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
        player->iposicion_player = i*PIXEL;
        player->jposicion_player = j*PIXEL;
      }
    }
  }

  return;
}

//Cambiar nombre a playershootupdate con _
void player_shoot_move(player *player)
{
  int i;

  for ( i = 0; i < BALAS; i++)
  {
    //Si la bala sale de la pantalla o llega al ultimo pixel, entonces, ya no esta activa
    if(player->bullet[i].posicion_y <= 0)
    {
      player->bullet[i].activa = false;
    }

    if(player->bullet[i].activa == true)
    {
      player->bullet[i].posicion_y = player->bullet[i].posicion_y-PASO;
      
    }

    
  }
  return;
}

void redraw_balas_jugador(player player)
{
  int i;

  for ( i = 0; i < BALAS; i++)
  {
    if(player.bullet[i].activa == true)
    {
      al_draw_filled_rectangle(player.bullet[i].posicion_x,player.bullet[i].posicion_y,player.bullet[i].posicion_x+5,player.bullet[i].posicion_y+5,al_map_rgb(0,0,0));
    }
  }
  
}

/*void redraw_ui_heart(ALLEGRO_BITMAP *heart_ui,player *player)
{
  int
  al_draw_scaled_bitmap(heart_ui,0,47,115,80,950,660,115*2.5,80*2.5,0);
  al_draw_scaled_bitmap(heart_ui,0,0,115,47,950,608,115*2.5,47*2.5,0);

    if(player->life_player == 100)
    {

    }
    
    else if([player])
    {

    }

    else if()
    {

    }

    else if()
    {

    }
}*/