
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



#include "Eagle/Gui/WidgetFlags.hpp"




WidgetFlags::WidgetFlags(int newflags) :
      flags(newflags),
      changed_flags(0),
      added_flags(0),
      removed_flags(0)
{}



WidgetFlags& WidgetFlags::operator=(WidgetFlags wflags) {
   SetNewFlags(wflags);
   return *this;
}



WidgetFlags& WidgetFlags::SetNewFlags(WidgetFlags wflags) {
   changed_flags = flags ^ wflags.flags;
   added_flags = changed_flags & (~flags);/// Added flags equals flags that were off and were changed
   removed_flags = changed_flags & (flags);/// Removed flags equals flags that were on and were changed
   flags = wflags;
   return *this;
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



void WidgetFlags::Reset() {1
   changed_flags = added_flags = removed_flags = FLAGS_NONE;
}



/// -------------------    Global functions    --------------------------------



unsigned int FlagDiff(const WidgetFlags& wf1 , const WidgetFlags& wf2) {
   return (wf1.flags) ^ (wf2.flags);
}


