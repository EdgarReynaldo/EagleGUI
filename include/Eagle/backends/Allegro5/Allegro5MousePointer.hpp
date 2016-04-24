

#ifndef Allegro5MousePointer_HPP
#define Allegro5MousePointer_HPP


#include "Eagle/MousePointer.hpp"



struct ALLEGRO_MOUSE_CURSOR;

class Allegro5MousePointer : public MousePointer {
private :
   
   ALLEGRO_MOUSE_CURSOR* a5mpointer;
   
public :
   Allegro5MousePointer();
   Allegro5MousePointer(MousePointerInfo info);

   virtual ~Allegro5MousePointer();

   virtual void Free();
   virtual bool Valid();
   
   ALLEGRO_MOUSE_CURSOR* GetAllegroMousePointer();
   
   virtual bool Create(MousePointerInfo info);
//   virtual void Free();
//   virtual bool Valid();

   
};





class Allegro5GraphicsContext;
struct ALLEGRO_DISPLAY;

class Allegro5MousePointerManager : public MousePointerManager {

protected :
   Allegro5GraphicsContext* a5win;
   ALLEGRO_DISPLAY* a5display;
   

   
   bool SelectSystemMousePointer(MOUSE_POINTER_TYPE ptype);
   bool SelectCustomMousePointer(Allegro5MousePointer* a5mp);

   virtual bool SelectMousePointerDirect(MousePointer** mp);

   virtual MousePointer* CreateMousePointer(MousePointerInfo info);
   
public :

   Allegro5MousePointerManager(EagleGraphicsContext* window);

   virtual void ShowMouse();
   virtual void HideMouse();
   
   virtual void SetMousePosition(int mousex , int mousey);

};







#endif // Allegro5MousePointer_HPP













