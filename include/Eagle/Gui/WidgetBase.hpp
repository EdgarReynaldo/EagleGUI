
/**
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
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2019+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */


#ifndef WidgetBaseNew_HPP
#define WidgetBaseNew_HPP


#include <memory>
#include <map>

#include "Eagle/Object.hpp"
#include "Eagle/Events.hpp"

#include "Eagle/Gui/WidgetMessage.hpp"

#include "Eagle/Gui/WidgetArea.hpp"
#include "Eagle/Gui/WidgetFlags.hpp"
#include "Eagle/Gui/WidgetAttributes.hpp"
#include "Eagle/Gui/WidgetPainters.hpp"
#include "Eagle/Gui/WidgetColorset.hpp"
#include "Eagle/Gui/WidgetContainer.hpp"


class LayoutBase;
class WidgetHandler;

enum WIDGET_ZORDER_PRIORITY {
   ZORDER_PRIORITY_LOWEST = 0,
   ZORDER_PRIORITY_LOW    = 64,
   ZORDER_PRIORITY_NORMAL = 128,
   ZORDER_PRIORITY_HIGH   = 192,
   ZORDER_PRIORITY_HIGHEST = 255
};



/// DIALOG_RETURN_VALUE = Bitfield values for messages returned to a dialog from the Update() and CheckInput() functions.
enum DIALOG_RETURN_VALUE {
   DIALOG_OKAY       = 0x00,/// Also used to signify that there are no messages for the WidgetHandler::TakeNextMessage function.
   DIALOG_CLOSE      = 0x01,
   DIALOG_REDRAW_ALL = 0x02,/// For when the background needs to be cleared / redrawn
   DIALOG_INPUT_USED = 0x04,/// Play nice, a WidgetHandler will stop checking other widgets input after this is received.
   DIALOG_TAKE_FOCUS = 0x08,
   DIALOG_DISABLED   = 0x10,/// Returned when you try to update or check the inputs of a disabled dialog.
   DIALOG_REMOVE_ME  = 0x20
};


extern const unsigned int TOPIC_DIALOG;


enum DIALOG_MSGS {
   DIALOG_NONE    = 0,
   DIALOG_I_MOVED = 1
};



REGISTER_WIDGET_MESSAGE(TOPIC_DIALOG , DIALOG_I_MOVED);



class WidgetBase : public EagleObject , public EagleEventSource {

protected :
   /// For sub widgets
   WIDGETCONTAINER widgets;
   
   /// Separate
   WIDGETAREA warea;
   WidgetFlags wflags;
   AttributeValueMap wattributes;

   /// References only
   WidgetBase* wparent;
   LayoutBase* wlayout;
   WidgetHandler* whandler;
   
   /// Can be shared
   WidgetPainter wpainter;
   SHAREDOBJECT<WidgetColorset> wcolors;
   
   /// Visual ordering
   int zdepth;

   

   /// WidgetEventSource
   void RaiseEvent(WidgetMsg msg);
   
   /// Private interface, override to define behavior
   virtual int PrivateHandleEvent(EagleEvent ee);
   virtual int PrivateCheckInputs();
   virtual int PrivateUpdate(double dt);
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);

   /// Callbacks, overload if you need to
   virtual void OnAreaChanged();
   virtual void OnAttributeChanged(const ATTRIBUTE& a , const VALUE& v);
   virtual void OnFlagChanged(WIDGET_FLAGS f , bool on);
   virtual void OnColorChanged();
   
   /// Default change handlers
   void OnSelfAreaChanged(WIDGETAREA new_widget_area);
   void OnSelfAttributeChanged(ATTRIBUTE a , VALUE v);
   void OnSelfFlagChanged(WidgetFlags new_widget_flags);
   void OnSelfColorChanged(SHAREDOBJECT<WidgetColorset> cset);
   
   
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
      zdepth(ZORDER_PRIORITY_NORMAL)
{}
   virtual ~WidgetBase();
   
   /// Main interface

   int HandleEvent(EagleEvent ee);
   int Update(double dt);
   void Display(EagleGraphicsContext* win , int xpos , int ypos);
   
   /// Parent messaging
   virtual void QueueUserMessage(WidgetMsg wmsg);

   /// Setters

   void SetAttributes(const AttributeValueMap& avmap);
   void SetAttribute(const ATTRIBUTE& a , const VALUE& v);
   void RemoveAttribute(const ATTRIBUTE& a);

   void SetWidgetArea(WIDGETAREA area , bool notify_layout = true);/// Totally overwrites area if layout allows
   void SetWidgetArea(Rectangle oarea , bool notify_layout = true);/// Preserves border areas
   
   void SetWidgetFlags(WidgetFlags flags);
   void SetHoverState(bool hover);
   void SetFocusState(bool focus);
   
   void SetWidgetColorset(SHAREDOBJECT<WidgetColorset> cset);
   void SetWidgetColorset(const WidgetColorset& cset);
   void UnsetWidgetColorset();

   void SetWidgetPainter(const WidgetPainter& wp);
   void UnsetWidgetPainter();
   
   void SetZOrder(int zpriority);
   
   /// Getters

   bool HasAttribute(const ATTRIBUTE& a) const;
   bool InheritsAttribute(const ATTRIBUTE& a) const;
   bool AttributeIsSet(const ATTRIBUTE& a) const;


   VALUE GetAttributeValue(const ATTRIBUTE& a) const;

   WIDGETAREA GetWidgetArea() const;
   Rectangle OuterArea() const;
   Rectangle InnerArea() const;

   WidgetFlags Flags() const;

   SHAREDOBJECT<WidgetColorset> GetWidgetColorset() const;
   const WidgetColorset& WidgetColors() const;
   EagleColor GetColor(WIDGETCOLOR wc) const;
   
   
   WidgetPainter GetWidgetPainter() const;
   
   
   virtual bool AcceptsFocus() {return true;}
   
   virtual void SetRedrawFlag();
   void SetBgRedrawFlag();

   void ClearRedrawFlag();
   
   void SetLayoutOwner(LayoutBase* l);
   void SetWidgetHandler(WidgetHandler* wh);
   void SetParent(WidgetBase* p);
   
   virtual int AbsMinWidth() const;
   virtual int AbsMinHeight() const;
   
///   virtual Pos2I ViewPos() const;
   virtual Pos2I ViewPos() const {return Pos2I(0,0);}
   
   Pos2I AbsParentPos() const;
   int AbsParentX() const;
   int AbsParentY() const;
   
   WidgetArea AbsoluteArea() const;
   
   WidgetBase*    Root();
   WidgetHandler* RootHandler();
   
   WidgetBase*    GetParent()  {return wparent;}
   LayoutBase*    GetLayout()  {return wlayout;}
   WidgetHandler* GetHandler() {return whandler;}
   
   bool HasGui();
   virtual WidgetHandler* GetGui();
   
   int ZValue() const {return zdepth;}
   
   
   virtual std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const ;
   
};

bool DrawPriorityIsLess(const WidgetBase* w1 , const WidgetBase* w2);

#endif // WidgetBaseNew_HPP






