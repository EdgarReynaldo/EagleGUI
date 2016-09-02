



#include "Eagle/Gui/Text/GlobalText.hpp"
#include "Eagle/Gui/Text/SelectText.hpp"


#include "Eagle/Platform.hpp"

#include "Eagle/GraphicsContext.hpp"
#include "Eagle/System.hpp"
#include "Eagle/Gui/WidgetHandler.hpp"

#include "Eagle/StringWork.hpp"



#include <cstdlib>


const int TEXT_COPIED = GetNextFreeTextMessageId();

REGISTERED_WIDGET_MESSAGE(TOPIC_TEXT_WIDGET , TEXT_COPIED);



/// -------------------------------------     SelectText    --------------------------------------



EagleGraphicsContext* SelectText::GetDrawWindow() {
   WidgetHandler* root_gui = RootGui();
   if (root_gui) {
      return root_gui->GetDrawWindow();
   }
   return 0;
}


/*
      caret_pos(-1),
      caret_line(-1),
      select_pos(-1),
      select_line(-1),
      select_line_start(-1),
      select_line_close(-1),
      select_left(-1),
      select_right(-1),
      drag(false),
      selected_text()
*/
void SelectText::FindCaretPos(int msx , int msy , int* pstrpos , int* plinenum) {
   EAGLE_ASSERT(pstrpos);
   EAGLE_ASSERT(plinenum);
   Rectangle inner = InnerArea();
   *pstrpos = -1;
   *plinenum = -1;
   /// TODO : This only works for HALIGN_LEFT AND VALIGN_TOP

   EAGLE_ASSERT(lines.size());
   if (!lines.size()) {return;}
      
   if (inner.Contains(msx,msy)) {

      if (msy < lineareas[0].Y()) {
         *plinenum = 0;
      }
      else if (msy > lineareas[lineareas.size() - 1].BRY()) {
         *plinenum = lineareas.size() - 1;
      }
      else {
         for (int line = 0 ; line < nlines ; ++line) {
            Rectangle r = lineareas[line];
            int ty = r.Y();
            int by = r.BRY();
            if (msy >= ty && msy <= by) {
               *plinenum = line;
            }
         }
      }
      string linestr = lines[*plinenum];
      Rectangle r = lineareas[*plinenum];
      int lx = r.X();
      int rx = r.BRX();
      if (msx < lx) {
         *pstrpos = 0;
      }
      else if (msx >= rx) {
         *pstrpos = (int)linestr.size();
      }
      else {
         *pstrpos = 0;
         for (int index = 0; index < (int)linestr.size() ; ++index) {
            string sub = linestr.substr(0 , (unsigned int)index);
            string sub2 = linestr.substr(0 , (unsigned int)index + 1);
            float xpos = lx + text_font->Width(sub.c_str());
            float xpos2 = lx + text_font->Width(sub2.c_str());
            float cx = (xpos + xpos2) / 2.0;
            if (msx >= xpos && msx < cx) {
               *pstrpos = index;
               break;
            }
            else if (msx >= cx && msx < xpos2) {
               *pstrpos = index + 1;
               break;
            }
         }
      }
   }
}



void SelectText::RefreshSelection() {
   
   SetBgRedrawFlag();

   selected_text = "";
   
   select_line_start = -1;
   select_line_close = -1;
   select_left = -1;
   select_right = -1;
   
   if (select_line == -1) {
      return;
   }
   
   if (select_line < caret_line) {
      select_line_start = select_line;
      select_line_close = caret_line;
      select_left = select_pos;
      select_right = caret_pos;
   }
   else if (select_line == caret_line) {
      select_line_start = select_line_close = select_line;
      if (select_pos < caret_pos) {
            select_left = select_pos;
            select_right = caret_pos;
      }
      else if (select_pos == caret_pos) {
         select_left = select_right = select_pos;
      }
      else {
         /// select pos > caret_pos
         select_left = caret_pos;
         select_right = select_pos;
      }
   }
   else {
      /// select_line > caret_line
      select_line_start = caret_line;
      select_line_close = select_line;
      select_left = caret_pos;
      select_right = select_pos;
   }

   selected_text = "";
   if (select_line == -1) {
      return;
   }

   EAGLE_ASSERT(select_line_start >= 0 && select_line_start < nlines);
   EAGLE_ASSERT(select_line_close >= 0 && select_line_close < nlines);
   EAGLE_ASSERT(select_left >= 0 && select_left <= (int)lines[select_line_start].size());
   EAGLE_ASSERT(select_right >= 0 && select_right <= (int)lines[select_line_close].size());
   
///   EagleLog() << StringPrintF("LineStart = %d , LineClose = %d , Nlines = %d" , select_line_start , select_line_close , nlines) << std::endl;
///   EagleLog() << StringPrintF("Left = %d , size() = %d , Right = %d , size() = %d", 
///                              select_left , (int)lines[select_line_start].size() , select_right , (int)lines[select_line_close].size()) << std::endl;
                              
   if (select_line_start == select_line_close) {
      selected_text = lines[select_line_start].substr(select_left , select_right - select_left);
   }
   else {
      string s1 = lines[select_line_start];
      string s2 = lines[select_line_close];
      for (int i = select_line_start ; i <= select_line_close ; ++i) {
         if (i == select_line_start) {
               selected_text = s1.substr(select_left);
               selected_text += '\n';
         }
         else if (i == select_line_close) {
               selected_text += s2.substr(0 , select_right);
         }
         else {
            string s3 = lines[i];
            selected_text += s3;
            selected_text += '\n';
         }
      }
   }
}


