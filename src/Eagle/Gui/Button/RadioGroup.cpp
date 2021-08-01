
/**
 *
 *         _______       ___       ____      __       _______
 *        /\  ____\    /|   \     /  __\    /\ \     /\  ____\
 *        \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_
 *         \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\
 *          \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_
 *           \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *            \/______/|/_/  \/_/  \_\_____/    \/_____/ \/______/
 *
 *
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2021+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file RadioGroup.cpp
 * @brief The implementation for the gui helper class, RadioGroup. Turns any set of buttons into a radio button.
 */


#include "Eagle/Gui/Button/RadioGroup.hpp"
#include "Eagle/Gui/Button/BasicButton.hpp"



RadioGroup::RadioGroup() :
      EagleEventListener(),
      EagleEventSource(),
      btn_group()
{
   
}



void RadioGroup::AddRadioButton(BasicButton* btn) {
   btn_group.insert(btn);
   ListenTo(btn);
}



void RadioGroup::RemoveRadioButton(BasicButton* btn) {
   if (btn_group.find(btn) != btn_group.end()) {
      btn_group.erase(btn_group.find(btn));
      StopListeningTo(btn);
   }
}



void RadioGroup::SelectButton(BasicButton* btn) {
   btn->SetButtonState(btn->Hover() , false , false);
   btn_group.erase(btn_group.find(btn));
   for (std::unordered_set<BasicButton*>::iterator it = btn_group.begin() ; it != btn_group.end() ; ++it) {
      BasicButton* b = *it;
      b->SetButtonState(b->Hover() , true , false);
   }
   btn_group.insert(btn);
}



void RadioGroup::SetRadioGroup(std::vector<BasicButton*> btns , BasicButton* active) {
   for (std::unordered_set<BasicButton*>::iterator it = btn_group.begin() ; it != btn_group.end() ; ++it) {
      StopListeningTo(*it);
   }
   btn_group.clear();
   for (unsigned int i = 0 ; i < btns.size() ; ++i) {
      EAGLE_ASSERT(btns[i]);
      if (btns[i]) {
         btn_group.insert(btns[i]);
         ListenTo(btns[i]);
      }
   }
   EAGLE_ASSERT(active && btn_group.find(active) != btn_group.end());
   if (active && btn_group.find(active) != btn_group.end()) {
      SelectButton(active);
   }
}



void RadioGroup::RespondToEvent(EagleEvent e , EagleThread* thread) {
   if (e.type == EAGLE_EVENT_WIDGET) {
      if (e.widget.topic == TOPIC_BUTTON_WIDGET && e.widget.msgs == BUTTON_TOGGLED) {
         BasicButton* btn = dynamic_cast<BasicButton*>(e.widget.from);
         if (btn) {
            if (btn_group.find(btn) != btn_group.end()) {
               /// The button is in our set, and it was toggled
               SelectButton(btn);
               e.source = this;
               EmitEvent(e , thread);
            }
         }
      }
   }
}



std::unordered_set<BasicButton*> RadioGroup::GetButtonGroup() {
   return btn_group;
}




