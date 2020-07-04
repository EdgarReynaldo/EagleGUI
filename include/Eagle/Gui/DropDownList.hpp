



#ifndef DropDownList_HPP
#define DropDownList_HPP


#include "Eagle/Gui/Text/BasicText.hpp"
#include "Eagle/Gui/Button/BasicButton.hpp"

#include "Eagle/Gui/ListBox.hpp"
#include "Eagle/Gui/Layout/RelativeLayout.hpp"


#include <vector>




/** !## The first two slots of this layout are reserved for the text and the toggle button, anything else is fair game */
class DropDownList : public RelativeLayout , public EagleEventListener {
   
protected :
   BasicText basic_text;
   BasicButton basic_button;
   
   BasicText* our_selection_text;
   BasicButton* our_toggle_button;
   
   ListBox* our_list;
   
   std::vector<std::string> choice_strings;
   
   bool list_open;
   
   virtual void RespondToEvent(EagleEvent e , EagleThread* thread = MAIN_THREAD);
   
   void 
   
public :   

   void SetText(BasicText* text);
   void SetButton(BasicButton* btn);
   void SetList(ListBox* listbox);

   DropDownList(ListBox* listbox , BasicText* seltext = 0 , BasicButton* button = 0);
   /// LayoutBase
   virtual void PlaceWidget(WidgetBase* w , int slot);
   virtual int AddWidget(WidgetBase* w);

   virtual void RemoveWidget(WidgetBase* widget);
   virtual void ClearWidgets();

   void SetListOpen(bool open);

   std::vector<BasicButton*> ButtonsDown();
   int Choice();
   WidgetBase* WChoice();
   
   void SetTextChoices(std::vector<std::string> choices);
   void SetTextChoices(std::vector<std::string> choices) {
      choice_strings = choices;
      
   }
   
};










#endif // DropDownList_HPP
