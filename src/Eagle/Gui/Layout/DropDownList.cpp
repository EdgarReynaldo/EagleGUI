



#include "Eagle/Gui/Layout/DropDownList.hpp"
#include "Eagle/Gui/WidgetHandler.hpp"








void DropDownList::RespondToEvent(EagleEvent e , EagleThread* thread) {
   (void)thread;
   if (e.type == EAGLE_EVENT_WIDGET) {
      if (e.widget.from == our_toggle_button) {
         if (e.widget.msgs == BUTTON_TOGGLED) {
            SetListOpen(!list_open);
         }
      }
      if (e.widget.from == our_list) {
         if (e.widget.msgs == LISTBOX_SELECTION_MADE) {
            RefreshTextChoice();
            SetListOpen(false);
         }
      }
   }
}



void DropDownList::RefreshTextChoice() {
   std::string text = "INVALID";
   if (Choice() > -1) {
      text = choice_strings[Choice()];
   }
   our_selection_text->SetupText(text , our_font);
}



void DropDownList::SetText(BasicText* text) {
   if (!text) {
      text = &basic_text;
   }
   our_selection_text = text;
   RelativeLayout::PlaceWidget(our_selection_text , 1 , LayoutRectangle(0.0 , 0.0 , 0.9 , 1.0));
   RefreshTextChoice();
}



void DropDownList::SetButton(BasicButton* btn) {
   if (!btn) {
      btn = &basic_button;
   }
   btn->SetButtonType(TOGGLE_BTN);
   if (our_toggle_button) {
      StopListeningTo(our_toggle_button);
   }
   our_toggle_button = btn;
   if (our_toggle_button) {
      ListenTo(our_toggle_button);
      btn->SetButtonUpState(true);
      list_open = false;
   }
   RelativeLayout::PlaceWidget(our_toggle_button , 0 , LayoutRectangle(0.9 , 0.0 , 0.1 , 1.0));
}



void DropDownList::SetList(ListBox* listbox) {
   if (our_list) {
      StopListeningTo(our_list);
   }
   our_list = listbox;
   if (our_list) {
      ListenTo(our_list);
   }
}



DropDownList::DropDownList(ListBox* listbox , EagleFont* font , BasicText* seltext , BasicButton* button) :
      RelativeLayout("DropDownList" , "DDL"),
      EagleEventListener(),
      our_selection_text(0),
      our_toggle_button(0),
      our_list(0),
      our_font(font),
      list_open(false)
{
   Resize(2);
   SetText(seltext);
   SetButton(button);

   SetList(listbox);
}



Rectangle DropDownList::RequestWidgetArea(int slot , int newx , int newy , int newwidth , int newheight) {
   return RelativeLayout::RequestWidgetArea(slot , newx , newy , newwidth , newheight);
}



int DropDownList::AddWidget(WidgetBase* w) {
   (void)w;
   return -1;
}



void DropDownList::SetListOpen(bool open) {
   if (list_open == open) {return;}
   list_open = open;
   if (whandler) {
      whandler->GiveWidgetFocus(open?our_list:0 , true);
   }
   our_toggle_button->SetButtonState(our_toggle_button->Flags().FlagOn(HOVER) , !open);
}




std::vector<BasicButton*> DropDownList::ButtonsDown() {
   std::vector<BasicButton*> btns;
   if (!our_list) {return btns;}
   return our_list->ButtonsDown();
}



WidgetBase* DropDownList::WChoice() {
   return (our_list?our_list->WChoice():0);
}



int DropDownList::Choice() {
   return (our_list?our_list->Choice():-1);
}



std::string DropDownList::TextChoice() {
   int c = Choice();
   if (c >= 0 && c < (int)choice_strings.size()) {
      return choice_strings[c];
   }
   return "INVALID";
}



void DropDownList::SetTextChoices(std::vector<std::string> choices) {
   choice_strings = choices;
   RefreshTextChoice();
}



void DropDownList::SetChoice(int c) {
   our_list->SetChoice(c);
   RefreshTextChoice();
}



void DropDownList::SetFont(EagleFont* font) {
   our_font = font;
   RefreshTextChoice();
}



BasicButton* DropDownList::GetOurButton() {
   return our_toggle_button;
}



BasicText* DropDownList::GetOurText() {
   return our_selection_text;
}


