
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

#ifndef SplitterLayouts_HPP
#define SplitterLayouts_HPP

#include "Eagle/Area.hpp"
#include "Eagle/Gui/WidgetColorset.hpp"

#include "Eagle/Gui/Layout/Layout.hpp"


enum SPLITTER_TYPE {
   SPLITTER_VERTICAL = 0,// splitter is vertical (left and right areas)
   SPLITTER_HORIZONTAL = 1// splitter is horizontal (top and bottom areas)
};

typedef void (*SPLITTER_DRAW_FUNC) (EagleGraphicsContext* window , Rectangle divider_rect , SPLITTER_TYPE type , const WidgetColorset& colors);

void DefaultSplitterDrawFunction(EagleGraphicsContext* window , Rectangle divider_rect , SPLITTER_TYPE type , const WidgetColorset& colors);


class TwoWaySplitter : public Layout {
   
private :
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
   

public :
   Rectangle GetHandleArea();

protected :
   
   /// LayoutBase
   void RepositionAllChildren();
   void RepositionChild(int slot);
   
   /// WidgetBase
   int PrivateHandleEvent(EagleEvent e);
   int PrivateCheckInputs();
   void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);

public :   
   
   TwoWaySplitter(SPLITTER_TYPE stype , EagleGraphicsContext* mouse_window = 0);
   
   virtual ~TwoWaySplitter();
   
   virtual bool PlaceWidget(WidgetBase* widget , int slot , bool delete_when_removed = false);
   virtual bool AddWidget(WidgetBase* widget , bool delete_when_removed = false);

   /// Layout and WidgetBase
   virtual void SetDrawPos(int xpos , int ypos);
   virtual void SetDrawDimensions(int width , int height);
   virtual void SetArea(int xpos , int ypos , int width , int height);
   void SetArea(const Rectangle& r);

   int AbsMinWidth();
   int AbsMinHeight();
   
   /// Member functions
   void SetDividerPosActual(int divpos);
   void SetDividerSize(int divsize);
   void SetDividerPos(int divpos);
   void SetDividerPercent(float percent);

   void SetMouseWindow(EagleGraphicsContext* window);

};



/*
class ThreeWaySplitter {

private :
   SPLITTER_TYPE splitter_type;
   
   WidgetBase* first;
   WidgetBase* second;
   WidgetBase* third;


protected :
   virtual void RepositionAllChildren()=0;
   virtual void RepositionChild(int slot)=0;
   
public :
   
   
};
*/


#endif // SplitterLayouts_HPP



