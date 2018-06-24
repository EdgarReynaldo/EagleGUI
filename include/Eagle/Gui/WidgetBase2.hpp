
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

#include "Eagle/Gui/WidgetArea2.hpp"
#include "Eagle/Gui/WidgetMessage.hpp"


#define WIDGETBASE WidgetBase2
#define WIDGETCONTAINER WidgetContainer


class WIDGETBASE;



class WIDGETCONTAINER {
public :
   typedef std::shared_ptr<WIDGETBASE> SHAREDWIDGET;
   typedef std::map<EAGLE_ID , SHAREDWIDGET > COMPONENT_MAP;
   typedef COMPONENT_MAP::iterator CMIT;
protected :
   
   COMPONENT_MAP cmap;
   
public :   
   
   
   EAGLE_ID Register(SHAREDWIDGET sw);
   void Remove(EAGLE_ID id);
   void Clear();

   SHAREDWIDGET FindById(EAGLE_ID id) {return cmap[id];}
   
   SHAREDWIDGET operator[] (EAGLE_ID id) {return FindById(id);}
};

#define LAYOUTBASE Layout2

class LAYOUTBASE;



class ATTRIBUTEVALUEMAP {
   
public :
   typedef std::string ATTRIBUTE;
   typedef std::string VALUE;
   typedef std::map<ATTRIBUTE , VALUE> ATTVALMAP;
   
protected :
   
   ATTVALMAP attributes;
   
public :
   
///   VALUE& operator[](ATTRIBUTE a);
   VALUE& operator[](ATTRIBUTE a) {
      return attributes[a];
   }
///   bool HasAttribute(ATTRIBUTE a);
   bool HasAttribute(ATTRIBUTE a) {
      return attributes.find(a) != attributes.end();
   }
   
};



class WidgetHandler2;

class WIDGETBASE : public EagleObject , protected EagleEventSource {


protected :
   WIDGETCONTAINER widgets;
   
   WIDGETBASE* parent;
   LAYOUTBASE* layout;
   WidgetHandler2* handler;
   
   ATTRIBUTEVALUEMAP attributes;
   
   WIDGETAREA warea;
   
      
   void RaiseWidgetEvent(WidgetMsg msg);
   
   virtual int PrivateHandleEvent(EagleEvent ee)=0;
   virtual int PrivateCheckInputs() {return 0;}
   virtual void PrivateUpdate(double dt) {(void)dt;}
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos)=0;
   
   virtual void QueueUserMessage(WidgetMsg wmsg);
public :
   
   
   int HandleEvent(EagleEvent ee);
   void Update(double dt);
   void Display(EagleGraphicsContext* win , int xpos , int ypos);
   
   
/*   virtual bool SetAttribute(ATTRIBUTE a , VALUE v);
   virtual bool SetAttribute(ATTRIBUTE a , VALUE v) {
      if (IsKnownAttribute(a)) {
         SetKnownAttribute(a , v);
      }
   }
*/

   WIDGETAREA& EditArea();

   void SetRedrawFlag() {(void)0;}
   void ClearRedrawFlag() {(void)0;}
};





#endif // WidgetBaseNew_HPP