SelectText::SelectText() :
      BasicText(StringPrintF("SelectText object at %p" , this)),
      caret_time(0.0f),
      caret_blink_time(0.5f),
      caret_visible(true),
      caret_pos(0),
      caret_line(0),
      select_pos(0),
      select_line(0),
      select_line_start(0),
      select_line_close(0),
      select_left(0),
      select_right(0),
      drag(false),
      selected_text(""),
      deselect_on_lost_focus(true)
{}



SelectText::SelectText(string name) :
      BasicText(name),
      caret_time(0.0),
      caret_blink_time(0.5),
      caret_visible(true),
      caret_pos(0),
      caret_line(0),
      select_pos(0),
      select_line(0),
      select_line_start(0),
      select_line_close(0),
      select_left(0),
      select_right(0),
      drag(false),
      selected_text(""),
      deselect_on_lost_focus(true)
{}



Rectangle SelectText::GetSelectionArea(int linenum , int leftchar , int rightchar , int basex , int basey) {
   EAGLE_ASSERT(linenum >= 0 && linenum < (int)lines.size());
   EAGLE_ASSERT(leftchar >= 0 && leftchar <= (int)lines[linenum].size());
   EAGLE_ASSERT(rightchar >= 0 && rightchar <= (int)lines[linenum].size());
   EAGLE_ASSERT(leftchar <= rightchar);
   
   EAGLE_ASSERT(text_font && text_font->Valid());
   
   Rectangle r = lineareas[linenum];
   string s = lines[linenum];

   int tx = r.X();
   int ty = r.Y();

   string s1,s2;
   s1 = s.substr(0,leftchar);
   s2 = s.substr(0,rightchar);
   int w1 = text_font->Width(s1.c_str());
   int w2 = text_font->Width(s2.c_str());
   int x1 = tx + w1 + basex;
   int x2 = tx + w2 + basex;
   int y1 = ty + basey;
   int y2 = y1 + text_font->Height();
   int w = x2 - x1;
   int h = y2 - y1;
   
   int nchar = rightchar - leftchar;
   
   if (nchar <= 0) {
      w = 0;
      h = 0;
   }
   
   return Rectangle(x1,y1,w,h);
   
}



void SelectText::DrawSelectionBackground(EagleGraphicsContext* win , int linenum , int left , int right , int xpos , int ypos) {
   EAGLE_ASSERT(win && win->Valid());

   Rectangle r = GetSelectionArea(linenum , left , right , xpos , ypos);

   if (r.W()) {
      win->DrawFilledRectangle(r , WCols()[MGCOL]);
      win->DrawRectangle(r , 3.0 , WCols()[HLCOL]);
   }
}



