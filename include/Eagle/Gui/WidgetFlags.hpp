
/**
 *
 *         _______       ___       ____      __       _______
 *        /\  ____\    /|   \     /  __\    /\ \     /\  ____\
 *        \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_
 *         \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\
 *          \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_
 *           \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *            \/______/|/_/  \/_/  \_\_____/    \/_____/ \/______/
 *
 *
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2019+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file WidgetFlags.hpp
 * @brief Functions and class to work with a set of flags for widgets
 * 
 */



#ifndef WidgetFlags_HPP
#define WidgetFlags_HPP


#include "Eagle/Indenter.hpp"

#include <string>
#include <iostream>



/**! @enum WIDGET_FLAGS
 *   @brief Defines the flags used by widgets
 */

enum WIDGET_FLAGS {
   FLAGS_NONE      = 0x000, ///< No flags set
   ENABLED         = 0x001, ///< Currently active or not
   VISIBLE         = 0x002, ///< Whether anything is drawn
   HOVER           = 0x004, ///< If it has mouse hover - don't steal the focus for hover!
   HASFOCUS        = 0x008, ///< If it has the focus
   MOVEABLE        = 0x010, ///< Whether it can be moved
   RESIZEABLE      = 0x020, ///< Whether it can be resized
   NEEDS_REDRAW    = 0x040, ///< Whether it should be redrawn before display?
   NEEDS_BG_REDRAW = 0x080, ///< Whether it needs its background redrawn before being redrawn
   ALLOW_CLOSE     = 0x100, ///< If it is allowed to close the dialog based on input
   NUM_WIDGET_FLAGS = 10
};



///< Get a string representation of the flags that are set
std::string WidgetFlagString(unsigned int flags);



/**! @class WidgetFlags
 *   @brief Stores the commonly used widget flags
 *   
 *   Provides a way to track changes to a set of widget flags
 */

class WidgetFlags {

protected :
   
typedef unsigned int UINT;

   UINT flags;        ///< Flags currently set
   UINT changed_flags;///< Flags that have changed since last time
   UINT added_flags;  ///< Flags that have been added since last time
   UINT removed_flags;///< Flags that have been removed since last time

public :
   WidgetFlags();///< Default constructor with no flags set
   WidgetFlags(int newflags);///< Constructor that sets the specified flags
   
   WidgetFlags& operator=(WidgetFlags wflags);///< Copies the flags passed
   
   WidgetFlags& SetNewFlags(WidgetFlags wflags);///< Specify new flags to be set
   
   WidgetFlags& SetNewFlag(WIDGET_FLAGS f , bool value);///< Set a specific flag

   WidgetFlags& AddFlag(WIDGET_FLAGS f);                  ///< Add a flag to the set
   WidgetFlags& RemoveFlag(WIDGET_FLAGS f);               ///< Remove a flag from the set
   WidgetFlags& AddFlags(unsigned int flags_to_add);      ///< Add multiple flags
   WidgetFlags& RemoveFlags(unsigned int flags_to_remove);///< Remove multiple flags

   unsigned int ChangedFlags() {return changed_flags;}///< Returns the flags that have changed
   unsigned int AddedFlags() {return added_flags;}    ///< Returns the flags that have been added
   unsigned int RemovedFlags() {return removed_flags;}///< Returns the flags that have been removed
   
   void Reset();///< Clear all flags and reset to FLAGS_NONE
   
   inline bool FlagOn(WIDGET_FLAGS f) {return flags & f;}///< Whether flag f is on
   inline bool FlagOff(WIDGET_FLAGS f) {return flags & ~f;}///< Whether flag f is off
   
   friend unsigned int FlagDiff(const WidgetFlags& wf1 , const WidgetFlags& wf2);///< Helper function
   
   bool operator&(unsigned int f) {return flags & f;}///< Bitwise AND and return result
   bool operator|(unsigned int f) {return flags | f;}///< Bitwise OR and return result
   
   operator unsigned int() {return flags;}///< Get the bitfield

   std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const ;///< Describe the flags to the stream

   friend std::ostream& operator<<(std::ostream& os , const WidgetFlags& wf);///< Helper function
};



///< Output a WidgetFlags object to a stream
std::ostream& operator<<(std::ostream& os , const WidgetFlags& wf);



///< Helper function for the @ref WidgetFlags class. Returns the bitwise difference between the flag sets (uses XOR)
unsigned int FlagDiff(const WidgetFlags& wf1 , const WidgetFlags& wf2);



#endif // WidgetFlags_HPP



