




#ifndef SelectText_HPP
#define SelectText_HPP



#include "Eagle/Gui/Text/GlobalText.hpp"
#include "Eagle/Gui/Text/BasicText.hpp"



#include "Eagle/Gui/Text/BasicText.hpp"



extern const int TEXT_COPIED;

REGISTER_WIDGET_MESSAGE(TOPIC_TEXT_WIDGET , TEXT_COPIED);


/// TODO : Cache string widths with current font and current string so they don't have to be measured over and over again

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

public :

   SelectText(std::string classname = "SelectText" , std::string objname = "Nemo");

   virtual ~SelectText() {}

   void FindCaretPos(int msx , int msy , int* pstrpos , int* plinenum);

   
   void GetCaretAttributes(int* pselect_line , int* pselect_pos , int* pcaret_line , int* pcaret_pos);
   
   Rectangle GetSelectionArea(int linenum , int leftchar , int rightchar , int basex , int basey);

   void DrawSelectionBackground(EagleGraphicsContext* win , int linenum , int left , int right , int xpos , int ypos);

   void MoveCaretUpOrDown(int keycode , bool shift_held);
   void MoveCaretLeftOrRight(int keycode , bool shift_held);

   void SetDeselectOnLostFocus(bool deselect);
   
   virtual void Refresh();
};




#endif // SelectText_HPP
