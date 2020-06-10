



#ifndef DropDownList_HPP
#define DropDownList_HPP


#include "Eagle/Gui/Text/BasicText.hpp"
#include "Eagle/Gui/Button/BasicButton.hpp"

#include "Eagle/Gui/ListBox.hpp"
#include "Eagle/Gui/Layout/RelativeLayout.hpp"


#include <vector>




/** !## The first two slots of this layout are reserved for the listbox and the toggle button, anything else is fair game */
class DropDownList : public RelativeLayout , public EagleEventListener {
   
   
protected :
   
   BasicText* our_selection_text;
   BasicButton* our_toggle_button;
   
   ListBox* our_list;

   
   bool list_open;
   
   
   void SetText(BasicText* text);
   void SetButton(BasicButton* btn);
   void SetList(ListBox* listbox);

public :   
   DropDownList(BasicText* seltext , BasicButton* button , ListBox* listbox);
   /// LayoutBase
   virtual void PlaceWidget(WidgetBase* w , int slot);
   virtual int AddWidget(WidgetBase* w);

   virtual void RemoveWidget(WidgetBase* widget);
   virtual void ClearWidgets();

   std::vector<BasicButton*> ButtonsDown();
   int Choice();
   WidgetBase* WChoice();
   
};













#endif // DropDownList_HPP
