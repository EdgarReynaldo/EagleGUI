
/*
 *
 *     _______       ___       ____      __       _______
 *    /\  ____\    /|   \     /  __\    /\ \     /\  ____\
 *    \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_
 *     \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\
 *      \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_
 *       \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *        \/______/|/_/  \/_/  \_\_____/    \/_____/ \/______/
 *
 *
 *    EAGLE
 *    Edgar's Agile Gui Library and Extensions
 *
 *    Copyright 2009-2018+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */



#ifndef EagleGuiLayout_HPP
#define EagleGuiLayout_HPP


#include "Eagle/Gui/WidgetBase.hpp"
#include "Eagle/Gui/Layout/LayoutRectangle.hpp"
#include "Eagle/Gui/Alignment.hpp"


#include <iostream>
#include <string>
#include <climits>



enum LAYOUT_ATTRIBUTES {
   LAYOUT_ALLOWS_NOTHING               = 0,
   LAYOUT_ALLOWS_REPOSITION            = 1 << 0,
   LAYOUT_ALLOWS_RESIZE                = 1 << 1,
   LAYOUT_ALLOWS_RESIZE_AND_REPOSITION = 3
};


std::string PrintLayoutAttributes(LAYOUT_ATTRIBUTES attributes);


class Layout : public WidgetBase {

protected :
   
   std::vector<SHAREDWIDGET> wchildren;

   LAYOUT_ATTRIBUTES attributes;
   
   HALIGNMENT halign;
   VALIGNMENT valign;



   /// Utility
   int WidgetIndex(const WidgetBase* widget) const;
   WidgetBase* GetWidget(int slot);
   const WidgetBase* GetWidget(int slot) const {return GetWidget(slot);}
   int NextFreeSlot();

   /// Override and call if you need special storage
   virtual void ReserveSlots(int nslots);

   /// All widget placement functions use ReplaceWidget
   void ReplaceWidget(SHAREDWIDGET widget , int slot);

   /// Adjusts the widget area to obey minimum dimensions and to obey layout attributes and widget flags
   void AdjustWidgetArea(const WidgetBase* widget , int* newx , int* newy , int* newwidth , int* newheight) const;

   /// Called automatically on reposition
   void RepositionAllChildren();
   void RepositionChild(int slot);



	/// WIDGETBASE
   virtual void OnAreaChanged();

   virtual int PrivateHandleInputEvent(EagleEvent e);
   virtual int PrivateUpdate(double dt);
   virtual void PrivateDisplay(EagleGraphicsContext* win , int x , int y);

   
   
public :
   
   Layout(std::string objclass , std::string objname);
   virtual ~Layout();
   



   /// Layout

   /// Pass INT_MAX for a parameter if you don't care about the position or size
   /// NOTE : These functions do NOT change the widget's area, they only return the area that the layout would give it
   virtual Rectangle RequestWidgetArea(int widget_slot , int newx , int newy , int newwidth , int newheight) const;
   
   Rectangle RequestWidgetArea(const WidgetBase* widget , int newx , int newy , int newwidth , int newheight) const;

   Rectangle RequestWidgetArea(int widget_slot , Rectangle newarea) const;

   Rectangle RequestWidgetArea(const WidgetBase* widget , Rectangle newarea) const;

   Rectangle RequestWidgetArea(const WidgetBase* widget) const;
   
   
   void Resize(unsigned int nsize);

   /// Adding widgets to layout
   
   /// Widget may be null for PlaceWidget
   /// Both replace the widget (Addwidget replaces a null widget) and call RepositionChild

   virtual void PlaceWidget(SHAREDWIDGET w , int slot);
   virtual int AddWidget(SHAREDWIDGET w);/// Adds the widget to the next free slot or creates one if necessary, returns slot used

   /// Removal of widgets

   void EmptySlot(int slot);/// Remove a widget from the layout

   void RemoveWidget(WidgetBase* widget);/// Remove a widget from the layout

   void ClearWidgets();/// Remove all widgets from layout
   
/// TODO : Why is this public again?
   void RemoveWidgetFromLayout(WidgetBase* widget);/// Stops tracking widget - talks to WidgetHandler

   void DetachFromGui();/// Call this in Layout derived class's destructor
   

   virtual void SetAlignment(HALIGNMENT h_align , VALIGNMENT v_align);

   
   
   virtual void SetWChildren(std::vector<SHAREDWIDGET> new_children);

   // Getters
   std::vector<SHAREDWIDGET> WChildren() const ;
   std::vector<SHAREDWIDGET> Descendants() const ;
   
   Layout* RootLayout();
   const Layout* RootLayout() const;
   
   bool IsRootLayout() const;
   
   int GetLayoutSize() const ;
   
   
   virtual bool AcceptsFocus() {return false;}

   virtual std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const;
};




#endif // EagleGuiLayout_HPP







