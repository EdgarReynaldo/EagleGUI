




#ifndef SelectText_HPP
#define SelectText_HPP


#include "Eagle/Gui/Text.hpp"

#include "Eagle/Gui/DumbText.hpp"


class SelectText : public DumbText {

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
   string selected_text;

   
   
   EagleGraphicsContext* GetDrawWindow();
public :
   void FindCaretPos(int msx , int msy , int* pstrpos , int* plinenum);

   virtual void RefreshSelection();
   
public :

   SelectText();
   SelectText(string name);
   virtual ~SelectText() {}
   
   void DrawSelectionBackground(int linenum , int left , int right , int xpos , int ypos);

   void MoveCaretUpOrDown(int keycode , bool shift_held);
   void MoveCaretLeftOrRight(int keycode , bool shift_held);

   virtual int PrivateHandleEvent(EagleEvent e);
   virtual int PrivateCheckInputs();
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);
   virtual int PrivateUpdate(double tsec);

   virtual void SetHoverState(bool state);

};




#endif // SelectText_HPP
