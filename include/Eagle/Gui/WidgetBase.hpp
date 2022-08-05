
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
 * @file WidgetBase.hpp
 * @brief The main interface for the WidgetBase class and associated functions
 * 
 * @ref WidgetBase provides the functionality common to all widgets, and provides virtual
 * functions to override certain behaviors. To create a new widget, your class should
 * extend WidgetBase.
 * 
 */

#ifndef WidgetBaseNew_HPP
#define WidgetBaseNew_HPP



#include "Eagle/Object.hpp"
#include "Eagle/Events.hpp"
#include "Eagle/SharedMemory.hpp"
//#include "Eagle/Gui/WidgetDecorators.hpp"

#include "Eagle/Gui/WidgetMessage.hpp"
#include "Eagle/Gui/WidgetArea.hpp"
#include "Eagle/Gui/WidgetFlags.hpp"
#include "Eagle/Gui/WidgetAttributes.hpp"
#include "Eagle/Gui/WidgetPainters.hpp"
#include "Eagle/Gui/WidgetColorset.hpp"
#include "Eagle/Gui/WidgetContainer.hpp"

/// Some forward declarations

class LayoutBase;
class WidgetHandler;
class WidgetDecorator;

extern int PREFERREDWIDGETWIDTH;
extern int PREFERREDWIDGETHEIGHT;


/*! @enum WIDGET_ZORDER_PRIORITY
 *  @brief This enum provides a small range of z depth for widgets, to enable layering
 *  
 *  WIDGET_ZORDER_PRIORITY controls the z order for widgets used by a @ref WidgetHandler.
 *  Widgets with a higher priority appear closer to the viewer and lower priority widgets
 *  are sorted beneath them.
 */

enum WIDGET_ZORDER_PRIORITY {
   ZORDER_PRIORITY_LOWEST = 0,
   ZORDER_PRIORITY_LOW    = 64,
   ZORDER_PRIORITY_NORMAL = 128,
   ZORDER_PRIORITY_HIGH   = 192,
   ZORDER_PRIORITY_HIGHEST = 255
};



/*! @enum DIALOG_RETURN_VALUE
 *  @brief Bitfield values for messages returned to a dialog from the @ref WidgetBase::Update<double> and @ref WidgetBase::PrivateCheckInputs<> functions.
 * 
 *  Valid values are any bitwise OR'ed combination of the following flags :
 */
enum DIALOG_RETURN_VALUE {
   DIALOG_OKAY       = 0x00,///< Return value used to specify no messages
                            ///< Also used to signify that there are no messages for the WidgetHandler::TakeNextMessage function.
   DIALOG_CLOSE      = 0x01,///< Return value used to tell our widget handler to close the dialog
   DIALOG_REDRAW_ALL = 0x02,///< For when the background needs to be cleared / redrawn
   DIALOG_INPUT_USED = 0x04,///< Play nice, a WidgetHandler will stop checking other widgets input after this is received.
   DIALOG_TAKE_FOCUS = 0x08,///< Add this flag if you want to take the focus
   DIALOG_DISABLED   = 0x10,///< Returned when you try to update or check the inputs of a disabled dialog.
   DIALOG_REMOVE_ME  = 0x20 ///< Used for popups, so they can be removed timely
};


///< Default topic ID to specify this message relates to a dialog. @ref WidgetMessage
extern const unsigned int TOPIC_DIALOG;

/**! @enum DIALOG_MSGS
 *   @brief An enum to keep track of the different kinds of messages a dialog may send
 */
enum DIALOG_MSGS {
   DIALOG_NONE    = 0,
   DIALOG_I_MOVED = 1
};



///< Registers the DIALOG_I_MOVED message ID with the TOPIC_DIALOG topic
REGISTER_WIDGET_MESSAGE(TOPIC_DIALOG , DIALOG_I_MOVED);



/*! @class WidgetBase
 *  @brief The main widget class for Eagle
 * 
 *  WidgetBase holds the code and data common to all widgets.
 *  A WidgetBase object is also an @ref EagleObject and an @ref EagleEventSource
 */

class WidgetBase : public EagleObject , public EagleEventSource {

protected :
   /// For sub widgets
   WIDGETCONTAINER widgets;///< Container for sub widgets. Currently unused.
   
   /// Separate
   WIDGETAREA warea;///< The area occupied by our widget
   WidgetFlags wflags;///< The flags set for this widget
   AttributeValueMap wattributes;///< Our widget attribute value map

   /// References only
   WidgetBase* wparent;///< For notifying the parent
   LayoutBase* wlayout;///< For notifying our layout
   WidgetHandler* whandler;///< For notifying our widget handler
   
   /// Can be shared
   WidgetPainter wpainter;///< A painter object that controls how the widget will be painted
   SHAREDOBJECT<WidgetColorset> wcolors;///< A shared reference to our widget's colors
   SHAREDOBJECT<WidgetDecorator> wdecorator;///< A decorator object that can be used to alter the widgets appearance. Decorators may be chained.
   
   
   /// Visual ordering
   int zdepth;///< How deep a widget is in the z order. Larger values indicate the widget is closer to the screen, and on top

