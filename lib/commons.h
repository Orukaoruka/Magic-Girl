#ifndef COMMONS_H
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>
#define COMMONS_H

#define DISPLAY_WEIGHT 1280
#define DISPLAY_HEIGHT 720
#define COL 40
#define FIL 23
#define PIXEL 32 
#define BALAS 1 // holder
#define PASO 3
#define ALTO_UI 100 

struct disparo_player{
  int posicion_x;
  int posicion_y;
  bool activa;
};
typedef struct disparo_player disparo_player;

struct player{
  int iposicion_player;
  int jposicion_player;
  int life_player;
  int bullet_cont;
  disparo_player bullet[BALAS];
};
typedef struct player player;

struct mapa{
  char mapa_char;
  int mapa_binario;
};
typedef struct mapa mapa;




int menu(ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_EVENT event, ALLEGRO_BITMAP *background, ALLEGRO_FONT *font);
void dibujar_menu(ALLEGRO_BITMAP *background, int select_play, int select_quit, ALLEGRO_FONT *font);
void dibujar_mapa(ALLEGRO_BITMAP *heart_ui, ALLEGRO_BITMAP *player_sprite, mapa mapa[FIL][COL], player player);
void leer_mapa(mapa mapa[FIL][COL], player *player);
int colision(int ax, int ay, int bx, int by);
void shutdown_game(ALLEGRO_DISPLAY *disp, ALLEGRO_TIMER *timer, ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_FONT *font, ALLEGRO_BITMAP *background, ALLEGRO_BITMAP *player_sprite, ALLEGRO_BITMAP *heart_ui);
void redraw_balas_jugador(player player);
void player_shoot_move(player *player);
int movimiento_jugador(player *player, ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_EVENT event, ALLEGRO_KEYBOARD_STATE ks, int redraw);




          //HOLDER condicion donde pregunta que tan lejos estuvo la anterior bala para decidir si spawnear otra
         /* if(al_key_down(&ks, ALLEGRO_KEY_Z)) 
          {
            player->bullet_cont ++;
            //Preguntar si todas las balas se han usado para resetear el contador y su posicion
            if(player->bullet_cont == BALAS)
              player->bullet_cont = 0;

            player->bullet[player->bullet_cont].posicion_x = player->jposicion_player;
            player->bullet[player->bullet_cont].posicion_y = player->iposicion_player;
            player->bullet[player->bullet_cont].activa = true;

          }*/

#endif

/*gcc main.c src/funciones.c -I lib -o magic-girl $(pkg-config --libs --cflags allegro-5 allegro_main-5 allegro_primitives-5 allegro_font-5 allegro_ttf-5 allegro_image-5) */