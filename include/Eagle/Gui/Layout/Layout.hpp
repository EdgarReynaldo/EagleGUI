
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



class WidgetHandler;



enum LAYOUT_ATTRIBUTES {
   LAYOUT_ALLOWS_NOTHING               = 0,
   LAYOUT_ALLOWS_REPOSITION            = 1 << 0,
   LAYOUT_ALLOWS_RESIZE                = 1 << 1,
   LAYOUT_ALLOWS_RESIZE_AND_REPOSITION = 3
};


std::string PrintLayoutAttributes(LAYOUT_ATTRIBUTES attributes);


class Layout : public WidgetBase {

protected :
   
   LAYOUT_ATTRIBUTES attributes;
   
   HALIGNMENT halign;
   VALIGNMENT valign;

   bool size_fixed;

   std::vector<WidgetBase*> wchildren;
   std::map<WidgetBase* , bool> delete_map;
   
   WidgetHandler* whandler;



   int WidgetIndex(WidgetBase* widget);
   int NextFreeSlot();

   virtual void ReserveSlots(int nslots);
   void ReplaceWidget(WidgetBase* widget , int slot);

   void AdjustWidgetArea(WidgetBase* widget , int* newx , int* newy , int* newwidth , int* newheight);
///   void SetWidgetPos(WidgetBase* widget , int newx , int newy , int newwidth , int newheight);

   virtual void RepositionAllChildren()=0;
   virtual void RepositionChild(int slot)=0;


public :
   
   Layout();
   Layout(std::string name);
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


   virtual bool AcceptsFocus() {return true;}


   /// LAYOUTBASE
/**
   virtual Rectangle RequestPosition   (WidgetBase* widget , int newx , int newy);
   virtual Rectangle RequestSize       (WidgetBase* widget , int newwidth , int newheight);
   virtual Rectangle RequestArea       (WidgetBase* widget , int newx , int newy , int newwidth , int newheight);
   Rectangle         RequestArea       (WidgetBase* widget , Rectangle newarea);
//*/
//**
   /// Pass INT_MAX for a parameter if you don't care about the position or size
   /// NOTE : These two functions do NOT change the widget's area, they only return the area that the layout would give it
   virtual Rectangle RequestWidgetArea(WidgetBase* widget , int newx , int newy , int newwidth , int newheight);
   Rectangle RequestWidgetArea(WidgetBase* widget , Rectangle newarea);
//*/
   
   /// Widget may be null for PlaceWidget
   /// Both replace the widget (addwidget replaces a null widget) and call RepositionChild
   /// Override if necessary, and use ReplaceWidget in your code
   virtual bool PlaceWidget(WidgetBase* widget , int slot , bool delete_when_removed = false);/// Will free the old widget if specified previously
   virtual bool AddWidget(WidgetBase* widget , bool delete_when_removed = false);

   void EmptySlot(int slot);/// Remove a widget from the layout without freeing it
   void RemoveWidget(WidgetBase* widget);/// Remove a widget from the layout without freeing it : TODO : This doesn't work as intended
   void ClearWidgets();/// Remove all widgets from layout without freeing them
   
   void RemoveWidgetFromLayout(WidgetBase* widget);/// Optionally frees widget and places NULL in its slot

   virtual void ClearLayoutAndFreeWidgets();/// Clears all widgets and frees the ones marked for deletion, 
                                            /// call this in derived Layout destructors

   void DetachFromGui();/// Call this in Layout derived class's destructor
   

   void SetAlignment(HALIGNMENT h_align , VALIGNMENT v_align);

protected :
   friend class WidgetHandler;
   
   void SetGuiHandler(WidgetHandler* handler);// for WidgetHandlers only

public :   
   // Getters
   std::vector<WidgetBase*> WChildren();
   std::vector<WidgetBase*> Descendants();
   
   Layout* RootLayout();
   bool IsRootLayout();
   WidgetHandler* WHandler();

   int GetLayoutSize();
   
   virtual std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const;
};



/** *************************** LAYOUTS *************************** */



// dumb , grid , table , vsplitter , hsplitter , flow , border?




#endif // EagleGuiLayout_HPP

