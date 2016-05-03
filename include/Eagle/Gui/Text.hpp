



#ifndef Text_HPP
#define Text_HPP

#include "Eagle/Gui/Alignment.hpp"
#include "Eagle/Gui/WidgetBase.hpp"
#include "Eagle/StringWork.hpp"

#include <vector>
#include <string>

void GetTextAttributes(std::string text , EagleFont* font , int line_spacing , int* pnlines ,
                       std::vector<std::string>* plines , std::vector<int>* pwidths_vector ,
                       int* pmaxwidth , int* ptotalheight);


using std::string;

class DumbText : public WidgetBase {

protected :
   HALIGNMENT halign;
   VALIGNMENT valign;
   EagleFont* text_font;
   string text;
   std::vector<std::string> lines;
   std::vector<Rectangle> lineareas;
   int nlines;
   int hpadding;
   int vpadding;
   int linespacing;
   int tx;
   int ty;
   int maxwidth;
   int totalheight;
   std::vector<int> widths_vector;
   

   
   void DrawText(EagleGraphicsContext* win , int xpos , int ypos , EagleColor c);
   
   virtual void RefreshTextPosition();

   virtual int PrivateHandleEvent(EagleEvent e);
   virtual int PrivateCheckInputs();
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);
   virtual int PrivateUpdate(double tsec);

   
public :
   DumbText();
   DumbText(string name);
   virtual ~DumbText(){}
//**
   virtual void SetDrawPos(int xpos , int ypos , bool notify_layout = true);
   virtual void SetDrawDimensions(int width , int height , bool notify_layout = true);
   virtual void SetArea(int xpos , int ypos , int width , int height , bool notify_layout = true);
   void         SetArea(const Rectangle& r , bool notify_layout = true);
//*/

   void SetTextParameters(HALIGNMENT hal , VALIGNMENT val , int hpad , int vpad , int vspacing , EagleFont* font);
   void SetTextString(std::string str , EagleFont* font);
   
   void SetFont(EagleFont* font);
   void Refresh();

   
   string GetText() {return text;}
   
};



class SelectText : public DumbText {

protected :
   
   int caret_pos;
   int select_start;
   int select_left;
   int select_right;
   bool drag;
   string selected_text;

   
   
   EagleGraphicsContext* GetDrawWindow();
      
   int CaretPos(int msx);

   void RefreshSelection();
   
public :

   SelectText();
   SelectText(string name);
   virtual ~SelectText() {}
   
   virtual int PrivateHandleEvent(EagleEvent e);
   virtual int PrivateCheckInputs();
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);

   virtual void SetHoverState(bool state);

};



static const int TOPIC_HYPERLINK = NextFreeTopicId();

static const int LINK_LAUNCHED = 0;

REGISTER_WIDGET_MESSAGE(TOPIC_HYPERLINK , LINK_LAUNCHED);



class LinkText : public SelectText {

   

protected :
   virtual void RefreshTextPosition();

public :
   LinkText();
   
   /// overload hoverstate

   virtual int PrivateHandleEvent(EagleEvent e);
   virtual int PrivateCheckInputs();
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);
   
   void LaunchLink();
   
};



#endif // Text_HPP