void SelectText::MoveCaretUpOrDown(int keycode , bool shift_held) {

   EAGLE_ASSERT(lines.size());
   EAGLE_ASSERT(keycode == EAGLE_KEY_UP || keycode == EAGLE_KEY_DOWN);

   /// TODO : Compensate for alignment - for now assume left alignment
   int old_caret_line = caret_line;
   if (keycode == EAGLE_KEY_UP) {
      caret_line--;
      if (caret_line < 0) {
         /// Already at the top, no need to change the caret position
         caret_line = 0;
      }
   }
   if (keycode == EAGLE_KEY_DOWN) {
      caret_line++;
      if (caret_line >= ((int)lines.size() - 1)) {
         caret_line = (int)lines.size() - 1;
      }
   }
   if (old_caret_line == caret_line) {
      /// Nothing needs to change
      return;
   }
   
   Rectangle r = lineareas[old_caret_line];
   int lx = r.X();
   
   string os = lines[old_caret_line];
   int caretx = lx + text_font->Width(os.substr(0 , caret_pos).c_str());

   Rectangle r2 = lineareas[caret_line];
   int nlx = r2.X();
   int nrx = r2.BRX();

   int new_caret_pos = -1;
   string ns = lines[caret_line];

   if (caretx < nlx) {
      new_caret_pos = 0;
   }
   else if (caretx >= nrx) {
      new_caret_pos = ns.size();
   }
   else {
      new_caret_pos = 0;
      if (ns.size() > 1) {
         float x1 = nlx + text_font->Width(ns.substr(0 , 0).c_str());
         float x2 = nlx + text_font->Width(ns.substr(0 , 1).c_str());
         for (int i = 0 ; i < (int)ns.size() - 1 ; ++i) {
            float cx = (x1 + x2)/2.0;
            if (caretx >= x1 && caretx <= x2) {
               if (caretx < cx) {
                  new_caret_pos = i;
               }
               else if (caretx >= cx) {
                  new_caret_pos = i + 1;
               }
            }
            x1 = x2;
            x2 = nlx + text_font->Width(ns.substr(0 , i + 1).c_str());
         }
      }
   }
   
   caret_pos = new_caret_pos;
   
   /// We know that the caret line has changed - if shift is not held, then the select_pos has to be updated too
   if (!shift_held) {
      select_pos = caret_pos;
      select_line = caret_line;
   }
   
   RefreshSelection();
}



void SelectText::MoveCaretLeftOrRight(int keycode , bool shift_held) {

   EAGLE_ASSERT(lines.size());
   EAGLE_ASSERT(keycode == EAGLE_KEY_LEFT || keycode == EAGLE_KEY_RIGHT);

   int old_caret_pos = caret_pos;
   int old_caret_line = caret_line;
   if (keycode == EAGLE_KEY_LEFT) {
      caret_pos--;
      if (caret_pos < 0) {
         if (caret_line > 0) {
            --caret_line;
            caret_pos = lines[caret_line].size();
         }
         else {
            EAGLE_ASSERT(caret_line == 0);
            caret_pos = 0;
         }
      }
   }
   if (keycode == EAGLE_KEY_RIGHT) {
      caret_pos++;
      if (caret_pos > (int)lines[caret_line].size()) {
         if (caret_line < ((int)(lines.size() - 1))) {
            ++caret_line;
            caret_pos = 0;
         }
         else {
            EAGLE_ASSERT(caret_line == ((int)lines.size() - 1 ));
            caret_pos = (int)(lines[caret_line].size());
         }
      }
   }
   if (caret_pos != old_caret_pos || caret_line != old_caret_line) {
      /// We know that the caret position has changed - if shift is not held, then the select_pos has to be updated too
      if (!shift_held) {
         select_pos = caret_pos;
         select_line = caret_line;
         RefreshSelection();
      }
   }
}



