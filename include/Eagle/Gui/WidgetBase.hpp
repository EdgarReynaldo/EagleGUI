
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
#include "Eagle/Gui/WidgetContainer.hpp"
#include "Eagle/Gui/WidgetPainters.hpp"
#include "Eagle/Gui/WidgetColorset.hpp"
#include "Eagle/Gui/WidgetContainer.hpp"




SHAREDWIDGET StackWidget(WIDGETBASE* stack_widget);
SHAREDWIDGET StackWidget(WIDGETBASE& stack_widget);


class LAYOUTBASE;
class WidgetHandler2;



class WIDGETBASE : public EagleObject , protected EagleEventSource {

protected :
   /// For sub widgets
   WIDGETCONTAINER widgets;
   
   /// Separate
   WIDGETAREA warea;
   WidgetFlags wflags;
   ATTRIBUTEVALUEMAP wattributes;

   /// References only
   WIDGETBASE* wparent;
   LAYOUTBASE* wlayout;
   WidgetHandler2* whandler;
   
   /// Can be shared
   WidgetPainter wpainter;
   std::shared_ptr<WidgetColorset> wcolors;
   
   

   /// WidgetEventSource
   void RaiseWidgetEvent(WidgetMsg msg);
   
   /// Private interface, override to define behavior
   virtual int PrivateHandleEvent(EagleEvent ee);
   virtual int PrivateCheckInputs();
   virtual void PrivateUpdate(double dt);
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
   void OnSelfColorChanged(std::shared_ptr<WidgetColorset> cset);
   
   
public :
   
WIDGETBASE(std::string classname , std::string objname) :
      EagleObject(classname , objname),
      EagleEventSource(),
      widgets(),
      warea(),
      wflags(),
      wattributes(),
      wparent(0),
      wlayout(0),
      whandler(0),
      wpainter(),
      wcolors(0)
{}
   virtual ~WIDGETBASE();
   
   /// Main interface

   int HandleEvent(EagleEvent ee);
   void Update(double dt);
   void Display(EagleGraphicsContext* win , int xpos , int ypos);
   
   /// Parent messaging
   virtual void QueueUserMessage(WidgetMsg wmsg);

   /// Setters

   void SetAttribute(const ATTRIBUTE& a , const VALUE& v);
   void RemoveAttribute(const ATTRIBUTE& a);

   void SetWidgetArea(WIDGETAREA area , bool notify_layout = true);/// Totally overwrites area if layout allows
   void SetWidgetArea(Rectangle oarea , bool notify_layout = true);/// Preserves border areas
   
   void SetWidgetFlags(WidgetFlags flags);
   void SetHoverState(bool hover);
   void SetFocusState(bool focus);
   
   void SetWidgetColorset(std::shared_ptr<WidgetColorset> cset);
   void SetWidgetColorset(const WidgetColorset& cset);
   void UnsetWidgetColorset();

   void SetWidgetPainter(const WidgetPainter& wp);
   void UnsetWidgetPainter();
   
   /// Getters

   bool HasAttribute(const ATTRIBUTE& a) const;
   bool InheritsAttribute(const ATTRIBUTE& a) const;
   bool AttributeIsSet(const ATTRIBUTE& a) const;


   VALUE GetAttributeValue(const ATTRIBUTE& a) const;

   WIDGETAREA GetWidgetArea();
   Rectangle OuterArea() {return warea.OuterArea();}
   Rectangle InnerArea() {return warea.InnerArea();}

   WidgetFlags Flags();

   EagleColor GetColor(WIDGETCOLOR wc);
   
   WidgetColorset WidgetColors();
   
   WidgetPainter GetWidgetPainter();
   
   
   virtual bool AcceptsFocus() {return true;}
   
   virtual void SetRedrawFlag();
   void SetBgRedrawFlag();

   void ClearRedrawFlag();
   
   void SetLayoutOwner(LAYOUTBASE* l);
   void SetWidgetHandler(WidgetHandler2* wh);
   
   virtual int AbsMinWidth();
   virtual int AbsMinHeight();
   
};







#endif // WidgetBaseNew_HPP






