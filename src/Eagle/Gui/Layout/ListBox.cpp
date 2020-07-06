



#include "Eagle/Gui/ListBox.hpp"


const unsigned int TOPIC_LISTBOX = NextFreeTopicId();


REGISTERED_WIDGET_MESSAGE(TOPIC_LISTBOX , LISTBOX_SELECTION_MADE);



ListBox::ListBox() :
      ClassicMenuLayout("ListBox"),
      EagleEventListener(),
      choice(-1),
      wchoice(0)
{}



ListBox::~ListBox() {
   DetachFromGui();
}



void ListBox::RespondToEvent(EagleEvent e , EagleThread* thread) {
   if (e.type == EAGLE_EVENT_WIDGET) {
      if (e.widget.topic == TOPIC_BUTTON_WIDGET) {
         if (e.widget.msgs == BUTTON_CLICKED) {
            wchoice = e.widget.from;
            for (unsigned int i = 0 ; i < wchildren.size() ; ++i) {
               if (wchoice == wchildren[i]) {
                  choice = i;
               }
            }
         }
         else if (e.widget.msgs == BUTTON_TOGGLED) {
            choice = -1;
         }
         EagleEvent ee = e;
         ee.widget.from = this;
         ee.widget.topic = TOPIC_LISTBOX;
         ee.widget.msgs = LISTBOX_SELECTION_MADE;
         EmitEvent(ee , thread);
      }
   }
}



void ListBox::OnFlagChanged(WIDGET_FLAGS f , bool on) {
   if (f & VISIBLE) {
      for (unsigned int i = 0 ; i < wchildren.size() ; ++i) {
         WidgetBase* w = wchildren[i];
         if (w) {
            w->SetVisibleState(on);
         }
      }
   }
   if (f & ENABLED) {
      for (unsigned int i = 0 ; i < wchildren.size() ; ++i) {
         WidgetBase* w = wchildren[i];
         if (w) {
            w->SetEnabledState(on);
         }
      }
   }
   /// If this list box loses focus, it should disappear
   if (f & HASFOCUS) {
      for (unsigned int i = 0 ; i < wchildren.size() ; ++i) {
         if (wchildren[i]) {
            WidgetBase* w = wchildren[i];
            w->SetFocusState(on);
            w->SetEnabledState(on);
            w->SetVisibleState(on);
         }
      }
   }
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



WidgetBase* ListBox::WChoice() {
   return wchoice;
}