int SelectText::PrivateHandleEvent(EagleEvent ev) {
   int retflags = DIALOG_OKAY;
   if (IsMouseEvent(ev)) {
      int msx = ev.mouse.x;
      int msy = ev.mouse.y;
      if (ev.type == EAGLE_EVENT_MOUSE_BUTTON_DOWN) {
         EagleLog() << "SelectText::PrivateHandleEvent - mouse button down." << std::endl;
         if (InnerArea().Contains(msx,msy)) {
            retflags |= DIALOG_TAKE_FOCUS;
            if (input_key_held(EAGLE_KEY_LSHIFT) || input_key_held(EAGLE_KEY_RSHIFT)) {
               if (select_pos != -1) {
                  int new_caret_pos = -1;
                  int new_caret_line = -1;
                  FindCaretPos(msx , msy , &new_caret_pos , &new_caret_line);
                  if (new_caret_line != -1 && new_caret_pos != -1) {
                     caret_pos = new_caret_pos;
                     caret_line = new_caret_line;
                  }
                  RefreshSelection();
               }
            }
            else {
               int new_caret_pos = -1;
               int new_caret_line = -1;
               FindCaretPos(msx , msy , &new_caret_pos , &new_caret_line);
               if (new_caret_line != -1 && new_caret_pos != -1) {
                  select_pos = caret_pos = new_caret_pos;
                  select_line = caret_line = new_caret_line;
                  drag = true;
               }
               RefreshSelection();
            }
         }
      }
      if (ev.type == EAGLE_EVENT_MOUSE_AXES) {
         if (drag) {
            EagleLog() << "SelectText::PrivateHandleEvent - mouse axes event and drag begin." << std::endl;
            int newcaretpos = -1;
            int newcaretline = -1;
            FindCaretPos(msx , msy , &newcaretpos , &newcaretline);
            if (newcaretline != -1 && newcaretpos != -1) {
               caret_pos = newcaretpos;
               caret_line = newcaretline;
            }
            RefreshSelection();
            EagleLog() << "SelectText::PrivateHandleEvent - mouse axes event and drag finish." << std::endl;
         }
      }
      if (ev.type == EAGLE_EVENT_MOUSE_BUTTON_UP) {
         if (drag) {
            int newcaretpos = -1;
            int newcaretline = -1;
            FindCaretPos(msx , msy , &newcaretpos , &newcaretline);
            if (newcaretline != -1 && newcaretpos != -1) {
               caret_pos = newcaretpos;
               caret_line = newcaretline;
            }
            RefreshSelection();
            drag = false;
         }
      }
   }
   if (Flags() & HASFOCUS) {
      if (ev.type == EAGLE_EVENT_KEY_CHAR) {
         EagleLog() << "Key down event detected in SelectText::HandleEvent - key " <<
                        ev.keyboard.keycode << " was pressed." << std::endl;

         if (ev.keyboard.keycode == EAGLE_KEY_DOWN || ev.keyboard.keycode == EAGLE_KEY_UP) {
            if (input_key_held(EAGLE_KEY_LSHIFT) || input_key_held(EAGLE_KEY_RSHIFT)) {
               MoveCaretUpOrDown(ev.keyboard.keycode , true);
            }
            else {
               MoveCaretUpOrDown(ev.keyboard.keycode , false);
            }
         }
         if (ev.keyboard.keycode == EAGLE_KEY_LEFT || ev.keyboard.keycode == EAGLE_KEY_RIGHT) {
            if (input_key_held(EAGLE_KEY_LSHIFT) || input_key_held(EAGLE_KEY_RSHIFT)) {
               MoveCaretLeftOrRight(ev.keyboard.keycode , true);
            }
            else {
               MoveCaretLeftOrRight(ev.keyboard.keycode , false);
            }
         }

         if (ev.keyboard.keycode == EAGLE_KEY_A) {
            if (input_key_held(EAGLE_KEY_LCTRL) || input_key_held(EAGLE_KEY_RCTRL)) {
               /// CTRL + A : select all
               select_pos = 0;
               select_line = 0;
               int endline = (int)lines.size() - 1;
               caret_pos = lines[endline].size();
               caret_line = endline;
               RefreshSelection();
            }
         }
         
         if (ev.keyboard.keycode == EAGLE_KEY_C) {
            if (input_key_held(EAGLE_KEY_LCTRL) || input_key_held(EAGLE_KEY_RCTRL)) {
               /// CTRL + C - copy to clipboard
               EAGLE_ASSERT(eagle_system);
               EAGLE_ASSERT(eagle_system->GetSystemClipboard());
               eagle_system->GetSystemClipboard()->CopyToClipboard(selected_text);
               EagleLog() << "Copying \"" << selected_text << "\" to clipboard." << std::endl;
               QueueUserMessage(this , TOPIC_TEXT_WIDGET , TEXT_COPIED);
            }
         }
      }
   }
   return retflags;
}



int SelectText::PrivateCheckInputs() {
   return DIALOG_OKAY;
}

