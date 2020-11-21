



#include "Eagle/Gui/Layout/DropDownList.hpp"
#include "Eagle/Gui/WidgetHandler.hpp"








void DropDownList::RespondToEvent(EagleEvent e , EagleThread* thread) {
   (void)thread;
   if (e.type == EAGLE_EVENT_WIDGET) {
      if (e.widget.from == our_toggle_button) {
         if (e.widget.msgs == BUTTON_TOGGLED) {
            SetPopupOpen(!popup_open);
         }
      }
      if (e.widget.topic == TOPIC_LISTBOX && e.widget.msgs == LISTBOX_SELECTION_MADE) {
         ListBox* lb = dynamic_cast<ListBox*>(e.widget.from);
         if (lb) {
            our_choice = lb->Choice();
            RefreshTextChoice();
            SetPopupOpen(false);
         }
      }
   }
}



void DropDownList::RefreshTextChoice() {
   std::string text = "INVALID";
   if (Choice() > -1) {
      text = choice_strings[our_choice];
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
      btn->SetButtonState(btn->Flags().FlagOn(HOVER) , true , false);
      SetPopupOpen(false);
   }
   RelativeLayout::PlaceWidget(our_toggle_button , 0 , LayoutRectangle(0.9 , 0.0 , 0.1 , 1.0));
}



void DropDownList::SetPopup(WidgetBase* widget) {
   if (our_popup) {
      StopListeningTo(our_popup);
   }
   our_popup = widget;
   if (our_popup) {
      ListenTo(our_popup);
   }
}



DropDownList::DropDownList(WidgetBase* widget , EagleFont* font , BasicText* seltext , BasicButton* button) :
      RelativeLayout("DropDownList" , "DDL"),
      EagleEventListener(),
      our_selection_text(0),
      our_toggle_button(0),
      our_popup(0),
      choice_strings(),
      our_choice(-1),
      our_font(font),
      popup_open(true)
{
   Resize(2);
   SetPopup(widget);
   SetText(seltext);
   SetButton(button);
}



Rectangle DropDownList::RequestWidgetArea(int slot , int newx , int newy , int newwidth , int newheight) {
   return RelativeLayout::RequestWidgetArea(slot , newx , newy , newwidth , newheight);
}



int DropDownList::AddWidget(WidgetBase* w) {
   (void)w;
   return -1;
}



void DropDownList::SetPopupOpen(bool open) {
   if (popup_open == open) {return;}
   popup_open = open;
   if (!our_popup) {return;}
   if (popup_open) {
      our_popup->ShowAndEnable();
      if (whandler) {
         whandler->GiveWidgetFocus(popup_open?our_popup:0 , true);
      }
   } else {
      our_toggle_button->SetButtonState(our_toggle_button->Flags().FlagOn(HOVER) , true , false);
      our_popup->HideAndDisable();
   }
}



std::string DropDownList::TextChoice() {
   int c = our_choice;
   if (c >= 0 && c < (int)choice_strings.size()) {
      return choice_strings[c];
   }
   return "INVALID";
}



void DropDownList::SetTextChoices(std::vector<std::string> choices) {
   choice_strings = choices;
   SetChoice(0);
}



void DropDownList::SetChoice(int c) {
   if (c < 0) {
      c = 0;
   }
   if (c > (int)choice_strings.size() - 1) {
      c = (int)choice_strings.size() - 1;
   }
   our_choice = c;
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



std::ostream& DropDownList::DescribeTo(std::ostream& os , Indenter indent) const {
   our_toggle_button->DescribeTo(os,indent);
   our_selection_text->DescribeTo(os,indent);
   RelativeLayout::DescribeTo(os);
   return os;
}



