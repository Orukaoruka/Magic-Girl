#ifndef FUNCIONES_H
#define FUNCIONES_H

#define DISPLAY_WEIGHT 1280
#define DISPLAY_HEIGHT 720
#define COL 40
#define FIL 23
#define PIXEL 32 // holder
#define BALAS 100

struct player{
  int iposicion_player;
  int jposicion_player;
  int life_player;
};
typedef struct player player;

struct mapa{
  char mapa_char;
  int mapa_binario;
};
typedef struct mapa mapa;

struct enemy{
  
};



int menu(ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_EVENT event, ALLEGRO_BITMAP *background, ALLEGRO_FONT *font);
void dibujar_menu(ALLEGRO_BITMAP *background, int select_play, int select_quit, ALLEGRO_FONT *font);
void dibujar_mapa(mapa mapa[FIL][COL], player player);
void shutdown_game(ALLEGRO_DISPLAY *disp, ALLEGRO_TIMER *timer, ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_FONT *font, ALLEGRO_BITMAP *background);
int movimiento_jugador(mapa mapa[FIL][COL],player *player,ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_EVENT event, ALLEGRO_KEYBOARD_STATE ks,int redraw);
void leer_mapa(mapa mapa[FIL][COL], player *player);


#endif

/*gcc prototipo.c assets/funciones.c -I lib -o magic-girl $(pkg-config --libs --cflags allegro-5 allegro_main-5 allegro_primitives-5 allegro_font-5 allegro_ttf-5 allegro_image-5) */