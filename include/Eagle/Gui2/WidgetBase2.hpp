
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

#include "Eagle/Gui2/WidgetArea2.hpp"
#include "Eagle/Gui2/WidgetFlags.hpp"
#include "Eagle/Gui2/WidgetAttributes.hpp"
#include "Eagle/Gui2/WidgetContainer.hpp"


#define WIDGETBASE WidgetBase2



class LAYOUTBASE;






class WidgetHandler2;

class WIDGETBASE : public EagleObject , protected EagleEventSource {

protected :
   /// For sub widgets
   WIDGETCONTAINER widgets;
   
   /// References only
   WIDGETBASE* wparent;
   LAYOUTBASE* wlayout;
   WidgetHandler2* whandler;
   
   /// Can be shared
   WidgetPainter wpainter;
   
   /// Separate
   ATTRIBUTEVALUEMAP wattributes;
   
   WIDGETAREA warea;
   
   WidgetFlags wflags;
   


   /// WidgetEventSource
   void RaiseWidgetEvent(WidgetMsg msg);
   
   /// Private interface, override to define behavior
   virtual int PrivateHandleEvent(EagleEvent ee);
   virtual int PrivateCheckInputs();
   virtual void PrivateUpdate(double dt);
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);

   /// Parent messaging
   virtual void QueueUserMessage(WidgetMsg wmsg);

   /// Callbacks, overload if you need to
   virtual void OnAreaChanged();
   virtual void OnAttributeChanged(const ATTRIBUTE& a , const VALUE& v);
   virtual void OnFlagChanged(WIDGET_FLAG f , bool on);

   /// Default change handlers
   void OnSelfAreaChange(WIDGETAREA new_widget_area);
   void OnSelfAttributeChange(ATTRIBUTE a , VALUE v);
   void OnSelfFlagChange(WidgetFlags new_widget_flags);
   
public :
   
   /// Main interface
   int HandleEvent(EagleEvent ee);
   void Update(double dt);
   void Display(EagleGraphicsContext* win , int xpos , int ypos);
   
   /// Setters
   bool SetAttribute(ATTRIBUTE a , VALUE v);
   void SetWidgetArea(WIDGETAREA warea);
   void SetWidgetFlags(WidgetFlags flags);

   /// Getters
   bool HasAttribute(ATTRIBUTE a);
   bool InheritsAttribute(ATTRIBUTE a);
   VALUE GetAttributeValue(ATTRIBUTE a);
   WIDGETAREA GetWidgetArea();
   WidgetFlags Flags();

   virtual void SetRedrawFlag();
   void ClearRedrawFlag();
};




#endif // WidgetBaseNew_HPP



