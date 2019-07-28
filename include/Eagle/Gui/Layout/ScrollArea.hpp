
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

#include "Eagle/Gui/Layout/RelativeLayout.hpp"
#include "Eagle/Gui/ScrollBar.hpp"



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
   
   WidgetBase* our_scroll_widget;
   


   virtual void ReserveSlots(int nslots);
   
   void ResetScrollbars();
   
   Rectangle GetViewRectangle();
   
   virtual void RespondToEvent(EagleEvent e , EagleThread* thread = MAIN_THREAD);

public :
   
//   ScrollArea(std::string name="Nemo");
   ScrollArea(std::string name) :
         LayoutBase("ScrollArea" , name),
         EagleEventListener(),
         basic_hscrollbar("BasicScrollBar" , "basic_hscrollbar"),
         basic_vscrollbar("BasicScrollBar" , "basic_vscrollbar"),
         hscrollbar(0),
         vscrollbar(0),
         scrollbarsize(10),
         onleft(false),
         ontop(false),
         our_scroll_widget(0)
   {
      SetScrollBars((BasicScrollBar*)0 , (BasicScrollBar*)0);
   }
   
   void SetScrollBars(BasicScrollBar* horizontalscrollbar , BasicScrollBar* verticalscrollbar);
   
   void SetScrollbarPosition(bool on_top , bool on_left);///< Pass true for left/top, false for right/bottom
   void SetHScrollbarPosition(bool on_top);///< Pass true to place the horizontal scrollbar on the top, false on the bottom
   void SetVSCrollbarPosition(bool on_left);///< Pass true to place the vertical scrollbar on the left, false on the right
   
   /// LayoutBase
   
   virtual Rectangle RequestWidgetArea(int widget_slot , int newx , int newy , int newwidth , int newheight);
   virtual void Resize(unsigned int nsize);///< Virtual function to resize the layout's widget storage
   virtual void PlaceWidget(WidgetBase* w , int slot);
   virtual int AddWidget(WidgetBase* w);/// Adds the widget to the next free slot or creates one if necessary, returns slot used
   
   /// WidgetBase

   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);///< Draws the background and filler object in the corner
};


































