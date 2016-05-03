


#include "Eagle/Gui/Text.hpp"

#include "Eagle/GraphicsContext.hpp"
#include "Eagle/System.hpp"
#include "Eagle/Gui/WidgetHandler.hpp"


#include <cstdlib>


void GetTextAttributes(std::string text , EagleFont* font , int line_spacing , int* pnlines ,
                       std::vector<std::string>* plines , std::vector<int>* pwidths_vector ,
                       int* pmaxwidth , int* ptotalheight) {

   EAGLE_ASSERT(font);
   EAGLE_ASSERT(font->Valid());
///   EAGLE_ASSERT(pnlines);
///   EAGLE_ASSERT(plines);
///   EAGLE_ASSERT(pwidths_vector);
   EAGLE_ASSERT(pmaxwidth);
   EAGLE_ASSERT(ptotalheight);

   /// calculate size of entire text
   if (plines) {*plines = SplitByNewLines(text);}
   if (pnlines) {
      if (plines) {
         *pnlines = (int)plines->size();
      }
   }
   if (*pnlines < 1) {
      return;
   }
   int lineheight = font->Height();
   if (pwidths_vector) {pwidths_vector->clear();}
   *pmaxwidth = 0;
   for (int i = 0 ; i < *pnlines ; ++i) {
      int w = font->Width((*plines)[i].c_str());
      if (pwidths_vector) {pwidths_vector->push_back(w);}
      if (w > *pmaxwidth) {*pmaxwidth = w;}
   }
   *ptotalheight = lineheight*(*pnlines) + line_spacing*(*pnlines-1);

}



void DumbText::DrawText(EagleGraphicsContext* win , int xpos , int ypos , EagleColor c) {
   EAGLE_ASSERT(win);
   EAGLE_ASSERT(win->Valid());

   for (int i = 0 ; i < nlines ; ++i) {
      if (i == 0) {
         EAGLE_ASSERT(text_font && text_font->Valid());
      }
      string s = lines[i];
      Rectangle r = lineareas[i];
      win->DrawTextString(text_font , s , r.X() + xpos , r.Y() + ypos , c , halign , valign);
   }
///   win->DrawMultiLineTextString(text_font , text , tx + xpos , ty + ypos , c , linespacing , halign , valign);
}



void DumbText::RefreshTextPosition() {

   EAGLE_ASSERT(text_font);
   lines = SplitByNewLines(text);
   nlines = lines.size();
   
   int lineheight = text_font->Height();
   maxwidth = 0;
   totalheight = lineheight*nlines + linespacing*(nlines-1);

   lineareas.clear();
   
   if (nlines < 1) {return;}

   int h = text_font->Height();
   
   int ystart = InnerArea().Y() + vpadding;
   
   for (int i = 0 ; i < nlines ; ++i) {
      int w = text_font->Width(text.c_str());

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
         y = InnerArea().BRY() - vpadding - h;
      }
      if (w > maxwidth) {maxwidth = w;}
      lineareas.push_back(Rectangle(x,y,w,h));
   }
   SetBgRedrawFlag();
}



int DumbText::PrivateHandleEvent(EagleEvent e) {
   (void)e;
   return DIALOG_OKAY;
}



int DumbText::PrivateCheckInputs() {
   return DIALOG_OKAY;
}



void DumbText::PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos) {
   DrawText(win , xpos , ypos , WCols()[TXTCOL]);
}



int DumbText::PrivateUpdate(double tsec) {
   (void)tsec;
   return DIALOG_OKAY;
}


/**
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
   int maxwidth;
   int totalheight;
   std::vector<int> widths_vector;

//*/
DumbText::DumbText() :
      WidgetBase(StringPrintF("Dumb Text object at %p" , this)),
      halign(HALIGN_LEFT),
      valign(VALIGN_TOP),
      text_font(0),
      text(""),
      lines(),
      lineareas(),
      nlines(0),
      hpadding(0),
      vpadding(0),
      linespacing(0),
      maxwidth(0),
      totalheight(0),
      widths_vector()
{
   
}



DumbText::DumbText(string name) :
      WidgetBase(name),
      halign(HALIGN_LEFT),
      valign(VALIGN_TOP),
      text_font(0),
      text(""),
      lines(),
      lineareas(),
      nlines(0),
      hpadding(0),
      vpadding(0),
      linespacing(0),
      maxwidth(0),
      totalheight(0),
      widths_vector()

{
   
}
   


void DumbText::SetDrawPos(int xpos , int ypos , bool notify_layout) {
   WidgetBase::SetDrawPos(xpos,ypos,notify_layout);
   RefreshTextPosition();
}



