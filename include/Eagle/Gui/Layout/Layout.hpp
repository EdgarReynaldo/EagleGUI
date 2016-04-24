
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
 *    Copyright 2009-2014+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */



#ifndef EagleGuiLayout_HPP
#define EagleGuiLayout_HPP


#include "Eagle/Gui/WidgetBase.hpp"
#include "Eagle/Gui/Layout/LayoutRectangle.hpp"



enum LAYOUT_HALIGN {
   HALIGN_LEFT = 0,
   HALIGN_CENTER = 1,
   HALIGN_RIGHT = 2
};

enum LAYOUT_VALIGN {
   VALIGN_TOP = 0,
   VALIGN_CENTER = 1,
   VALIGN_BOTTOM = 2
};


class WidgetHandler;



enum LAYOUT_ATTRIBUTES {
   LAYOUT_ALLOWS_NOTHING               = 0,
   LAYOUT_ALLOWS_REPOSITION            = 1 << 0,
   LAYOUT_ALLOWS_RESIZE                = 1 << 1,
   LAYOUT_ALLOWS_RESIZE_AND_REPOSITION = 3
};




class Layout : public WidgetBase {

protected :
   
   LAYOUT_ATTRIBUTES attributes;
   
   LAYOUT_HALIGN halign;
   LAYOUT_VALIGN valign;

   bool size_fixed;

   std::vector<WidgetBase*> wchildren;
   std::map<WidgetBase* , bool> delete_map;
   
   WidgetHandler* whandler;



   int WidgetIndex(WidgetBase* widget);
   int NextFreeSlot();

   virtual void ReserveSlots(int nslots);
   void RemoveWidgetFromLayout(WidgetBase* widget);
   void ReplaceWidget(WidgetBase* widget , int slot);

   void AdjustWidgetArea(WidgetBase* widget , int* newx , int* newy , int* newwidth , int* newheight);
   void SetWidgetPos(WidgetBase* widget , int newx , int newy , int newwidth , int newheight);

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


   virtual void SetDrawPos(int xpos , int ypos);
   virtual void SetDrawDimensions(int width , int height);
   virtual void SetArea(int xpos , int ypos , int width , int height);
   void SetArea(const Rectangle& r);
   
   virtual bool AcceptsFocus() {return true;}


   /// LAYOUTBASE
/*
   virtual Rectangle RequestPosition   (WidgetBase* widget , int newx , int newy);
   virtual Rectangle RequestSize       (WidgetBase* widget , int newwidth , int newheight);
   virtual Rectangle RequestArea       (WidgetBase* widget , int newx , int newy , int newwidth , int newheight);
   Rectangle         RequestArea       (WidgetBase* widget , Rectangle newarea);
*/

   /// Pass INT_MAX for a parameter if you don't care about the position or size
   virtual Rectangle RequestWidgetArea(WidgetBase* widget , int newx , int newy , int newwidth , int newheight);
   Rectangle RequestWidgetArea(WidgetBase* widget , Rectangle newarea);

   // Widget may be null for PlaceWidget
   // Both replace the widget (addwidget replaces a null widget) and call RepositionChild
   // Override if necessary, and use ReplaceWidget in your code
   virtual bool PlaceWidget(WidgetBase* widget , int slot , bool delete_when_removed = false);
   virtual bool AddWidget(WidgetBase* widget , bool delete_when_removed = false);

   void EmptySlot(int slot);
   void RemoveWidget(WidgetBase* widget);
   void ClearLayout();



   void SetAlignment(LAYOUT_HALIGN h_align , LAYOUT_VALIGN v_align);

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
   
};



/** *************************** LAYOUTS *************************** */



// dumb , grid , table , vsplitter , hsplitter , flow , border?




#endif // EagleGuiLayout_HPP

