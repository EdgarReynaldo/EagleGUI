


#include "Eagle/Gui/Text/EditText.hpp"
#include "Eagle/StringWork.hpp"

#include "Eagle/InputHandler.hpp"
#include "Eagle/GraphicsContext.hpp"
#include "Eagle/System.hpp"




int EditText::PrivateHandleEvent(EagleEvent e) {
   
   int ret = SelectText::PrivateHandleEvent(e);
   if (ret & DIALOG_INPUT_USED) {
      return ret;
   }
   
   if (e.type == EAGLE_EVENT_KEY_DOWN) {
      if (e.keyboard.keycode == EAGLE_KEY_INSERT) {
         /// Toggle insert mode
         insert_mode = !insert_mode;
         SetRedrawFlag();
         return DIALOG_INPUT_USED;
      }
   }
   else if (e.type == EAGLE_EVENT_KEY_CHAR) {
      /// Make sure it is an ascii character
      std::string newstr = "";
      if (e.keyboard.unicode <= 27 || e.keyboard.unicode == 127) {
         /// Control key, or invisible
         if (e.keyboard.keycode == EAGLE_KEY_TAB) {
            newstr = "";
            for (unsigned int i = 0 ; i < NTABSPACES ; ++i) {
               newstr += " ";
            }
            /// TODO : Follow indentation guides
         }
         else if (e.keyboard.keycode == EAGLE_KEY_ENTER) {
            newstr = '\n';
         }
         else if (e.keyboard.keycode == EAGLE_KEY_DELETE) {
            if (select_line_start > -1) {
               DeleteSelection(select_line_start , select_line_close , select_left , select_right);
            }
            else {
               if (caret_pos < lines[caret_line].size()) {
                  DeleteSelection(caret_line , caret_line , caret_pos , caret_pos + 1);
               }
            }
            Refresh();
            ResetSelection();
            return DIALOG_INPUT_USED;
         }
         else if (e.keyboard.keycode == EAGLE_KEY_BACKSPACE) {
            if (select_line_start > -1) {
               DeleteSelection(select_line_start , select_line_close , select_left , select_right);
            }
            else {
               if (caret_pos > 0) {
                  DeleteSelection(caret_line , caret_line , caret_pos - 1 , caret_pos);
                  
               }
               else if (caret_pos == 0) {
                  if (caret_line > 0) {
                     DeleteSelection(caret_line - 1 , caret_line , lines[caret_line-1].size() - 1 , 0);
                  }
               }
            }
            Refresh();
            ResetSelection();
            return DIALOG_INPUT_USED;
         }
         else if (e.keyboard.keycode == EAGLE_KEY_V && input_key_held(EAGLE_KEY_ANY_CTRL)) {
            /// Paste action

            /// TODO : This won't work for stand alone widgets - it depends on the root gui's drawing window
            EagleGraphicsContext* win = GetDrawWindow();
            EAGLE_ASSERT(win);

            EagleSystem* sys = win->GetSystem();
            EAGLE_ASSERT(sys);

            EagleClipboard* cb = sys->GetSystemClipboard();
            EAGLE_ASSERT(cb);

            newstr = cb->GetClipboardString();
         }
      }
      else if (e.keyboard.unicode < 127) {
         newstr.push_back((char)e.keyboard.unicode);
      }
      
      if (newstr.size()) {
         if (Overwrite()) {
            /// Overwrite
            if (select_line_start > -1) {
               Overwrite(newstr , select_line_start , select_line_close , select_left , select_right);
            }
            else {
               if (caret_pos < lines[caret_line].size()) {
                  Overwrite(newstr , caret_line , caret_line , caret_pos , caret_pos + 1);
               } else {
                  if (caret_line == lines.size() - 1) {
                     Insert(newstr , caret_line , caret_pos);
                  }
               }
            }
         }
         else {
            /// Insert mode
            Insert(newstr , caret_line , caret_pos);
         }
         ResetSelection();
      }
      return DIALOG_INPUT_USED;
   }
   return DIALOG_OKAY;
}



int EditText::PrivateCheckInputs() {
   return SelectText::PrivateCheckInputs();
}



void EditText::PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos) {
   SelectText::PrivateDisplay(win , xpos , ypos - (int)pixscroll);
}



int EditText::PrivateUpdate(double tsec) {
   return SelectText::PrivateUpdate(tsec);
}



void EditText::OnAreaChanged() {
   SelectText::OnAreaChanged();
}



void EditText::OnFlagChanged(WIDGET_FLAGS f , bool on) {
   SelectText::OnFlagChanged(f , on);
}



void EditText::DrawCaret(EagleGraphicsContext* win , int xpos , int ypos) {
   if (insert_mode) {
      SelectText::DrawCaret(win , xpos , ypos);
   }
   else {
      DrawOverwriteCaret(win , xpos , ypos);
   }
}



void EditText::DrawOverwriteCaret(EagleGraphicsContext* win , int xpos , int ypos) {
   if (caret_visible) {
      if (caret_line >= 0 && caret_pos > -1) {
         Rectangle r(textx , texty , 1 , 1);
         std::string s = "";
         if (lines.size()) {
            r = lineareas[caret_line];
            s = lines[caret_line].substr(0 , caret_pos);
         }
         
         float x = r.X() + xpos + text_font->Width(s.c_str());
         float y = r.Y() + ypos;
         float w = text_font->Width(" ");
         float h = text_font->Height();
         
         EagleColor col = GetColor(FGCOL);
         
         win->DrawFilledRectangle(x - 1 , y + h + 1,
                                  w + 2 , 3.0f , col);
      }
   }
}



void EditText::DeleteSelection(int selLineStart , int selLineStop , int selLeft , int selRight) {
   std::string::iterator itLeft = text.begin();
   std::string::iterator itRight = text.begin();
   if (GetSelectionIterators(text , &itLeft , &itRight , selLineStart , selLineStop , selLeft , selRight)) {
      text.erase(itLeft , itRight);
      caret_line = selLineStart;
      caret_pos = selLeft;
      select_line = caret_line;
      select_pos = caret_pos;
      Refresh();
   }
}



void EditText::Insert(std::string instr , int caretLine , int caretPos) {
   std::string::iterator it = text.begin();
   
   GetPositionIterator(text , &it , caretLine , caretPos);
   
   text.insert(it , instr.begin() , instr.end());
   
   int line = 0;
   int new_caret_pos = caretPos;
   while (line < caretLine) {
      new_caret_pos += lines[line].size();
      ++line;
   }
   Refresh();
   MoveCaretAbsolute(new_caret_pos + instr.size());
}



void EditText::Overwrite(std::string instr , int selLineStart , int selLineStop , int selLeft , int selRight) {
   if (selLineStart != selLineStop || selLeft != selRight) {
      DeleteSelection(selLineStart , selLineStop , selLeft , selRight);
   }
   Insert(instr , selLineStart , selLeft);///caret_line , caret_pos);
   Refresh();
   /// TODO : Move the caret to the end of the overwrite
   
   int SZ = instr.size();
   int CNT = CountNewlines(instr);
   MoveCaretRelative(selLineStart + CNT , selLeft + SZ);
}



EditText::EditText(std::string classname , std::string objname) :
      SelectText(classname , objname),
      insert_mode(true),
      pixscroll(0),
      NTABSPACES(3)
{}



void EditText::SetTabSpacing(unsigned int ntabspaces) {
   NTABSPACES = ntabspaces;
}



bool EditText::Insert() {
   return insert_mode;
}



bool EditText::Overwrite() {
   return !insert_mode;
}



