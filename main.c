#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "commons.h"

// Nota: Asegúrate de que estas constantes estén en "funciones.h". 
// Si no, puedes descomentar las siguientes líneas para pruebas rápidos:
// #define DISPLAY_WEIGHT 1024
// #define DISPLAY_HEIGHT 768
// #define ALTO_UI 100
// #define TAMANO_SPRITE 32

int main()
{
  // Inicialización segura de Allegro
  if(!al_init()) {
    printf("Error al inicializar Allegro.\n");
    return -1;
  }
  
  al_install_keyboard();
  al_init_primitives_addon();
  al_init_font_addon();
  al_init_ttf_addon();
  al_init_image_addon();

  int redraw = 1;
  player player;
  
  // Variables para controlar el efecto de scroll infinito
  float fondo_y = 0;
  float velocidad_fondo = 1.5; // Ajusta este valor para cambiar la velocidad del juego

  // FPS y Cola de eventos
  ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60.0);
  ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
  
  // Carga de recursos (Separamos el fondo del menú y el fondo del juego)
  ALLEGRO_BITMAP *menu_background = al_load_bitmap("GatoMenu.jpeg");
  ALLEGRO_BITMAP *game_background = al_load_bitmap("fondo_galaxia_gatos.png"); 
  ALLEGRO_BITMAP *player_sprite = al_load_bitmap("kitty.png");
  ALLEGRO_BITMAP *heart_ui = al_load_bitmap("assets/life_asset.png")
  
  // Fuentes para el Menú y la UI
  ALLEGRO_FONT *font = al_load_ttf_font("BoldPixels.ttf", 48, 0);
  ALLEGRO_FONT *ui_font = al_load_ttf_font("BoldPixels.ttf", 24, 0); 

  // Crear la ventana principal
  ALLEGRO_DISPLAY *disp = al_create_display(DISPLAY_WEIGHT, DISPLAY_HEIGHT);
  
  // Registro de eventos
  al_register_event_source(queue, al_get_keyboard_event_source());
  al_register_event_source(queue, al_get_display_event_source(disp));
  al_register_event_source(queue, al_get_timer_event_source(timer));
  al_start_timer(timer);

  ALLEGRO_EVENT event;
  ALLEGRO_KEYBOARD_STATE ks;

  // Ciclo del MENÚ
  if(1 == menu(queue, event, menu_background, font)){
    // Si el menú retorna 1 (SALIR), destruimos todo y cerramos antes de iniciar el juego
    shutdown_game(disp, timer, queue, font, menu_background);
    al_destroy_bitmap(game_background);
    al_destroy_bitmap(player_sprite);
    al_destroy_font(ui_font);
    exit(0);
  }

  // --- POSICIONAMIENTO INICIAL DEL JUGADOR ---
  // Reemplaza a leer_mapa(). Coloca al jugador centrado horizontalmente
  // y justo por encima de la zona de la UI en el eje vertical.
  player.jposicion_player = (DISPLAY_WEIGHT / 2) - (PIXEL / 2);
  player.iposicion_player = DISPLAY_HEIGHT - ALTO_UI - PIXEL - 50; 
  
  // Ciclo principal del juego
  while(1)
  {  
    // Llamado a la función de movimiento (sin la matriz del mapa)
    redraw = movimiento_jugador(&player, queue, event, ks, redraw);

    // Si retorna 2 es porque el usuario presionó ESC o cerró la ventana
    if(redraw == 2){
      break;
    }

    // Dibujado de elementos de juego
    if(redraw == 1 && al_is_event_queue_empty(queue)){
      
      // 1. ACTUALIZAR POSICIÓN DEL SCROLL
      // Desplazamos el fondo. Si llega al límite de la zona jugable, reinicia.
      fondo_y += velocidad_fondo;
      if(fondo_y >= (DISPLAY_HEIGHT - ALTO_UI)) {
        fondo_y = 0;
      }

      // Limpiar la pantalla completa
      al_clear_to_color(al_map_rgb(0, 0, 0));

      // 2. RENDERIZAR EL FONDO CON SCROLL (Doble dibujado para continuidad artística)
      // Primera copia en la posición actual
      al_draw_bitmap(game_background, 0, fondo_y, 0);
      // Segunda copia exactamente una pantalla arriba para rellenar el vacío
      al_draw_bitmap(game_background, 0, fondo_y - (DISPLAY_HEIGHT - ALTO_UI), 0);

      // 3. RENDERIZAR AL JUGADOR
      al_draw_bitmap(player_sprite, player.jposicion_player, player.iposicion_player, 0);
      
      // (Opcional) Puedes descomentar esta línea si quieren ver la caja de colisión real del personaje:
      // al_draw_rectangle(player.jposicion_player, player.iposicion_player, player.jposicion_player + TAMANO_SPRITE, player.iposicion_player + TAMANO_SPRITE, al_map_rgb(255, 0, 255), 1);

      // 4. RENDERIZAR LA INTERFAZ DE USUARIO (UI)
      // Dibujamos un rectángulo sólido que tape el fondo y cualquier objeto que baje de más
      al_draw_filled_rectangle(0, DISPLAY_HEIGHT - ALTO_UI, DISPLAY_WEIGHT, DISPLAY_HEIGHT, al_map_rgb(25, 25, 35));
      // Línea estética superior para separar el juego de la UI
      al_draw_line(0, DISPLAY_HEIGHT - ALTO_UI, DISPLAY_WEIGHT, DISPLAY_HEIGHT - ALTO_UI, al_map_rgb(255, 255, 255), 2);
      
      // Textos informativos de la UI
      al_draw_text(ui_font, al_map_rgb(255, 255, 255), 30, DISPLAY_HEIGHT - ALTO_UI + 35, 0, "SCORE: 00000");
      al_draw_text(ui_font, al_map_rgb(0, 255, 0), DISPLAY_WEIGHT - 220, DISPLAY_HEIGHT - ALTO_UI + 35, 0, "STAGE: 1");

      // Intercambio de buffers
      al_flip_display();
      redraw = 0;
    }
  }
  
  // Fin del programa - Liberación estricta de memoria dinámicamente reservada
  shutdown_game(disp, timer, queue, font, menu_background,player_sprite,heart_ui);
  al_destroy_bitmap(game_background);
  al_destroy_bitmap(player_sprite);
  al_destroy_font(ui_font);
  
  return 0;
}