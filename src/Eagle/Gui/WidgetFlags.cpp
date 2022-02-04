
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
 *    Copyright 2009-2021+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */



#include "Eagle/Gui/WidgetFlags.hpp"

#include <sstream>


std::string WidgetFlagString(unsigned int flags) {
   static const char* fstrs[NUM_WIDGET_FLAGS] = {
      "FLAGS_NONE",
      "ENABLED",
      "VISIBLE",
      "HOVER",
      "HASFOCUS",
      "MOVEABLE",
      "RESIZEABLE",
      "NEEDS_REDRAW",
      "NEEDS_BG_REDRAW",
      "ALLOW_CLOSE"
   };
   if (!flags) {
      return fstrs[0];
   }
   std::stringstream strm;
   strm.str("");
   bool first = true;
   for (int i = 1 ; i < NUM_WIDGET_FLAGS ; ++i) {
      unsigned int f = 1 << (i-1);
      if (flags & f) {
         if (!first) {
            strm << " | ";
         }
         strm << fstrs[i];
         first = false;
      }
      
   }
   return strm.str();
}



WidgetFlags::WidgetFlags() :
      flags(FLAGS_NONE),
      changed_flags(FLAGS_NONE),
      added_flags(FLAGS_NONE),
      removed_flags(FLAGS_NONE)
{}

WidgetFlags::WidgetFlags(int newflags) :
      flags(newflags),
      changed_flags(newflags),
      added_flags(newflags),
      removed_flags(FLAGS_NONE)
{}



WidgetFlags::WidgetFlags(const WidgetFlags& f) :
      flags(0),
      changed_flags(0),
      added_flags(0),
      removed_flags(0)
{
   *this = f;
}



WidgetFlags& WidgetFlags::operator=(WidgetFlags wflags) {
   SetNewFlags(wflags);
   return *this;
}



WidgetFlags& WidgetFlags::SetNewFlags(WidgetFlags wflags) {
   changed_flags = flags ^ wflags.flags;
   added_flags = changed_flags & (~flags);/// Added flags equals flags that were off and were changed
   removed_flags = changed_flags & (flags);/// Removed flags equals flags that were on and were changed
   flags = wflags.flags;
   return *this;
}



WidgetFlags& WidgetFlags::SetNewFlag(WIDGET_FLAGS f , bool value) {
   return (value?AddFlag(f):RemoveFlag(f));
}



WidgetFlags& WidgetFlags::AddFlag(WIDGET_FLAGS f) {
   return AddFlags((UINT)f);
}



WidgetFlags& WidgetFlags::RemoveFlag(WIDGET_FLAGS f) {
   return RemoveFlags((UINT)f);
}



WidgetFlags& WidgetFlags::AddFlags(unsigned int flags_to_add) {
   flags_to_add &= ~added_flags;/// Only add flags that are not added already
   removed_flags &= ~flags_to_add;
   added_flags |= flags_to_add;
   changed_flags = removed_flags | added_flags;
   flags |= flags_to_add;
   return *this;
}



WidgetFlags& WidgetFlags::RemoveFlags(unsigned int flags_to_remove) {
   flags_to_remove &= ~removed_flags;/// Only remove flags that were not removed already
   removed_flags |= flags_to_remove;
   added_flags &= ~flags_to_remove;
   changed_flags = removed_flags | added_flags;
   flags &= ~flags_to_remove;
   return *this;
}



void WidgetFlags::Reset() {
   changed_flags = added_flags = removed_flags = FLAGS_NONE;
}



std::ostream& WidgetFlags::DescribeTo(std::ostream& os , Indenter indent) const {
   os << indent << WidgetFlagString(flags);
   return os;
}



/// -------------------    Global functions    --------------------------------



unsigned int FlagDiff(const WidgetFlags& wf1 , const WidgetFlags& wf2) {
   return (wf1.flags) ^ (wf2.flags);
}


std::ostream& operator<<(std::ostream& os , const WidgetFlags& wf) {
   return wf.DescribeTo(os);
}



