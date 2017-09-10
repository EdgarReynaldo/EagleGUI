
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
 *    Copyright 2009-2016+ by Edgar Reynaldo
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


class WidgetHandler;



enum LAYOUT_ATTRIBUTES {
   LAYOUT_ALLOWS_NOTHING               = 0,
   LAYOUT_ALLOWS_REPOSITION            = 1 << 0,
   LAYOUT_ALLOWS_RESIZE                = 1 << 1,
   LAYOUT_ALLOWS_RESIZE_AND_REPOSITION = 3
};


std::string PrintLayoutAttributes(LAYOUT_ATTRIBUTES attributes);


class Layout : public WidgetBase {

   friend class Decorator;

protected :
   
   LAYOUT_ATTRIBUTES attributes;
   
   HALIGNMENT halign;
   VALIGNMENT valign;

   std::vector<WidgetBase*> wchildren;

   WidgetHandler* whandler;



   int WidgetIndex(WidgetBase* widget) const;
   WidgetBase* GetWidget(int slot) const;
   int NextFreeSlot();


   virtual void ReserveSlots(int nslots);

   void ReplaceWidget(WidgetBase* widget , int slot);

   void AdjustWidgetArea(const WidgetBase* widget , int* newx , int* newy , int* newwidth , int* newheight) const;

   void RepositionAllChildren();
   void RepositionChild(int slot);


public :
   
   Layout(std::string objclass , std::string objname);
   virtual ~Layout();
   
	/// WIDGETBASE
	
   virtual int PrivateHandleInputEvent(EagleEvent e);
   virtual int PrivateUpdate(double dt);
   virtual void PrivateDisplay(EagleGraphicsContext* win , int x , int y);

   virtual void SetWidgetArea(int xpos , int ypos , int width , int height , bool notify_layout = true);

	/// Changes position and outer area!!!
	virtual void SetMarginsExpandFromInner(int left , int right , int top , int bottom);

	/// Make room in outer area for inner area first!!!
	virtual void SetMarginsContractFromOuter(int left , int right , int top , int bottom);


   virtual bool AcceptsFocus() {return false;}


   /// LAYOUTBASE

   /// Pass INT_MAX for a parameter if you don't care about the position or size
   /// NOTE : These functions do NOT change the widget's area, they only return the area that the layout would give it
   virtual Rectangle RequestWidgetArea(int widget_slot , int newx , int newy , int newwidth , int newheight) const;
   
   Rectangle RequestWidgetArea(WidgetBase* widget , int newx , int newy , int newwidth , int newheight) const;

   Rectangle RequestWidgetArea(int widget_slot , Rectangle newarea) const;

   Rectangle RequestWidgetArea(WidgetBase* widget , Rectangle newarea) const;

   Rectangle RequestWidgetArea(WidgetBase* widget) const;
   
   
   void Resize(unsigned int nsize);

   /// Widget may be null for PlaceWidget
   /// Both replace the widget (addwidget replaces a null widget) and call RepositionChild
   virtual void PlaceWidget(WidgetBase* widget , int slot);
   virtual int AddWidget(WidgetBase* widget);/// Adds the widget to the next free slot or creates one if necessary, returns slot used

   void EmptySlot(int slot);/// Remove a widget from the layout
   void RemoveWidget(WidgetBase* widget);/// Remove a widget from the layout
   void ClearWidgets();/// Remove all widgets from layout
   
   void RemoveWidgetFromLayout(WidgetBase* widget);/// Stops tracking widget - talks to WidgetHandler

   void DetachFromGui();/// Call this in Layout derived class's destructor
   

   virtual void SetAlignment(HALIGNMENT h_align , VALIGNMENT v_align);

protected :
   friend class WidgetHandler;
   
   void SetGuiHandler(WidgetHandler* handler);// for WidgetHandlers only

public :   
   // Getters
   std::vector<WidgetBase*> WChildren() const ;
   std::vector<WidgetBase*> Descendants() const ;
   
   Layout* RootLayout();
   const Layout* RootLayout() const ;
   bool IsRootLayout() const ;
   WidgetHandler* WHandler() const ;

   int GetLayoutSize() const ;
   
   virtual std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const;
};



/** *************************** LAYOUTS *************************** */



// dumb , grid , table , vsplitter , hsplitter , flow , border?




#endif // EagleGuiLayout_HPP

