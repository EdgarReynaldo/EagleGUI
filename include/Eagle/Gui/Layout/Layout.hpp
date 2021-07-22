
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
 *    Copyright 2009-2021+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file Layout.hpp
 * @brief The base interface for the Layout classes
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



/**! @enum LAYOUT_ATTRIBUTES
 *   @brief Stores the abilities of the current layout
 */
enum LAYOUT_ATTRIBUTES {
   LAYOUT_ALLOWS_NOTHING               = 0,     ///< This layout doesn't allow resizing or repositioning
   LAYOUT_ALLOWS_REPOSITION            = 1 << 0,///< This layout only allows you to move widgets, not resize
   LAYOUT_ALLOWS_RESIZE                = 1 << 1,///< This layout only allows you to resize widgets??? That would move them dummy???
   LAYOUT_ALLOWS_RESIZE_AND_REPOSITION = (LAYOUT_ALLOWS_REPOSITION | LAYOUT_ALLOWS_RESIZE)///< This layout allows resizing and repositioning
};


std::string PrintLayoutAttributes(LAYOUT_ATTRIBUTES attributes);///< Helper function to print LAYOUT_ATTRIBUTES


/**! @class LayoutBase
 *   @brief The virtual base class for all layouts
 */

class LayoutBase : public WidgetBase {

protected :
   
   std::vector<WidgetBase*> wchildren;///< Our widget children. We do not own these.

   LAYOUT_ATTRIBUTES attributes;///< The stored attributes of this layout
   
   HALIGNMENT halign;///< The horizontal alignment in use
   VALIGNMENT valign;///< The vertical alignment in use



   /// Utility
   int WidgetIndex(const WidgetBase* widget) const;
   WidgetBase* GetWidget(int slot);
   const WidgetBase* GetWidget(int slot) const;
   int NextFreeSlot();

   /// Override and call if you need special storage
   virtual void ReserveSlots(int nslots);

   /// All widget placement functions use ReplaceWidget
   void ReplaceWidget(WidgetBase* widget , int slot);

   /// Adjusts the widget area to obey minimum dimensions and to obey layout attributes and widget flags
   void AdjustWidgetArea(const WidgetBase* widget , int* newx , int* newy , int* newwidth , int* newheight) const;

   /// Called automatically on reposition
   void RepositionAllChildren();
   void RepositionChild(int slot);

   /// Takes care of removing widget from associated handler and from our layout - public API is RemoveWidget
   void RemoveWidgetFromLayout(WidgetBase* widget);/// Stops tracking widget - talks to WidgetHandler

	/// WIDGETBASE
   virtual void OnFlagChanged(WIDGET_FLAGS f , bool on);///< If a layout is hidden, so are its children, if a layout is disabled, so are its children
   virtual void OnAreaChanged();///< Takes care of automatic resizing of contents

   virtual int PrivateHandleInputEvent(EagleEvent e);
   virtual int PrivateUpdate(double dt);
   virtual void PrivateDisplay(EagleGraphicsContext* win , int x , int y);

   
   
public :
   
   LayoutBase(std::string objclass , std::string objname);
   virtual ~LayoutBase();
   



   /// Layout

   ///< Virtual function for layouts to override as necessary, returns the area the layout would use for the widget
   /*! Pass INT_MAX for a parameter if you don't care about the position or size
    *  NOTE : These functions do NOT change the widget's area, they only return the area that the layout would give it
    */
   virtual Rectangle RequestWidgetArea(int widget_slot , int newx , int newy , int newwidth , int newheight);
   
   ///< A shortcut to the virtual function
   ///< @sa RequestWidgetArea(int,int,int,int,int)
   Rectangle RequestWidgetArea(const WidgetBase* widget , int newx , int newy , int newwidth , int newheight);

   ///< A shortcut to the virtual function
   ///< @sa RequestWidgetArea(int,int,int,int,int)
   Rectangle RequestWidgetArea(int widget_slot , Rectangle newarea);

   ///< A shortcut to the virtual function
   ///< @sa RequestWidgetArea(int,int,int,int,int)
   Rectangle RequestWidgetArea(const WidgetBase* widget , Rectangle newarea);

   ///< A shortcut to the virtual function
   ///< @sa RequestWidgetArea(int,int,int,int,int)
   Rectangle RequestWidgetArea(const WidgetBase* widget) const;
   
   
   virtual void Resize(unsigned int nsize);///< Virtual function to resize the layout's widget storage

   /// Adding widgets to layout
   
   /// Both replace the widget (Addwidget replaces a null widget) and call RepositionChild

   ///< Widget may be null for PlaceWidget, make sure to use a valid slot
   /*! @sa LayoutBase::GetLayoutSize */
   virtual void PlaceWidget(WidgetBase* w , int slot);

   ///< Adds a widget to the layout
   virtual int AddWidget(WidgetBase* w);///< Adds the widget to the next free slot or creates one if necessary, returns slot used
   
   ///< Inserts a widget into the layout
   virtual void InsertWidget(WidgetBase* w , int insertion_slot);///< Inserts the widget just before the insertion slot, pushing all widgets down one slot
                                                                 ///< This may move widgets!

   /// Removal of widgets

   ///< Removes a widget from the specified slot if any exists
   void EmptySlot(int slot);/// Remove a widget from the layout

   ///< Remove a widget by address
   virtual void RemoveWidget(WidgetBase* widget);///< Remove a widget from the layout, does the same thing as emptying the widget's slot

   ///< Remove and erase a widget and its slot
   virtual void EraseWidgetSlot(int slot);///< Removes the widget at this slot (may be NULL) and erases the slot from the layout

   ///< Remove and erase a widget and its slot by address
   void EraseWidgetSlot(WidgetBase* widget);///< Removes the widget at this slot (must be non-NULL) and erases the slot from the layout
   
   ///< Remove all widgets
   virtual void ClearWidgets();///< Remove all widgets from layout
   
   ///< To remove the layout from the GUI
   void DetachFromGui();///< Call this in Layout derived class's destructor
   
   ///< Virtual function to set the specified alignments for this layout
   virtual void SetAlignment(HALIGNMENT h_align , VALIGNMENT v_align);

   ///< Shortcut to pass widgets from one layout to another
   virtual void SetWChildren(std::vector<WidgetBase*> new_children);

   /// Getters

   ///< Get a copy of the widgets in storage (note : some slots may be NULL!)
   std::vector<WidgetBase*> ChildrenVector() const ;///< Some children may be NULL!
   std::vector<WidgetBase*> WChildren() const ;///< Only real children
   std::vector<WidgetBase*> Descendants() const ;///< Only real descendants
   
   LayoutBase* RootLayout();///< Get the root layout
   const LayoutBase* RootLayout() const;///< Get the root layout
   
   bool IsRootLayout() const;///< True if we are the root layout
   
   int GetLayoutSize() const ;///< Returns the currently allocated storage size for widget children
   
   virtual bool AcceptsFocus() {return false;}///< Layouts are generally invisible, and do not accept focus

//?   virtual Rectangle GetClipRectangle();///< Gets the clipping rectangle for this layout
   
   HALIGNMENT GetHAlignment() {return halign;}
   VALIGNMENT GetVAlignment() {return valign;}
   
   virtual std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const;///< Describe this layout to a stream
};




#endif // EagleGuiLayout_HPP




