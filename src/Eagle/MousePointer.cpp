
/**
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
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2019+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */



#include "Eagle/Image.hpp"
#include "Eagle/MousePointer.hpp"
#include "Eagle/GraphicsContext.hpp"

using std::list;



const char* pointer_names[NUM_POINTERS] = {
   "NORMAL",
   "ARROW",
   "BUSY",
   "QUESTION",
   "EDIT",
   "GRAB",
   "GRABBED",
   "NWSE",
   "NS",
   "NESW",
   "EW",
   "CUSTOM"
};


/// ----------------------------------------------



MousePointerInfo::MousePointerInfo() :
      pointer_type(POINTER_NORMAL),
      system_pointer_in_use(false),
      image(0),
      alignx(0),
      aligny(0)
{
   
}



MousePointerInfo::MousePointerInfo(MOUSE_POINTER_TYPE ptype , EagleImage* img , int fx , int fy , bool is_system_pointer) :
      pointer_type(ptype),
      system_pointer_in_use(is_system_pointer),
      image(img),
      alignx(fx),
      aligny(fy)
{
   if (fx == -1 || fy == -1) {
      EAGLE_ASSERT(img && img->Valid());
      if (fx == -1) {
         alignx = img->W()/2;
      }
      if (fy == -1) {
         aligny = img->H()/2;
      }
   }
}



/// ------------------------------------



MousePointer::MousePointer() :
   mpinfo()
{}



MousePointerInfo MousePointer::GetInfo() {
   return mpinfo;
}



/// -------------------------------------------



SystemMousePointer::SystemMousePointer(MOUSE_POINTER_TYPE type) :
      MousePointer()
{
   EAGLE_ASSERT(type != NUM_POINTERS);
   mpinfo.system_pointer_in_use = true;
   mpinfo.pointer_type = type;
}



bool SystemMousePointer::Create(MousePointerInfo info) {
   (void)info;
   return false;
}



void SystemMousePointer::Free() {
   return;
}



bool SystemMousePointer::Valid() {
   return false;
}



/// ---------------------------------------



MousePointerSet::MousePointerSet() :
      pointer_set()
{
   for (int i = 0 ; i < NUM_POINTERS ; ++i) {
      pointer_set[i] = 0;
   }
}
   


MousePointerSet::~MousePointerSet() {
   Free();
}



void MousePointerSet::Free() {
   for (int i = 0 ; i < NUM_POINTERS ; ++i) {
      MousePointer* mp = pointer_set[i];
      if (mp) {
         mp->Free();
         delete mp;
      }
      pointer_set[i] = 0;
   }
}
   


MousePointer*& MousePointerSet::operator[](MOUSE_POINTER_TYPE type) {
   EAGLE_ASSERT(type != NUM_POINTERS);
   return pointer_set[type];
}



/// --------------------------------------------------



void MousePointerManager::SetupSystemPointers() {
   system_pointer_set.Free();
   for (int i = 0 ; i < NUM_POINTERS ; ++i) {
      MousePointer* mp = new SystemMousePointer((MOUSE_POINTER_TYPE)i);
      system_pointer_set[(MOUSE_POINTER_TYPE)i] = mp;
   }
}



MousePointerManager::MousePointerManager(EagleGraphicsContext* window) :
      display(window),
      custom_pointer_set(),
      system_pointer_set(),
      pointer_set_in_use(&custom_pointer_set),
      current_pointer(0),
      mouse_owners()
{
   EAGLE_ASSERT(window);
   SetupSystemPointers();
   // base class can't do this in the constructor (virtual call), so we let the derived class do it
//   AcquireMousePointer(this , POINTER_NORMAL , true);
}



MousePointerManager::~MousePointerManager() {
   FreePointers();
}
   


void MousePointerManager::FreePointers() {
   custom_pointer_set.Free();
   system_pointer_set.Free();
}



