



#include "Eagle/Gui/Layout/ListBox.hpp"


const unsigned int TOPIC_LISTBOX = NextFreeTopicId();


REGISTERED_WIDGET_MESSAGE(TOPIC_LISTBOX , LISTBOX_SELECTION_MADE);



ListBox::ListBox() :
      ClassicMenuLayout("ListBox"),
      EagleEventListener(),
      choice(-1)
{}



ListBox::~ListBox() {
   DetachFromGui();
   ClearWidgets();
}



void ListBox::RespondToEvent(EagleEvent e , EagleThread* thread) {
   if (e.type == EAGLE_EVENT_WIDGET) {
      if (e.widget.topic == TOPIC_BUTTON_WIDGET) {
         if (e.widget.msgs == BUTTON_CLICKED) {
            choice = -1;
            WidgetBase* wchoice = e.widget.from;
            for (unsigned int i = 0 ; i < wchildren.size() ; ++i) {
               if (wchoice == wchildren[i]) {
                  choice = i;
               }
            }
            EagleEvent ee = e;
            ee.widget.from = this;
            ee.widget.topic = TOPIC_LISTBOX;
            ee.widget.msgs = LISTBOX_SELECTION_MADE;
            EmitEvent(ee , thread);
         }
      }
   }
}



void ListBox::OnFlagChanged(WIDGET_FLAGS f , bool on) {
   ClassicMenuLayout::OnFlagChanged(f,on);
}



void ListBox::PlaceWidget(WidgetBase* w , int slot) {
   if (wchildren[slot]) {
      StopListeningTo(wchildren[slot]);
   }
   if (w) {
      ListenTo(w);
   }
   ClassicMenuLayout::PlaceWidget(w,slot);
}



int ListBox::AddWidget(WidgetBase* w) {
   ListenTo(w);
   return ClassicMenuLayout::AddWidget(w);
}



void ListBox::RemoveWidget(WidgetBase* widget) {
   StopListeningTo(widget);
   ClassicMenuLayout::RemoveWidget(widget);
}



void ListBox::ClearWidgets() {
   StopListening();
   ClassicMenuLayout::ClearWidgets();
}



void ListBox::SetChoice(int c) {
   if (c < 0) {c = -1;}
   if (c >= (int)wchildren.size()) {c = wchildren.size() - 1;}
   choice = c;
   EagleEvent e;
   e.type = EAGLE_EVENT_WIDGET;
   e.widget.msgs = LISTBOX_SELECTION_MADE;
   e.widget.topic = TOPIC_LISTBOX;
   e.widget.from = this;
   EmitEvent(e);
}



std::vector<BasicButton*> ListBox::ButtonsDown() {
   std::vector<BasicButton*> buttonsdown;
   for (unsigned int i = 0 ; i < wchildren.size() ; ++i) {
      WidgetBase* w = wchildren[i];
      if (w) {
         BasicButton* bb = dynamic_cast<BasicButton*>(w);
         if (bb) {
            if (!bb->Up()) {
               buttonsdown.push_back(bb);
            }
         }
      }
   }
   return buttonsdown;
}



int ListBox::Choice() {
   return choice;
}




