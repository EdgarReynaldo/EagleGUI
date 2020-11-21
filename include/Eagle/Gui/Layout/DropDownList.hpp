
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
 * @file 
 * @brief
 * 
 * 
 * 
 */

#ifndef DropDownList_HPP
#define DropDownList_HPP



#include <vector>

#include "Eagle/Gui/Text/BasicText.hpp"
#include "Eagle/Gui/Button/BasicButton.hpp"
#include "Eagle/Gui/Layout/ListBox.hpp"
#include "Eagle/Gui/Layout/RelativeLayout.hpp"



/** !## The first two slots of this layout are reserved for the text and the toggle button, anything else is fair game */
class DropDownList : public RelativeLayout , public EagleEventListener {
   
protected :
   BasicText basic_text;
   BasicButton basic_button;
   
   BasicText* our_selection_text;
   BasicButton* our_toggle_button;
   
   WidgetBase* our_popup;
   
   std::vector<std::string> choice_strings;
   
   int our_choice;
   
   EagleFont* our_font;
   
   bool popup_open;
   


   /// EagleEventListener
   virtual void RespondToEvent(EagleEvent e , EagleThread* thread = MAIN_THREAD);
   
   void RefreshTextChoice();
   
   
public :   

   DropDownList(WidgetBase* widget , EagleFont* font , BasicText* seltext = 0 , BasicButton* button = 0);


   /// LayoutBase
   Rectangle RequestWidgetArea(int slot , int newx , int newy , int newwidth , int newheight);

   int AddWidget(WidgetBase* w);///< Does nothing
   
   /// Members
   
   void SetText(BasicText* text);
   void SetButton(BasicButton* btn);
   void SetPopup(WidgetBase* widget);

   void SetPopupOpen(bool open);

   int Choice() {return our_choice;}
   std::string TextChoice();

   void SetTextChoices(std::vector<std::string> choices);
   void SetChoice(int c);
   void SetFont(EagleFont* font);
   
   BasicButton* GetOurButton();
   BasicText* GetOurText();
   
   
   
   
   virtual std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const ;
};










#endif // DropDownList_HPP