bool MousePointerManager::AcquireMousePointer(void* caller , MOUSE_POINTER_TYPE ptype , bool use_system_pointer) {

   MousePointerSet* pset = pointer_set_in_use;// could be user or local custom pointer set
   if (use_system_pointer) {
      pset = &system_pointer_set;
   }
   MousePointer** new_pointer = &((*pset)[ptype]);
   bool ret = SelectMousePointerDirect(new_pointer);

   if (ret) {
      if (mouse_owners.back().first != caller) {
         std::pair<void* , MousePointer**> mouse_owner(caller , new_pointer);
         mouse_owners.push_back(mouse_owner);
      }
      current_pointer = new_pointer;
   }
   return ret;
}



void MousePointerManager::ReleaseMousePointer(const void* caller) {
   
   EAGLE_ASSERT(caller != this);// We never remove our own mouse pointer from the bottom of the deque
   EAGLE_ASSERT(mouse_owners.size());// There should always be at least one mouse owner in the deque

   if (mouse_owners.back().first == caller) {
      /// The pointer is in use, so we need to pop_back and then re-select, otherwise, just remove
      mouse_owners.pop_back();
      EAGLE_ASSERT(mouse_owners.size());
      MousePointer** oldmp = mouse_owners.back().second;
      bool oldmp_restored = SelectMousePointerDirect(oldmp);
      EAGLE_ASSERT(oldmp_restored);
      (void)oldmp_restored;
      return;
   }

   /// remove the topmost instance of the owner's mouse pointer, but only one so Acquire and Release pairs match up
   std::list< std::pair<void* , MousePointer**> >::reverse_iterator it = mouse_owners.rbegin();
   while (it != mouse_owners.rend()) {
      if (caller == it->first) {
         ///it = mouse_owners.erase(it);// no erase member that takes a reverse_iterator :PPP
         /// This is dumb, but forward iterate through the list to meet the other iterator
         /// TODO : This is also broken FIXME
         std::list< std::pair<void* , MousePointer**> >::iterator it2 = mouse_owners.begin();
         while (*it != *it2) {
            ++it2;
         }
         it2 = mouse_owners.erase(it2);
         break;
      }
      else {
         ++it;
      }
   }
}



void MousePointerManager::ReAcquireMouse() {
   if (current_pointer && *current_pointer) {
      SelectMousePointerDirect(current_pointer);
   }
}



bool MousePointerManager::SetCustomPointer(MousePointerInfo info) {
   
   MOUSE_POINTER_TYPE ptype = info.pointer_type;
   
   EAGLE_ASSERT(ptype != NUM_POINTERS);
   
   MousePointer* oldmp = (*pointer_set_in_use)[ptype];
   
   MousePointer* newmp = CreateMousePointer(info);
   
   (*pointer_set_in_use)[ptype] = newmp;
   
   delete oldmp;
   
   return newmp;
}


/*
void MousePointerManager::SetCustomPointerSet(MousePointerSet* pointer_set) {
   if (pointer_set) {
      pointer_set_in_use = pointer_set;
   }
   else {
      pointer_set_in_use = &custom_pointer_set;
   }
   if (current_pointer && pointer_set_in_use
       !(current_pointer->GetInfo().system_pointer_in_use)) {
      SelectMousePointerDirect(&(*pointer_set_in_use)[(*current_pointer)->GetInfo().pointer_type]);
   }
}
*/





/**
// Info is on one line, like so :
// POINTER_TYPE , "image_file.xxx" , xalign , yalign
bool ReadMousePointerInfo(string info_line , MousePointerInfo& info);
bool ReadMousePointerInfo(string info_line , MousePointerInfo& info) {
   const char* str = info_line.c_str();
   MOUSE_POINTER_TYPE ptype = NUM_POINTERS;
   for (int i = 0 ; i < NUM_POINTERS ; ++i) {
      if (strncmp(str , pointer_names[i] , strlen(pointer_names[i])) == 0) {
         ptype = (MOUSE_POINTER_TYPE)i;
         str += strlen(pointer_names[i]);
      }
   }
   if (ptype == NUM_POINTERS) {
      return false;
   }
   str = SkipWhiteSpace(str);
   if (*str != '\"') {
      return false;
   }
   string s = "";
   str = CopyQuotedString(str , s);
   if (!str) {
      return false;
   }
   
   /// Load pointer image here, str should hold file path
   
   
}
//*/




