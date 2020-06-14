
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
 * @file ScrollArea.hpp
 * @brief This file holds the interface for scrolling layouts
 */

#ifndef ScrollArea_HPP
#define ScrollArea_HPP



#include "Eagle/Gui/Layout/RelativeLayout.hpp"
#include "Eagle/Gui/ScrollBar.hpp"
#include "Eagle/Gui/ScrollView.hpp"


/**! @class ScrollArea
 *   @brief The ScrollArea class is a layout that holds a horizontal and vertical scrollbars and arranges its view automatically
 */
 
class ScrollArea : public LayoutBase , public EagleEventListener {
   
   BasicScrollBar basic_hscrollbar;
   BasicScrollBar basic_vscrollbar;
   
   BasicScrollBar* hscrollbar;
   BasicScrollBar* vscrollbar;
   
   int scrollbarsize;
   bool onleft;
   bool ontop;

   ScrollView our_scroll_view;
   
   WidgetBase* our_scroll_widget;

   bool drag;
   int anchorx;
   int anchory;
   int anchorxscroll;
   int anchoryscroll;



   virtual void ReserveSlots(int nslots);
   
   void ResetScrollbars();
   
   Rectangle GetViewRectangle();
   
   /// EagleEventListener
   
   virtual void RespondToEvent(EagleEvent e , EagleThread* thread = MAIN_THREAD);

   /// WidgetBase
   
   virtual int PrivateHandleEvent(EagleEvent ee);

   virtual void OnAreaChanged();
   virtual void OnFlagChanged(WIDGET_FLAGS f , bool on);
   
public :
   
   
   ScrollArea(std::string name="Nemo");
   ~ScrollArea();
   
   void SetScrollBars(BasicScrollBar* horizontalscrollbar , BasicScrollBar* verticalscrollbar);
   
   
   void SetScrollbarPosition(bool on_top , bool on_left);///< Pass true for left/top, false for right/bottom
   void SetHScrollbarPosition(bool on_top);///< Pass true to place the horizontal scrollbar on the top, false on the bottom
   void SetVSCrollbarPosition(bool on_left);///< Pass true to place the vertical scrollbar on the left, false on the right
   
   void SetViewWidget(WidgetBase* widget_to_view);

   /// LayoutBase
   
   virtual Rectangle RequestWidgetArea(int widget_slot , int newx , int newy , int newwidth , int newheight);
   virtual void Resize(unsigned int nsize);///< No need to call, does nothing. Size is always 3.

   virtual void PlaceWidget(WidgetBase* w , int slot);///< Gives our scroll view widget a child widget
   virtual int AddWidget(WidgetBase* w);/// Adds the widget to the next free slot or creates one if necessary, returns slot used

   /// WidgetBase 
   
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);

   virtual bool AcceptsFocus() override {return true;}
};




#endif // ScrollArea_HPP




