
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



#include "Eagle/Platform.hpp"

#include "Eagle/Gui/WidgetHandler.hpp"

#include "Eagle/Gui/Text/GlobalText.hpp"
#include "Eagle/Gui/Text/BasicText.hpp"
#include "Eagle/Gui/Text/LinkText.hpp"

#include "Eagle/GraphicsContext.hpp"
#include "Eagle/System.hpp"
#include "Eagle/StringWork.hpp"



#include <cstdlib>
#include <cstring>
#include <cmath>



const int LINK_LAUNCHED = GetNextFreeTextMessageId();



REGISTERED_WIDGET_MESSAGE(TOPIC_TEXT_WIDGET , LINK_LAUNCHED);



/// ------------------------------     LinkText     -----------------------------------



int LinkText::LinkText::PrivateHandleEvent(EagleEvent e) {
   int ret = SelectText::PrivateHandleEvent(e);
   if (e.type == EAGLE_EVENT_MOUSE_BUTTON_DOWN) {
      int msx = e.mouse.x;
      int msy = e.mouse.y;
      if (InnerArea().Contains(msx,msy)) {
         if (e.mouse.button == LMB) {
            if (ms_dblclick(LMB)) {
               /// Double click on link, launch hyperlink
               LaunchLink();
               ret |= DIALOG_INPUT_USED;
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

   if (Flags().FlagOn(HOVER)) {
      /// Draw underline for hyperlink
      for (unsigned int i = 0 ; i < lineareas.size() ; ++i) {
         const Rectangle& r = lineareas[i];
         float x = r.X() + xpos;
         float y = r.Y() + ypos + fontheight + ceil(link_height);
         win->DrawFilledRectangle(x,y,(float)r.W(), link_height , GetColor(HLCOL));
      }
   }
}



int LinkText::PrivateUpdate(double tsec) {
   return SelectText::PrivateUpdate(tsec);
}



void LinkText::OnFlagChanged(WIDGET_FLAGS f , bool on) {
   (void)on;
   if (f == HOVER) {
      SetBgRedrawFlag();
   }
}



LinkText::LinkText(std::string classname , std::string objname) :
   SelectText(classname , objname),
   link_height(0.0f)
{}



void LinkText::SetFont(EagleFont* font) {
   BasicText::SetupText(text , font);
   
   SetLineSpacing(linespacing);
}



void LinkText::SetLineSpacing(int vspacing) {

   if (text_font) {
      link_height = 0.1f*(text_font->Height());
   }
   else {
      link_height = 0.0f;
   }
   
   int min_link_vspace = (int)ceil(3.0f*link_height);
   
   if (vspacing < min_link_vspace) {
      vspacing = min_link_vspace;
   }

   BasicText::SetLineSpacing(min_link_vspace);

}



void LinkText::LaunchLink() {
   /// TODO : Should probably do more to sanitize the input. Who knows what someone might run if they
   /// TODO : know about the system calls?
   std::string s1 = text;
   bool link = false;
   if (strncmp(s1.c_str() , "http://" , 7) == 0) {
      /// Should have a web address here
      link = true;
   }
   if (strncmp(s1.c_str() , "https://" , 8) == 0) {
      /// Should have a web address here
      link = true;
   }
   else if (strncmp(s1.c_str() , "www." , 4) == 0) {
      /// Should be a link here too
      link = true;
   }
   if (link) {
      /// TODO : Launching links only works on Windows, there is no code for other platforms yet
#ifdef EAGLE_WIN32
      std::string s2 = std::string("start ") + s1;
      system(s2.c_str());
#endif
      RaiseEvent(WidgetMsg(this , TOPIC_TEXT_WIDGET , LINK_LAUNCHED));
   }
   
}