void DumbText::SetDrawDimensions(int width , int height , bool notify_layout) {
   WidgetBase::SetDrawDimensions(width,height,notify_layout);
   RefreshTextPosition();
}



void DumbText::SetArea(int xpos , int ypos , int width , int height , bool notify_layout) {
   WidgetBase::SetArea(xpos,ypos,width,height,notify_layout);
   RefreshTextPosition();
}



void DumbText::SetArea(const Rectangle& r , bool notify_layout) {
   WidgetBase::SetArea(r.X() , r.Y() , r.W() , r.H() , notify_layout);
}



void DumbText::SetTextParameters(HALIGNMENT hal , VALIGNMENT val , int hpad , int vpad , int vspacing , EagleFont* font) { 
   EAGLE_ASSERT(font);
   halign = hal;
   valign = val;
   hpadding = hpad;
   vpadding = vpad;
   linespacing = vspacing;
   text_font = font;
   RefreshTextPosition();
   SetBgRedrawFlag();
}



void DumbText::SetTextString(std::string str , EagleFont* font) {
   EAGLE_ASSERT(font);
   text = str;
   SetFont(font);
   SetBgRedrawFlag();
}



void DumbText::SetFont(EagleFont* font) {
   EAGLE_ASSERT(font);
   text_font = font;
   RefreshTextPosition();
}



void DumbText::Refresh() {
   RefreshTextPosition();
}



/// -------------------------------------     SelectText    --------------------------------------



EagleGraphicsContext* SelectText::GetDrawWindow() {
   WidgetHandler* root_gui = RootGui();
   if (root_gui) {
      return root_gui->GetDrawWindow();
   }
   return 0;
}



int SelectText::CaretPos(int msx) {
   int caretp = 0;
   for (int index = 0 ; index < (int)text.size() - 1 ; ++index) {
      string sub = text.substr(0 , (unsigned int)index);
      string sub2 = text.substr(0 , (unsigned int)index + 1);
      float xpos = tx + text_font->Width(sub.c_str());
      float xpos2 = tx + text_font->Width(sub2.c_str());
      float cx = (xpos + xpos2) / 2.0;
      if (msx > xpos && msx <= cx) {
         caretp = index;
      }
      else if (msx > cx && msx <= xpos2) {
         caretp = index + 1;
      }
   }
   float xpos3 = tx + text_font->Width(text.c_str());
   if (msx >= xpos3) {
         caretp = (int)text.size();
   }
   return caretp;
}



void SelectText::RefreshSelection() {
   if (select_start < caret_pos) {
      select_left = select_start;
      select_right = caret_pos;
   }
   else {
      select_left = caret_pos;
      select_right = select_start;
   }
   if (select_left >= 0 && select_left <= select_right && select_right <= (int)text.size()) {
      selected_text = text.substr((unsigned int)select_left , (unsigned int)(select_right - select_left));
   }
}



SelectText::SelectText() :
      DumbText(StringPrintF("SelectText object at %p" , this)),
      caret_pos(0),
      select_start(0),
      select_left(0),
      select_right(0),
      drag(false),
      selected_text()
{}



SelectText::SelectText(string name) :
      DumbText(name),
      caret_pos(0),
      select_start(0),
      select_left(0),
      select_right(0),
      drag(false),
      selected_text()
{}



