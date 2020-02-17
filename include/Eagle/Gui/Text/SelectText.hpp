
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
 * @file SelectText.hpp
 * @brief The interface for the SelectText class
 * 
 */

#ifndef SelectText_HPP
#define SelectText_HPP



#include "Eagle/Gui/Text/GlobalText.hpp"
#include "Eagle/Gui/Text/BasicText.hpp"



extern const int TEXT_COPIED;/// Message ID for when the text gets copied



REGISTER_WIDGET_MESSAGE(TOPIC_TEXT_WIDGET , TEXT_COPIED);///< Registers the TEXT_COPIED message ID with the TOPIC_TEXT_WIDGET topic


/**! @class SelectText
 *   @brief A class to make selectable text
 *   @sa BasicText
 *
 *   TODO : Cache string widths with current font and current string so they don't have to be measured over and over again
 */

class SelectText : public BasicText {

protected :
   
   double caret_time;
   double caret_blink_time;
   bool caret_visible;
   int caret_pos;
   int caret_line;
   int select_pos;
   int select_line;
   int select_line_start;
   int select_line_close;
   int select_left;
   int select_right;
   bool drag;
   std::string selected_text;

   bool deselect_on_lost_focus;
   
   

   /// WIDGETBASE
   virtual int PrivateHandleEvent(EagleEvent e);
   virtual int PrivateCheckInputs();
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);
   virtual int PrivateUpdate(double tsec);

   virtual void OnFlagChanged(WIDGET_FLAGS f , bool on);

   /// Members
   EagleGraphicsContext* GetDrawWindow();

   virtual void RefreshSelection();

//   bool GetSelectionIterators(std::string::iterator* itLeft , std::string::iterator* itRight);
   
   
public :

   SelectText(std::string classname = "SelectText" , std::string objname = "Nemo");

   virtual ~SelectText() {}

protected :
   void FindCaretPos(int msx , int msy , int* pstrpos , int* plinenum);

   void GetCaretAttributes(int* pselect_line , int* pselect_pos , int* pcaret_line , int* pcaret_pos);
   
   void DrawSelectionBackground(EagleGraphicsContext* win , int linenum , int left , int right , int xpos , int ypos);

   void OldMoveCaretUpOrDown(int keycode , bool shift_held);
   void OldMoveCaretLeftOrRight(int keycode , bool shift_held , bool ctrl_held);
   void OldMoveCaretHomeOrEnd(int keycode , bool shift_held , bool ctrl_held);
   
   void MoveCaret(int keycode , bool shift_held , bool ctrl_held);

   
   
   Rectangle GetSelectionArea(int linenum , int leftchar , int rightchar , int basex , int basey);

public :
   ///< Passing true will deselect the text for you if this widget loses focus
   ///< Passing false keeps the current text selection on lost focus
   void SetDeselectOnLostFocus(bool deselect);
   
   virtual void Refresh();
};




#endif // SelectText_HPP
