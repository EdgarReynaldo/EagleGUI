

#include "Eagle/backends/Allegro5/Allegro5MousePointer.hpp"

#include "Eagle/backends/Allegro5/Allegro5GraphicsContext.hpp"
#include "Eagle/backends/Allegro5/Allegro5Image.hpp"

#include "allegro5/allegro.h"


#include <map>
using std::map;



Allegro5MousePointer::Allegro5MousePointer() :
      MousePointer(),
      a5mpointer(0)
{}



Allegro5MousePointer::Allegro5MousePointer(MousePointerInfo info) :
      MousePointer(),
      a5mpointer(0)
{
   Create(info);
}



Allegro5MousePointer::~Allegro5MousePointer() {
   Free();
}



void Allegro5MousePointer::Free() {
   if (a5mpointer) {
      al_destroy_mouse_cursor(a5mpointer);
      a5mpointer = 0;
   }
}



bool Allegro5MousePointer::Valid() {
   return a5mpointer;
}



ALLEGRO_MOUSE_CURSOR* Allegro5MousePointer::GetAllegroMousePointer() {
   return a5mpointer;
}



bool Allegro5MousePointer::Create(MousePointerInfo info) {
   Free();
   Allegro5Image* a5img = dynamic_cast<Allegro5Image*>(info.image);
   ALLEGRO_BITMAP* a5bmp = a5img->AllegroBitmap();
   a5mpointer = al_create_mouse_cursor(a5bmp , info.alignx , info.aligny);
   if (a5mpointer) {
      mpinfo.MousePointerInfo::operator=(info);// assign base class members
      return true;
   }
   return false;
}



/// --------------------------------------------------------




bool Allegro5MousePointerManager::SelectSystemMousePointer(MOUSE_POINTER_TYPE ptype) {

   EAGLE_ASSERT(ptype != NUM_POINTERS);

   static map<MOUSE_POINTER_TYPE , ALLEGRO_SYSTEM_MOUSE_CURSOR> pointermap;
   static int init = 0;
   if (!init) {
      init = 1;
      pointermap[POINTER_NORMAL]   = ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT;
      pointermap[POINTER_ARROW]    = ALLEGRO_SYSTEM_MOUSE_CURSOR_ARROW;
      pointermap[POINTER_BUSY]     = ALLEGRO_SYSTEM_MOUSE_CURSOR_BUSY;
      pointermap[POINTER_QUESTION] = ALLEGRO_SYSTEM_MOUSE_CURSOR_QUESTION;
      pointermap[POINTER_EDIT]     = ALLEGRO_SYSTEM_MOUSE_CURSOR_EDIT;
      pointermap[POINTER_GRAB]     = ALLEGRO_SYSTEM_MOUSE_CURSOR_MOVE;
      pointermap[POINTER_GRABBED]  = ALLEGRO_SYSTEM_MOUSE_CURSOR_MOVE;
      pointermap[POINTER_NWSE]     = ALLEGRO_SYSTEM_MOUSE_CURSOR_RESIZE_NW;
      pointermap[POINTER_NS]       = ALLEGRO_SYSTEM_MOUSE_CURSOR_RESIZE_N;
      pointermap[POINTER_NESW]     = ALLEGRO_SYSTEM_MOUSE_CURSOR_RESIZE_NE;
      pointermap[POINTER_EW]       = ALLEGRO_SYSTEM_MOUSE_CURSOR_RESIZE_E;
      pointermap[NUM_POINTERS]     = ALLEGRO_SYSTEM_MOUSE_CURSOR_UNAVAILABLE;
   }
   
   EAGLE_ASSERT(pointermap.find(ptype) != pointermap.end());
   
   ALLEGRO_SYSTEM_MOUSE_CURSOR al_mouse = pointermap[ptype];
   
   return al_set_system_mouse_cursor(a5display , al_mouse);
}




bool Allegro5MousePointerManager::SelectCustomMousePointer(Allegro5MousePointer* a5mp) {
   EAGLE_ASSERT(a5mp);
   
   ALLEGRO_MOUSE_CURSOR* a5mouse_pointer = a5mp->GetAllegroMousePointer();
   
   EAGLE_ASSERT(a5mouse_pointer);
   
   return al_set_mouse_cursor(a5display , a5mouse_pointer);
   
}



MousePointer* Allegro5MousePointerManager::CreateMousePointer(MousePointerInfo info) {
   return new Allegro5MousePointer(info);
}



bool Allegro5MousePointerManager::SelectMousePointerDirect(MousePointer** mp) {
   
   EAGLE_ASSERT(mp);
   EAGLE_ASSERT(*mp);
   
   MousePointerInfo info = (*mp)->GetInfo();
   
   bool success = false;
   
   if (info.system_pointer_in_use) {
      success = SelectSystemMousePointer(info.pointer_type);
   }
   else {
      success = SelectCustomMousePointer(dynamic_cast<Allegro5MousePointer*>(*mp));
   }
   
   if (success) {
      current_pointer = mp;
   }
   
   return success;
}
   


Allegro5MousePointerManager::Allegro5MousePointerManager(EagleGraphicsContext* window) :
      MousePointerManager(window),
      a5win(0),
      a5display(0)
{
   a5win = dynamic_cast<Allegro5GraphicsContext*>(window);
   EAGLE_ASSERT(a5win);
   a5display = a5win->AllegroDisplay();
   EAGLE_ASSERT(a5display);
   
   // base class can't do this in the constructor (virtual call), so we do
   AcquireMousePointer(this , POINTER_NORMAL , true);
}



void Allegro5MousePointerManager::ShowMouse() {
   al_show_mouse_cursor(a5display);
}



void Allegro5MousePointerManager::HideMouse() {
   al_hide_mouse_cursor(a5display);
}



void Allegro5MousePointerManager::SetMousePosition(int mousex , int mousey) {
   al_set_mouse_xy(a5display , mousex , mousey);
}