int SelectText::PrivateHandleEvent(EagleEvent e) {
   int flags = DIALOG_OKAY;
   if (IsMouseEvent(e)) {
      int msx = e.mouse.x;
      int msy = e.mouse.y;
      WidgetHandler* root_gui = RootGui();
      if (root_gui) {
         msx = root_gui->GetMouseX();
         msy = root_gui->GetMouseY();
      }
      if (e.type == EAGLE_EVENT_MOUSE_BUTTON_DOWN) {
         if (InnerArea().Contains(msx,msy)) {
            flags = flags | DIALOG_TAKE_FOCUS;
            if (input_key_held(EAGLE_KEY_LSHIFT) || input_key_held(EAGLE_KEY_RSHIFT)) {
               if (select_start != -1) {
                  caret_pos = CaretPos(msx);
                  RefreshSelection();
               }
            }
            else {
               select_left = 0;
               select_right = 0;
               caret_pos = select_start = CaretPos(msx);
               if (select_start != -1) {
                  select_left = select_start;
                  select_right = select_start;
                  selected_text = "";
                  drag = true;
               }
            }
         }
      }
      if (e.type == EAGLE_EVENT_MOUSE_AXES) {
         if (drag) {
            caret_pos = CaretPos(msx);
            RefreshSelection();
         }
      }
      if (e.type == EAGLE_EVENT_MOUSE_BUTTON_UP) {
         if (drag) {
            caret_pos = CaretPos(msx);
            RefreshSelection();
            drag = false;
         }
      }
   }
   if (flags & HASFOCUS) {
      if (e.type == EAGLE_EVENT_KEY_DOWN) {
         if (input_key_held(EAGLE_KEY_LSHIFT) || input_key_held(EAGLE_KEY_RSHIFT)) {
            if (e.keyboard.keycode == EAGLE_KEY_LEFT) {
               caret_pos--;
               if (caret_pos < 0) {caret_pos = 0;}
            }
            if (e.keyboard.keycode == EAGLE_KEY_RIGHT) {
               caret_pos++;
               if (caret_pos > (int)text.size()) {caret_pos = (int)text.size();}
            }
            RefreshSelection();
         }
         else {
            if (e.keyboard.keycode == EAGLE_KEY_LEFT) {
               select_start--;
               if (select_start < 0) {select_start = 0;}
            }
            if (e.keyboard.keycode == EAGLE_KEY_RIGHT) {
               select_start++;
               if (select_start > (int)text.size()) {select_start = (int)text.size();}
            }
            RefreshSelection();
         }
         if (e.keyboard.keycode == EAGLE_KEY_C) {
            if (input_key_held(EAGLE_KEY_LCTRL) || input_key_held(EAGLE_KEY_RCTRL)) {
               /// CTRL + C - copy to clipboard
               WidgetHandler* gui = RootGui();
               EAGLE_ASSERT(gui);
               EagleGraphicsContext* win = gui->GetDrawWindow();
               EAGLE_ASSERT(win);
               EAGLE_ASSERT(eagle_system);
               EAGLE_ASSERT(eagle_system->GetSystemClipboard());
               eagle_system->GetSystemClipboard()->CopyToClipboard(selected_text);
            }
         }
      }
   }
   return flags;
}



int SelectText::PrivateCheckInputs() {
   return DIALOG_OKAY;
}



void SelectText::PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos) {
   EAGLE_ASSERT(text_font);
   /// Draw selection background first
   int nchar = select_right - select_left;
   if (nchar) {
      string s1,s2;
      s1 = text.substr(0,select_left);
      s2 = text.substr(0,select_right);
      int w1 = text_font->Width(s1.c_str());
      int w2 = text_font->Width(s2.c_str());
      int x1 = tx + w1 + xpos;
      int x2 = tx + w2 + xpos;
      int y1 = ty + ypos;
      int y2 = y1 + text_font->Height();
      int w = x2 - x1;
      int h = y2 - y1;
      win->DrawFilledRectangle(x1,y1,w,h,WCols()[BGCOL]);
   }
   DrawText(win , xpos , ypos , WCols()[TXTCOL]);
   /// Draw caret
   string s = text.substr(0 , caret_pos);
   int x = tx + xpos + text_font->Width(s.c_str());
   int y = ty + ypos;
   int w = 3;
   int h = text_font->Height();
   win->DrawFilledRectangle(x+w/2,y,x+w/2,y+h,WCols()[MGCOL]);
   
   ClearRedrawFlag();
}



void SelectText::SetHoverState(bool state) {
   WidgetBase::SetHoverState(state);
   WidgetHandler* gui = RootGui();
   EAGLE_ASSERT(gui);
   EagleGraphicsContext* win = gui->GetDrawWindow();
   EAGLE_ASSERT(win);
   
   if (win) {
      if (state) {
         win->AcquireMousePointer(this , POINTER_EDIT , true);
      }
      else {
         win->ReleaseMousePointer(this);
      }
   }
   SetBgRedrawFlag();
}




/// ------------------------------     LinkText     -----------------------------------



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
   tx = x;
   ty = y;
   SetBgRedrawFlag();

}



int LinkText::PrivateHandleEvent(EagleEvent e) {
   int ret = SelectText::HandleEvent(e);
   if (e.type == EAGLE_EVENT_MOUSE_BUTTON_DOWN) {
      if (e.mouse.button == LMB) {
         if (ms_dblclick(LMB)) {
            /// Double click on link, launch hyperlink
            LaunchLink();
            QueueUserMessage(this , TOPIC_HYPERLINK , LINK_LAUNCHED);
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
   if (Flags() & HOVER) {
      float x1 = tx + xpos;
      float y1 = ty + ypos + 1.1*text_font->Height();
      float w = text_font->Width(text.c_str());
      float h = (0.1)*text_font->Height();
      win->DrawFilledRectangle(x1 , y1 , w , h , WCols()[HLCOL]);
   }
}




void LinkText::LaunchLink() {
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
      system(s2.c_str())
#endif
   }
   
}