   int prefw;
   int prefh;

   /// IScrollable
   
   virtual void ScrollCallback() {SetRedrawFlag();}

   /// WidgetEventSource
   void RaiseEvent(WidgetMsg msg);///< Call RaiseEvent when you want to emit an event from this widget
   
   /// Private interface, override to define behavior
   virtual int PrivateHandleEvent(EagleEvent ee);///< Virtual function that controls how widgets react to events
   virtual int PrivateCheckInputs();///< Virtual function that controls how widgets react to polled input
   virtual int PrivateUpdate(double dt);///< Virtual function that controls what happens when time passes for a widget
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);///< Virtual function that controls how a widget is drawn

   /// Callbacks, override and/or call if you need to
   virtual void OnAreaChanged();///< Override to react to changes in this widget's area
   virtual void OnAttributeChanged(const ATTRIBUTE& a , const VALUE& v);///< Override to react to attribute changes
   virtual void OnFlagChanged(WIDGET_FLAGS f , bool on);///< Override to react to flag changes
   virtual void OnColorChanged();///< Override to react to color changes
   
   /// Default change handlers
   void OnSelfAreaChanged(WIDGETAREA new_widget_area);///< Default code that runs when an area changes
   void OnSelfAttributeChanged(ATTRIBUTE a , VALUE v);///< Default code that runs when an attribute changes
   void OnSelfFlagChanged(WidgetFlags new_widget_flags);///< Default code that runs when a flag changes
   void OnSelfColorChanged(SHAREDOBJECT<WidgetColorset> cset);///< Default code that runs when a color changes
   
   
public :
   
WidgetBase(std::string classname , std::string objname) :
      EagleObject(classname , objname),
      EagleEventSource(),
      widgets(),
      warea(),
      wflags(VISIBLE | ENABLED | MOVEABLE | RESIZEABLE | NEEDS_REDRAW),
      wattributes(),
      wparent(0),
      wlayout(0),
      whandler(0),
      wpainter(),
      wcolors(),
      zdepth(ZORDER_PRIORITY_NORMAL),
      prefw(0),
      prefh(0)
{
   
}
   virtual ~WidgetBase();
   
   /// Main interface

   int HandleEvent(EagleEvent ee);///< Handles the event passed to it, and returns a bitfield composed of 
                                  ///< flags specified by @ref DIALOG_RETURN_VALUE
   int Update(double dt);///< Updates the widget based on the delta time, and returns a bitfield composed of
                         ///< flags specified by @ref DIALOG_RETURN_VALUE
   void Display(EagleGraphicsContext* win , int xpos , int ypos);///< Displays the specified widget on the graphics window at x,y
   
   /// Parent messaging
   virtual void QueueUserMessage(const WidgetMsg& wmsg);///< Used to queue user messages by widgets to their widget handler

   /// Setters

   void SetAttributes(const AttributeValueMap& avmap);///< Overwrites the attribute map used by this widget
   void SetAttribute(const ATTRIBUTE& a , const VALUE& v);///< Sets an attribute of the widget
   void RemoveAttribute(const ATTRIBUTE& a);///< Removes a widget attribute

   void SetWidgetArea(WIDGETAREA area , bool notify_layout = true);///< Totally overwrites area if layout allows
   void SetWidgetArea(Rectangle oarea , bool notify_layout = true);///< Preserves CSS box areas as best as possible
   
   void SetWidgetFlags(WidgetFlags flags);///< Overwrites the widget's flags
   void SetHoverState(bool hover);///< Sets the hover state to the value of hover
   void SetFocusState(bool focus);///< Sets the focus state to the value of focus
   void SetVisibleState(bool visible);///< Sets the visibility to the value of visible (invisible widgets still take input)
   void SetEnabledState(bool enabled);///< Sets the enabled state to the value of enabled (disabled widgets don't accept input)
   
   virtual void ShowAndEnable();///< Shows and enables this widget
   virtual void HideAndDisable();///< Hides and disables this widget
   
   
   void SetWidgetColorset(SHAREDOBJECT<WidgetColorset> cset);///< Sets the shared colorset
   void SetWidgetColorset(const WidgetColorset& cset);///< Sets a personal colorset
   void UnsetWidgetColorset();///< Removes the personal colorset and goes back to the default

   void SetWidgetPainter(const WidgetPainter& wp);///< Set the widget painter to use for painting this widget
   void UnsetWidgetPainter();///< Reset the widget painter for this widget to the default
   
   void SetWidgetDecorator(SHAREDOBJECT<WidgetDecorator> decorator);
   
   virtual void SetZOrder(int zpriority);///< Set the z priority. Larger values are closer to the screen and appear above
                                 ///< widgets with lower priority
   
   /// Getters

   bool HasAttribute(const ATTRIBUTE& a)      const;///< Returns true if the widget has this attribute set
   bool InheritsAttribute(const ATTRIBUTE& a) const;///< Returns true if the widget has this attribute set somewhere up the parent chain
   bool AttributeIsSet(const ATTRIBUTE& a)    const;///< Returns true if the widget has the attribute set or inherits it from a parent


   VALUE GetAttributeValue(const ATTRIBUTE& a) const;///< Get the value for the associated attribute. Returns VALUE() if not found.

   WIDGETAREA GetWidgetArea() const;///< Returns the widget area used by this widget
   Rectangle OuterArea() const;///< Returns the widget's outer rectangle
   Rectangle InnerArea() const;///< Returns the widget's inner rectangle

   WidgetFlags Flags() const;///< Returns the widget's flags
   inline bool FlagOn(WIDGET_FLAGS f) {return (unsigned int)Flags() & f;}///< Whether flag f is on
   inline bool FlagOff(WIDGET_FLAGS f) {return (unsigned int)Flags() & ~f;}///< Whether flag f is off

   SHAREDOBJECT<WidgetColorset> GetWidgetColorset() const;///< Returns the widget colorset in use via a shared pointer
   const WidgetColorset& WidgetColors() const;///< Returns a const reference to this widget's colors
   EagleColor GetColor(WIDGETCOLOR wc) const;///< Returns the specified widget color for this widget
   
   
   WidgetPainter GetWidgetPainter() const;///< Returns the current widget painter in use
   
   SHAREDOBJECT<WidgetDecorator> GetWidgetDecorator() const;///< Returns the widget decorator chain in use
   
   
   virtual bool AcceptsFocus() {return true;}///< Returns true if this widget accepts the focus. Overload if you want
                                             ///< to prevent this widget from accepting the focus
   
   virtual void SetRedrawFlag();///< Marks this widget for redraw
   void SetBgRedrawFlag();///< Marks this widget for background redraw

   void ClearRedrawFlag();///< Clears the redraw flags
   
   void SetLayoutOwner(LayoutBase* l);///< Sets the layout owner
   void SetWidgetHandler(WidgetHandler* wh);///< Sets the widget handler in charge of this widget
   void SetParent(WidgetBase* p);///< Sets the parent widget pointer
   
   
