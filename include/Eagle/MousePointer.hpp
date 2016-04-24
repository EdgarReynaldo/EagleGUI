
/*
 *
 *     _______       ___       ____      __       _______
 *    /\  ____\    /|   \     /  __\    /\ \     /\  ____\
 *    \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_
 *     \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\
 *      \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_
 *       \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *        \/______/|/_/  \/_/  \_\_____/    \/_____/ \/______/
 *
 *
 *    EAGLE
 *    Edgar's Agile Gui Library and Extensions
 *
 *    Copyright 2009-2014+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */


#ifndef MousePointer_HPP
#define MousePointer_HPP


#include <list>

/*
ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT x
ALLEGRO_SYSTEM_MOUSE_CURSOR_ARROW x
ALLEGRO_SYSTEM_MOUSE_CURSOR_BUSY x
ALLEGRO_SYSTEM_MOUSE_CURSOR_QUESTION x
ALLEGRO_SYSTEM_MOUSE_CURSOR_EDIT x
ALLEGRO_SYSTEM_MOUSE_CURSOR_MOVE x
ALLEGRO_SYSTEM_MOUSE_CURSOR_RESIZE_N x
ALLEGRO_SYSTEM_MOUSE_CURSOR_RESIZE_W x
ALLEGRO_SYSTEM_MOUSE_CURSOR_RESIZE_S x
ALLEGRO_SYSTEM_MOUSE_CURSOR_RESIZE_E x
ALLEGRO_SYSTEM_MOUSE_CURSOR_RESIZE_NW x
ALLEGRO_SYSTEM_MOUSE_CURSOR_RESIZE_SW x
ALLEGRO_SYSTEM_MOUSE_CURSOR_RESIZE_SE x
ALLEGRO_SYSTEM_MOUSE_CURSOR_RESIZE_NE x
ALLEGRO_SYSTEM_MOUSE_CURSOR_PROGRESS 
ALLEGRO_SYSTEM_MOUSE_CURSOR_PRECISION 
ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK 
ALLEGRO_SYSTEM_MOUSE_CURSOR_ALT_SELECT 
ALLEGRO_SYSTEM_MOUSE_CURSOR_UNAVAILABLE 
*/

enum MOUSE_POINTER_TYPE {
   POINTER_NORMAL   = 0,
   POINTER_ARROW    = 1,
   POINTER_BUSY     = 2,
   POINTER_QUESTION = 3,
   POINTER_EDIT     = 4,
   POINTER_GRAB     = 5,
   POINTER_GRABBED  = 6,

   POINTER_NWSE     = 7,
   POINTER_NW       = 7,
   POINTER_SE       = 7,

   POINTER_NS       = 8,
   POINTER_N        = 8,
   POINTER_S        = 8,

   POINTER_NESW     = 9,
   POINTER_NE       = 9,
   POINTER_SW       = 9,

   POINTER_EW       = 10,
   POINTER_E        = 10,
   POINTER_W        = 10,

   POINTER_CUSTOM   = 11,
   NUM_POINTERS     = 12
};


extern const char* pointer_names[NUM_POINTERS];



class EagleImage;


class MousePointerInfo {
public :

   MousePointerInfo(MOUSE_POINTER_TYPE ptype , EagleImage* img , int fx , int fy , bool is_system_pointer = false);

   MOUSE_POINTER_TYPE pointer_type;
   bool system_pointer_in_use;
   
   EagleImage* image;
   
   int alignx;
   int aligny;
   
   MousePointerInfo();
   
//   ~virtual MousePointerInfo() {}
   
   
};



class MousePointer {
protected :
   MousePointerInfo mpinfo;
   
public :
   MousePointer();
   virtual ~MousePointer() {}
   
   MousePointerInfo GetInfo();

   
   virtual bool Create(MousePointerInfo info)=0;
   virtual void Free()=0;
   virtual bool Valid()=0;
   
};

class SystemMousePointer : public MousePointer {
   
   
public :
   SystemMousePointer(MOUSE_POINTER_TYPE type);   
   
   virtual bool Create(MousePointerInfo info);
   virtual void Free();
   virtual bool Valid();
   
};



class MousePointerSet {
private :
   MousePointer* pointer_set[NUM_POINTERS];
   
public :
   MousePointerSet();
   ~MousePointerSet();
   void Free();

   MousePointer*& operator[](MOUSE_POINTER_TYPE type);// not guaranteed to return non-null, check

};



class EagleGraphicsContext;

// abstract class
class MousePointerManager {
   
protected :

   EagleGraphicsContext* display;
   
   MousePointerSet custom_pointer_set;
   MousePointerSet system_pointer_set;
   
   MousePointerSet* pointer_set_in_use;
   
   MousePointer** current_pointer;// need the address of the pointer in the pointer set array to make changes
   

   std::list< std::pair<void* , MousePointer**> > mouse_owners;



   void SetupSystemPointers();



   MousePointerManager(EagleGraphicsContext* window);
   
   

   virtual MousePointer* CreateMousePointer(MousePointerInfo info)=0;
///   virtual MousePointer** SelectMousePointer(MOUSE_POINTER_TYPE type , bool use_system_pointer)=0;
   virtual bool SelectMousePointerDirect(MousePointer** mp)=0;


public :
   
   virtual ~MousePointerManager();

   void FreePointers();

   
   virtual void ShowMouse()=0;
   virtual void HideMouse()=0;
   
   bool AcquireMousePointer(void* caller , MOUSE_POINTER_TYPE ptype , bool use_system_pointer);
   void ReleaseMousePointer(const void* caller);
   void ReAcquireMouse();// used when a window regains focus, to refresh the mouse, not called automatically
   
   
   bool SetCustomPointer(MousePointerInfo info);
//   void SetCustomPointerSet(MousePointerSet* pointer_set);
   
   virtual void SetMousePosition(int mousex , int mousey)=0;
   
   
   
};



#endif // MousePointer_HPP







