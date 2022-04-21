



#include "allegro5/allegro.h"
#include "allegro5/allegro_native_dialog.h"


#include <cstdio>


int main(int argc , char** argv) {
   
   (void)argc;
   (void)argv;
   
   
ALLEGRO_MENU_INFO main_menu_info[] =
{
ALLEGRO_START_OF_MENU("&Archivo", 100),
{ "&Cargar Capas", 101, 0, NULL },
{ "&Cargar Elementos", 102, 0, NULL },
{NULL , (uint16_t)-1 , 0 , NULL},
{ "&Salir", 103, 0, NULL },
ALLEGRO_END_OF_MENU,

ALLEGRO_START_OF_MENU("&Configuracion", 200),
ALLEGRO_START_OF_MENU("&Velocidad de desplazamiento del raton", 201),
{ "01", 20101, 0, NULL },
{ "02", 20102, 0, NULL },
{ "03", 20103, 0, NULL },
{ "04", 20104, 0, NULL },
{ "05", 20105, 0, NULL },
{ "06", 20106, 0, NULL },
{ "07", 20107, 0, NULL },
{ "08", 20108, 0, NULL },
{ "09", 20109, 0, NULL },
{ "10", 20110, 0, NULL },
{ "11", 20111, 0, NULL },
{ "12", 20112, 0, NULL },
{ "13", 20113, 0, NULL },
{ "14", 20114, 0, NULL },
{ "15", 20115, 0, NULL },
{ "16", 20116, 0, NULL },
{ "17", 20117, 0, NULL },
{ "18", 20118, 0, NULL },
{ "19", 20119, 0, NULL },
{ "20", 20120, 0, NULL },
ALLEGRO_END_OF_MENU,

ALLEGRO_START_OF_MENU("&Velocidad de zoom del raton", 202),
{ "01", 20201, 0, NULL },
{ "02", 20202, 0, NULL },
{ "03", 20203, 0, NULL },
{ "04", 20204, 0, NULL },
{ "05", 20205, 0, NULL },
{ "06", 20206, 0, NULL },
{ "07", 20207, 0, NULL },
{ "08", 20208, 0, NULL },
{ "09", 20209, 0, NULL },
{ "10", 20210, 0, NULL },
ALLEGRO_END_OF_MENU,

{ "&Mostrar datos de la foto", 203, ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
ALLEGRO_START_OF_MENU("&Teclado Numerico", 204),
{ "Administrador", 20401, 0, NULL },
{ "Usuario 01", 20402, ALLEGRO_MENU_ITEM_DISABLED, NULL },
{ "Usuario 02", 20403, ALLEGRO_MENU_ITEM_DISABLED, NULL },
ALLEGRO_END_OF_MENU,

ALLEGRO_START_OF_MENU("&Tiempo de demora al pasar pagina", 205),
{ "1.0", 20501, 0, NULL },
{ "1.5", 20502, 0, NULL },
{ "2.0", 20503, 0, NULL },
ALLEGRO_END_OF_MENU,

{ "&Exportar a la vez que procesa las fotos", 206, ALLEGRO_MENU_ITEM_DISABLED, NULL },

{NULL , (uint16_t)-1 , 0 , NULL},
{ "&Administrador", 207, ALLEGRO_MENU_ITEM_DISABLED, NULL },
ALLEGRO_END_OF_MENU,
ALLEGRO_START_OF_MENU("&Nemesis", 300),
{ "&Iniciar", 301, 0, NULL },
ALLEGRO_END_OF_MENU,

ALLEGRO_START_OF_MENU("&Ayuda", 400),
{ "&Acerca de Nemesis", 401, 0, NULL },
ALLEGRO_END_OF_MENU,
ALLEGRO_END_OF_MENU

};


   if (!al_init()) {
      printf("Failed to init allegro.\n");
      return 1;
   }
   if (!al_init_native_dialog_addon()) {
      printf("No native dialog addon.\n");
      return 2;
   }

   ALLEGRO_DISPLAY* display = al_create_display(800,600);
   
   ALLEGRO_MENU* menu = al_build_menu(main_menu_info);
   
   al_set_display_menu(display, menu);

   ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
   
   al_register_event_source(queue , al_get_display_event_source(display));
   
   al_clear_to_color(al_map_rgb(255,255,255));
   
   al_flip_display();
   
   while(true) {
      ALLEGRO_EVENT e;
      al_wait_for_event(queue , &e);
      if (e.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
         break;
      }
   };// while (!al_is_event_queue_empty(queue));


   return 0;
}
