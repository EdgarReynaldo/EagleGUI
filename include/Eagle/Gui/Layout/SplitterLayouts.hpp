
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
 *    Copyright 2009-2019+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file SplitterLayouts.hpp
 * @brief The interface for the SplitterLayout classes
 */

#ifndef SplitterLayouts_HPP
#define SplitterLayouts_HPP



#include "Eagle/Area.hpp"
#include "Eagle/Gui/WidgetColorset.hpp"

#include "Eagle/Gui/Layout/Layout.hpp"



/**! @enum SPLITTER_TYPE
 *   @brief Defines the splitter type in use, whether it is vertical or horizontal
 */
enum SPLITTER_TYPE {
   SPLITTER_VERTICAL   = 0,///< Makes the splitter use a vertical bar (separating the area into left and right)
   SPLITTER_HORIZONTAL = 1 ///< Makes the splitter use a horizontal bar (separating the area into top and bottom)
};


/**! @typedef SPLITTER_DRAW_FUNC
 *   @brief A typedef for the function signature of a splitter bar drawing function
 */

typedef void (*SPLITTER_DRAW_FUNC) (EagleGraphicsContext* window , Rectangle divider_rect , bool horizontal , const WidgetColorset& colors);



/**! @class TwoWaySplitter
 *   @brief A layout class to divide an area into two with a drag bar inbetween to resize them
 */

class TwoWaySplitter : public LayoutBase {
   
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
   
   /// LayoutBase 
   
   virtual void ReserveSlots(int nslots);

   /// WidgetBase

   virtual int PrivateHandleEvent(EagleEvent e);
   virtual int PrivateCheckInputs();
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);

   virtual void OnAreaChanged();

public :
   Rectangle GetHandleArea();///< Helper function to get the splitter handle for drawing

   TwoWaySplitter(SPLITTER_TYPE stype , EagleGraphicsContext* mouse_window = 0 , std::string objname = "Nemo");
   virtual ~TwoWaySplitter();

   /// WidgetBase overrides
   
   virtual int AbsMinWidth();///< @sa WidgetBase::AbsMinWidth
   virtual int AbsMinHeight();///< @sa WidgetBase::AbsMinHeight

   /// LayoutBase

   virtual void PlaceWidget(SHAREDWIDGET widget , int slot);///< @sa LayoutBase::PlaceWidget
   virtual int AddWidget(SHAREDWIDGET widget);///< @sa LayoutBase::AddWidget

   ///< @sa LayoutBase::RequestWidgetArea
   virtual Rectangle RequestWidgetArea(int widget_slot , int newx , int newy , int newwidth , int newheight) const;

   
   /// Member functions
protected:
   void SetDividerPosActual(int divpos);

public :

   void SetDividerSize(int divsize);///< Set the width for vertical splitters or the height for horizontal splitters
   void SetDividerPos(int divpos);///< Set the divider pos in pixels from left to right

   ///< Set the divider percent from left to right from [0.0,1.0]
   ///< A value of zero or one will effectively hide the left/top or right/bottom widget and set its area to zero
   void SetDividerPercent(float percent);

   ///< We need a graphics context to select the mouse pointer when it hovers over the drag bar
   void SetMouseWindow(EagleGraphicsContext* window);

   ///< Set the default splitter drawing function for this layout, NULL is not allowed
   void SetDrawFunction(SPLITTER_DRAW_FUNC func);
   
};


///< A simple helper for the default splitter bar drawing function
void DefaultSplitterDrawFunction(EagleGraphicsContext* window , Rectangle divider_rect , bool horizontal , EagleColor hlcol , EagleColor sdcol);

///< The default splitter bar drawing function
void DefaultSplitterDrawFunction(EagleGraphicsContext* window , Rectangle divider_rect , bool horizontal , const WidgetColorset& colors);



/**! @class ThreeWaySplitter
 *   @brief A layout that splits its area in three
 *   
 *   TODO : IMPLEMENT ME? The same result can be achieved with two TwoWaySplitters nested together
 */
 
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



