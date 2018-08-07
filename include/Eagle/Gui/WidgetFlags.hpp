
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
 *    EAGLE
 *    Edgar's Agile Gui Library and Extensions
 *
 *    Copyright 2009-2018+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */
 



#ifndef WidgetFlags_HPP
#define WidgetFlags_HPP


#include "Eagle/Indenter.hpp"

#include <string>
#include <iostream>


enum WIDGET_FLAGS {
   FLAGS_NONE      = 0x000,
   ENABLED         = 0x001,   // Currently active or not
   VISIBLE         = 0x002,   // Whether anything is drawn
   HOVER           = 0x004,   // If it has mouse hover - don't steal the focus for hover!
   HASFOCUS        = 0x008,   // If it has the focus
   MOVEABLE        = 0x010,   // Whether it can be moved
   RESIZEABLE      = 0x020,   // Whether it can be resized
   NEEDS_REDRAW    = 0x040,   // Whether it should be redrawn before display?
   NEEDS_BG_REDRAW = 0x080,   // Whether it needs its background redrawn before being redrawn
   ALLOW_CLOSE     = 0x100,   // If it is allowed to close the dialog based on input
   NUM_WIDGET_FLAGS = 10
};

std::string WidgetFlagString(unsigned int flags);

class WidgetFlags {

protected :
   
typedef unsigned int UINT;

   UINT flags;
   UINT changed_flags;
   UINT added_flags;
   UINT removed_flags;

public :
   WidgetFlags();
   WidgetFlags(int newflags);
   
   WidgetFlags& operator=(WidgetFlags wflags);
   
   WidgetFlags& SetNewFlags(WidgetFlags wflags);
   
   WidgetFlags& SetNewFlag(WIDGET_FLAGS f , bool value);

   WidgetFlags& AddFlag(WIDGET_FLAGS f);
   WidgetFlags& RemoveFlag(WIDGET_FLAGS f);
   WidgetFlags& AddFlags(unsigned int flags_to_add);
   WidgetFlags& RemoveFlags(unsigned int flags_to_remove);

   unsigned int ChangedFlags() {return changed_flags;}
   unsigned int AddedFlags() {return added_flags;}
   unsigned int RemovedFlags() {return removed_flags;}
   
   void Reset();
   
   inline bool FlagOn(WIDGET_FLAGS f) {return flags & f;}
   inline bool FlagOff(WIDGET_FLAGS f) {return flags & ~f;}
   
   friend unsigned int FlagDiff(const WidgetFlags& wf1 , const WidgetFlags& wf2);
   
   bool operator&(unsigned int f) {return flags & f;}
   bool operator|(unsigned int f) {return flags | f;}
   
   operator unsigned int() {return flags;}

   std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const ;

   friend std::ostream& operator<<(std::ostream& os , const WidgetFlags& wf);
};


std::ostream& operator<<(std::ostream& os , const WidgetFlags& wf);

unsigned int FlagDiff(const WidgetFlags& wf1 , const WidgetFlags& wf2);


#endif // WidgetFlags_HPP