///   virtual Pos2I ViewPos() const;
   virtual Pos2I ViewPos() const {return Pos2I(0,0);}///< Used by widgets with cameras to adjust the reported position of a widget
   
   Pos2I AbsParentPos() const;///< Returns the absolute (screen) position of this widget's parent
   int AbsParentX() const;///< Returns the absolute x position of the parent widget
   int AbsParentY() const;///< Returns the absolute y positoin of the parent widget
   
   WidgetArea AbsoluteArea() const;///< Returns this widget's absolute area
   
   WidgetBase*    Root();///< Returns the root widget in the parent chain
   WidgetHandler* RootHandler();///< Returns the root widget handler in the parent chain
   
   WidgetBase*    GetParent()  const {return wparent;} ///< Returns this widget's parent widget
   LayoutBase*    GetLayout()  const {return wlayout;} ///< Returns this widget's layout owner
   WidgetHandler* GetHandler() const {return whandler;}///< Returns this widget's widget handler
   
   bool HasGui();/// Returns true if this widget is also a widget handler or uses one
   virtual WidgetHandler* GetGui();///< Returns a pointer to this widget dynamically cast as a WidgetHandler*
   
   int ZValue() const {return zdepth;}///< Gets the z priority. See @ref WIDGET_ZORDER_PRIORITY 
   
   
   virtual Rectangle GetClipRectangle();
   
   virtual std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const ;///< Describes this widget to an ostream
   
   virtual bool DoIReallyHaveHover(int mx , int my);///< What's this for again? TODO
   
   virtual int AbsMinWidth() const;///< Returns the absolute minimum width needed by this widget. Override
                                   ///< to specify a minimum width to be used by layouts in charge of this widget.
   virtual int AbsMinHeight() const;///< Returns the absolute minimum height needed by this widget. Override
                                    ///< to specify a minimum height to be used by layouts in charge of this widget.
   virtual void SetPreferredSize(int pw = 0 , int ph = 0);///< 0 indicates there is no preference for this dimension , -1 indicates they want max size
   bool PreferredSize(int* pw , int* ph);///< Returns true if preferred sizes are positive and stores them in *pw and *ph
   int PreferredWidth();///< Get the preferred width for this widget - may be zero, which indicates no preference, or -1 which means max size
   int PreferredHeight();///< Get the preferred height for this widget - may be zero, which indicates no preference. or -1 which means max size
};



///< Global function to compare z order of widgets, for sorting by depth
bool DrawPriorityIsLess(const WidgetBase* w1 , const WidgetBase* w2);




#endif // WidgetBaseNew_HPP