void SelectText::PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos) {
   EAGLE_ASSERT(text_font);
   EAGLE_ASSERT(text_font->Valid());
   if (!nlines) {return;}

///   EagleLog() << "SelectText::PrivateDisplay - begin" << std::endl;
   for (int i = 0 ; i < nlines ; ++i) {
      Rectangle r = lineareas[i];
      if (i >= select_line_start && i <= select_line_close) {
///         win->DrawRectangle(r , 3.0 , EagleColor(0,255,255));
      }
      else {
///         win->DrawRectangle(r , 3.0 , EagleColor(255,0,255));
      }
   }
   
   
   /// Draw selection background first
///   EagleLog() << StringPrintF("start = %d , close = %d , nlines = %d , left = %d , right = %d" ,
///                              select_line_start , select_line_close , nlines , select_left , select_right) << std::endl;
///   EagleLog() << StringPrintF("lines[start].size() = %d , lines[close].size() = %d" ,
///                               (int)lines[select_line_start].size() , (int)lines[select_line_close].size()) << std::endl;
   if (select_line_start > -1) {
      if (select_line_start == select_line_close) {
         DrawSelectionBackground(win , select_line_start , select_left , select_right , xpos , ypos);
      }
      else {
         for (int lnum = select_line_start ; lnum <= select_line_close ; ++lnum) {
            if (lnum == select_line_start) {
               DrawSelectionBackground(win , lnum , select_left , (int)lines[lnum].size() , xpos , ypos);
            }
            else if (lnum == select_line_close) {
               DrawSelectionBackground(win , lnum , 0 , select_right , xpos , ypos);
            }
            else {
               DrawSelectionBackground(win , lnum , 0 , (int)lines[lnum].size() , xpos , ypos);
            }
         }
      }
   }
///   EagleLog() << "SelectText::PrivateDisplay - background drawn" << std::endl;

   DrawText(win , xpos , ypos , WCols()[TXTCOL]);

   /// Draw caret
   if (caret_visible) {
      if (caret_line >= 0 && caret_pos > -1) {
         
         Rectangle r = lineareas[caret_line];
         string s = lines[caret_line].substr(0 , caret_pos);
         
         float x = r.X() + xpos + text_font->Width(s.c_str());
         float y = r.Y() + ypos;
///         float w = 3;
         float h = text_font->Height();
         
         EagleColor col = WCols()[FGCOL];
         
         win->DrawFilledRectangle(x-1,y,3.0f,h,col);
         win->DrawFilledRectangle      (x-3 , y   , 7.0f , 2.0f , col);
         win->DrawFilledRectangle      (x-3 , y+h , 7.0f , 2.0f , col);
      }
   }
   /// DIAGNOSTIC TEST
/**
   EAGLE_DEBUG (
      float lineheight = 1.2*text_font->Height();
      win->DrawTextString(text_font , StringPrintF("caret_pos = %d , caret_line = %d",caret_pos , caret_line),
                          InnerArea().BRX() , InnerArea().BRY() , EagleColor(255,127,0) , HALIGN_RIGHT , VALIGN_BOTTOM);
      win->DrawTextString(text_font , StringPrintF("select_pos = %d , select_line = %d",select_pos , select_line),
                          InnerArea().BRX() , InnerArea().BRY() - lineheight  , EagleColor(255,127,0) ,
                          HALIGN_RIGHT , VALIGN_BOTTOM);
      win->DrawTextString(text_font , StringPrintF("select_left = %d , select_right = %d",select_left , select_right),
                          InnerArea().BRX() , InnerArea().BRY() - 2*lineheight  , EagleColor(255,127,0) ,
                          HALIGN_RIGHT , VALIGN_BOTTOM);
      win->DrawTextString(text_font , StringPrintF("select_line_start = %d , select_line_close = %d",
                          select_line_start , select_line_close),
                          InnerArea().BRX() , InnerArea().BRY() - 3*lineheight  , EagleColor(255,127,0) ,
                          HALIGN_RIGHT , VALIGN_BOTTOM);
   );
//*/
   ClearRedrawFlag();
///   EagleLog() << "SelectText::PrivateDisplay - finish" << std::endl;

}



int SelectText::PrivateUpdate(double tsec) {
   caret_time += tsec;
   if (caret_time >= caret_blink_time) {
      int numblinks = (int)(caret_time / caret_blink_time);
      if (numblinks % 2 == 1) {
         caret_visible = !caret_visible;
         SetBgRedrawFlag();
      }
      caret_time = fmod(caret_time,caret_blink_time);
   }
   return DIALOG_OKAY;
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



void SelectText::SetFocusState(bool state) {
   if (!state) {
      if (deselect_on_lost_focus) {
         select_line = -1;
         RefreshSelection();
      }
   }
}



void SelectText::SetDeselectOnLostFocus(bool deselect) {
   deselect_on_lost_focus = deselect;
}



void SelectText::Refresh() {
   BasicText::Refresh();
   RefreshSelection();
}



