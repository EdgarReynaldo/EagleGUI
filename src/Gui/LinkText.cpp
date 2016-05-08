




#include "Eagle/Gui/LinkText.hpp"


#include "Eagle/Platform.hpp"
#include "Eagle/Gui/Text.hpp"

#include "Eagle/GraphicsContext.hpp"
#include "Eagle/System.hpp"
#include "Eagle/Gui/WidgetHandler.hpp"


#include <cstdlib>




/// ------------------------------     LinkText     -----------------------------------


/**
void LinkText::RefreshTextPosition() {

   EAGLE_ASSERT(text_font);

   int w = text_font->Width(text.c_str());
   int h = (int)(1.2*text_font->Height());

   int x = InnerArea().X() + hpadding;
   int y = InnerArea().Y() + vpadding;
   if (halign == HALIGN_CENTER) {
      x = InnerArea().CX() - w/2;
   }
   if (halign == HALIGN_RIGHT) {
      x = InnerArea().BRX() - hpadding - w;
   }
   if (valign == VALIGN_CENTER) {
      y = InnerArea().CY() - h/2;
   }
   if (valign == VALIGN_BOTTOM) {
      y = InnerArea().BRY() + vpadding - h;
   }
   textx = x;
   texty = y;
   SetBgRedrawFlag();

}
//*/


void LinkText::RefreshTextPosition(int lineheight) {
   DumbText::RefreshTextPosition(lineheight);
}



void LinkText::ResetLinePadding() {
   float fontheight = text_font->Height();
   vline_padding = 0.2*fontheight;
   lineheight = fontheight + vline_padding + linespacing;
}



int LinkText::LinkText::PrivateHandleEvent(EagleEvent e) {
   int ret = SelectText::PrivateHandleEvent(e);
   if (e.type == EAGLE_EVENT_MOUSE_BUTTON_DOWN) {
      int msx = RootGui()->GetMouseX();
      int msy = RootGui()->GetMouseY();
      if (InnerArea().Contains(msx,msy)) {
         if (e.mouse.button == LMB) {
            if (ms_dblclick(LMB)) {
               /// Double click on link, launch hyperlink
               LaunchLink();
            }
         }
      }
   }
   return ret;
}



int LinkText::PrivateCheckInputs() {
   return DIALOG_OKAY;
}



void LinkText::PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos) {
   SelectText::PrivateDisplay(win,xpos,ypos);
   EAGLE_ASSERT(text_font);
   EAGLE_ASSERT(text_font->Valid());

   /// Draw underline for hyperlink
   float y1 = texty + ypos + 1.1*text_font->Height();
   if (Flags() & HOVER) {
      for (int i = 0 ; i < nlines ; ++i) {
         float x1 = textx + xpos;
         float w = text_font->Width(text.c_str());
         float h = (0.1)*text_font->Height();
         win->DrawFilledRectangle(x1 , y1 , w , h , WCols()[HLCOL]);
         y1 += lineheight;
      }
   }
}



int LinkText::PrivateUpdate(double tsec) {
   return SelectText::PrivateUpdate(tsec);
}



void LinkText::SetWidgetArea(int xpos , int ypos , int width , int height , bool notify_layout) {
   WidgetBase::SetWidgetArea(xpos,ypos,width,height,notify_layout);
   RefreshTextPosition(lineheight);
}


void LinkText::SetHoverState(bool state) {
   SelectText::SetHoverState(state);
   SetBgRedrawFlag();
}



void LinkText::SetupText(HALIGNMENT hal , VALIGNMENT val , int hpad , int vpad , int vspacing ,
                       std::string textstr , EagleFont* font) {
   DumbText::SetupText(hal,val,hpad,vpad,vspacing,textstr,font);
   ResetLinePadding();
   Refresh();
}



void LinkText::SetText(std::string textstr , EagleFont* font) {
   DumbText::SetText(textstr , font);
   ResetLinePadding();
   Refresh();
}



void LinkText::SetFont(EagleFont* font) {
   DumbText::SetFont(font);
   ResetLinePadding();
   Refresh();
}



void LinkText::LaunchLink() {
   /// TODO : Should probably do more to sanitize the input. Who knows what someone might run if they
   /// TODO : know about the system calls?
   string s1 = text;
   bool link = false;
   if (strncmp(s1.c_str() , "http://" , 7) == 0) {
      /// Should have a web address here
      link = true;
   }
   else if (strncmp(s1.c_str() , "www." , 4) == 0) {
      /// Should be a link here too
      link = true;
   }
   if (link) {
#ifdef EAGLE_WIN32
      string s2 = string("start ") + s1;
      system(s2.c_str());
#endif
      QueueUserMessage(this , TOPIC_TEXT_WIDGET , LINK_LAUNCHED);
   }
   
}



