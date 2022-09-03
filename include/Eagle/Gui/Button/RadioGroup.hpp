
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
 *
 *
 *
 * @file RadioButton.hpp
 * @brief Generic class for radio buttons
 * 
 */



#ifndef RadioGroup_HPP
#define RadioGroup_HPP



#include "Eagle/Events.hpp"



#include <unordered_set>
#include <vector>

class BasicButton;



class RadioGroup : public EagleEventListener , public EagleEventSource {
   
protected :
   std::unordered_set<BasicButton*> btn_group;
   
public :

   RadioGroup();
   
   void Clear();

   void AddRadioButton(BasicButton* btn);///< Add a radio button to the group
   void RemoveRadioButton(BasicButton* btn);///< Remove a radio button from the group

   void SelectButton(BasicButton* btn);///< Select a button to set to down, automatically deselects other buttons

   void SetRadioGroup(std::vector<BasicButton*> btns , BasicButton* active);
   
   virtual void RespondToEvent(EagleEvent e , EagleThread* thread = MAIN_THREAD);///< For handling widget events from our event sources

   std::unordered_set<BasicButton*> GetButtonGroup();///< Get the set of buttons in this group
};




#endif // RadioGroup_HPP

