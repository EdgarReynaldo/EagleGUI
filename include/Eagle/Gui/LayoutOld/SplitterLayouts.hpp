
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

#ifndef SplitterLayouts_HPP
#define SplitterLayouts_HPP

#include "Eagle/Area.hpp"
#include "Eagle/Gui/WidgetColorset.hpp"

#include "Eagle/Gui/Layout/Layout.hpp"


enum SPLITTER_TYPE {
   SPLITTER_VERTICAL = 0,// splitter is vertical (left and right areas)
   SPLITTER_HORIZONTAL = 1// splitter is horizontal (top and bottom areas)
};

typedef void (*SPLITTER_DRAW_FUNC) (EagleGraphicsContext* window , Rectangle divider_rect , bool horizontal , const WidgetColorset& colors);



class TwoWaySplitter : public Layout {
   
protected :
   SPLITTER_TYPE splitter_type;
   SPLITTER_DRAW_FUNC draw_func;
   
   int divider_size;
   int divider_position;
   float divider_percent;
   
   bool drag;
   bool olddrag;
   int dragxstart;
   int dragystart;
   int divposstart;
   bool oldhover;

///   MousePointerManager* mpman;
   EagleGraphicsContext* mouse_window;
   int owned_pointer_count;
   
   

   virtual void ReserveSlots(int nslots);

public :
   Rectangle GetHandleArea();

   TwoWaySplitter(SPLITTER_TYPE stype , EagleGraphicsContext* mouse_window = 0 , std::string objname = "Nemo");
   
   virtual ~TwoWaySplitter();


   /// WidgetBase

protected :
   virtual int PrivateHandleEvent(EagleEvent e);
   virtual int PrivateCheckInputs();
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);

   virtual void OnAreaChanged();

public :   

   virtual int AbsMinWidth();
   virtual int AbsMinHeight();

   /// LayoutBase

   virtual void PlaceWidget(SHAREDWIDGET widget , int slot);
   virtual int AddWidget(SHAREDWIDGET widget);

   virtual Rectangle RequestWidgetArea(int widget_slot , int newx , int newy , int newwidth , int newheight) const;

   
   /// Member functions
protected:
   void SetDividerPosActual(int divpos);
public :
   void SetDividerSize(int divsize);
   void SetDividerPos(int divpos);
   void SetDividerPercent(float percent);

   void SetMouseWindow(EagleGraphicsContext* window);

};



/*
class ThreeWaySplitter {

private :
   SPLITTER_TYPE splitter_type;
   
   SHAREDWIDGET first;
   SHAREDWIDGET second;
   SHAREDWIDGET third;


protected :
   virtual void RepositionAllChildren()=0;
   virtual void RepositionChild(int slot)=0;
   
public :
   
   
};
*/


#endif // SplitterLayouts_HPP


