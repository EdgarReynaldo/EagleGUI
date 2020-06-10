



#include "Eagle/Gui/DropDownList.hpp"









void DropDownList::SetText(BasicText* text) {
   our_selection_text = text;
}



void DropDownList::SetButton(BasicButton* btn) {
   if (our_toggle_button) {
      StopListeningTo(our_toggle_button);
   }
   our_toggle_button = btn;
   if (our_toggle_button) {
      ListenTo(our_toggle_button);
      btn->SetButtonType(TOGGLE_BTN);
      btn->SetButtonUpState(true);
      list_open = false;
   }
   PlaceWidget(our_toggle_button , 1);
}



void DropDownList::SetList(ListBox* listbox) {
   if (our_list) {
      StopListeningTo(our_list);
   }
   our_list = listbox;
   if (our_list) {
      ListenTo(our_list);
   }
///   PlaceWidget(our_list , 0);
}



DropDownList::DropDownList(BasicText* seltext , BasicButton* button , ListBox* listbox) :
      RelativeLayout("DropDownList" , "DDL"),
      EagleEventListener(),
      our_selection_text(0),
      our_toggle_button(0),
      our_list(0),
      list_open(false)
{
   Resize(2);
   SetText(seltext);
   SetButton(button);

   SetList(listbox);
}



void DropDownList::PlaceWidget(WidgetBase* w , int slot) {
   if (slot != 0 && slot != 1) {
      return;
   }
   BasicButton* bb = dynamic_cast<BasicButton*>(w);
   if (bb) {
      SetButton(bb);
   }
   RelativeLayout::PlaceWidget(w,slot);
}



int DropDownList::AddWidget(WidgetBase* w) {
   BasicButton* bb = dynamic_cast<BasicButton*>(w);
   if (bb) {
      SetButton(bb);
   }
   return RelativeLayout::AddWidget(w);
}



void DropDownList::RemoveWidget(WidgetBase* widget) {
   if (!widget) {return;}
   if (widget == our_list) {
      StopListeningTo(our_list);
      SetList(0);
   }
   else if (widget == our_toggle_button) {
      StopListeningTo(our_toggle_button);
      SetButton(0);
   }
   RelativeLayout::RemoveWidget(widget);
}



void DropDownList::ClearWidgets() {
   SetList(0);
   SetButton(0);
   RelativeLayout::ClearWidgets();
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








