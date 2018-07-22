
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
 *    EAGLE
 *    Edgar's Agile Gui Library and Extensions
 *
 *    Copyright 2009-2018+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */



#ifndef WidgetContainer_HPP
#define WidgetContainer_HPP


#include <memory>
#include <map>


#include "Eagle/Object.hpp"






#define WIDGETCONTAINER WidgetContainer


#define WIDGETBASE WidgetBase2



class WIDGETBASE;


struct WIDGET_DELETER { 
   bool deleteme;
   WIDGET_DELETER() : deleteme(true) {}
   WIDGET_DELETER(bool destroy_when_done) : deleteme(destroy_when_done) {}
   
   void operator()(WIDGETBASE* w) const {
      (void)w;
      if (deleteme) {
         delete w;
      }
      return;
   }
};


typedef std::shared_ptr<WIDGETBASE , WIDGET_DELETER> SHAREDWIDGET;



SHAREDWIDGET StackWidget(WIDGETBASE* stack_widget);
SHAREDWIDGET StackWidget(WIDGETBASE& stack_widget);



class WIDGETCONTAINER {
public :
   typedef std::map<EAGLE_ID , SHAREDWIDGET > COMPONENT_MAP;
   typedef COMPONENT_MAP::iterator CMIT;
protected :
   
   COMPONENT_MAP cmap;
   
public :   
   
   WIDGETCONTAINER();
   ~WIDGETCONTAINER();
   
   EAGLE_ID Register(SHAREDWIDGET sw);
   void Remove(EAGLE_ID id);
   void Clear();

   SHAREDWIDGET FindById(EAGLE_ID id);
   
   SHAREDWIDGET operator[] (EAGLE_ID id) {return FindById(id);}
};





#endif // WidgetContainer_HPP

