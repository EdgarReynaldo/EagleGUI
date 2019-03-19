
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




#ifndef WidgetFactory_HPP
#define WidgetFactory_HPP



#include <map>
#include <string>
#include <typeinfo>



#include "Eagle/StringWork.hpp"
#include "Eagle/Exception.hpp"
#include "Eagle/Gui/WidgetBase.hpp"
#include "Eagle/Gui/Factory/WidgetCreators.hpp"
#include "Eagle/Gui/Scripting/Parsing.hpp"
#include "Eagle/Gui/Scripting/GuiScript.hpp"

class WidgetFactory;

WidgetFactory& EagleWidgetFactory();

extern const char* const DEFAULT_GROUP_NAME;


class WidgetFactory {

   typedef std::map<std::string , WIDGET_CREATION_FUNCTION> WIDGET_MAKER_MAP;

   typedef std::map<EAGLE_ID , bool> WIDGET_FREED_MAP;

   typedef std::map<std::string , WIDGET_FREED_MAP> WIDGET_GROUP_MAP;

   WIDGET_MAKER_MAP widget_maker_map;

   WIDGET_FREED_MAP widget_freed_map;

   WIDGET_GROUP_MAP widget_group_map;


   void TrackWidget(EAGLE_ID eid , WIDGET_FREED_MAP& freed_map);
   void TrackWidget(EAGLE_ID eid , std::string group_name = DEFAULT_GROUP_NAME);

   bool FreeWidget(EAGLE_ID eid , WIDGET_FREED_MAP& freed_map);

public :

   void RegisterWidgetCreationFunction(std::string widget_class_name , WIDGET_CREATION_FUNCTION widget_creator);

   int RegisterBasicWidgetCreator(WIDGET_CREATION_FUNCTION widget_creator);
   int RegisterBasicTextWidgetCreator(WIDGET_CREATION_FUNCTION widget_creator);
   int RegisterBasicButtonWidgetCreator(WIDGET_CREATION_FUNCTION widget_creator);

   WidgetBase* CreateWidgetBaseObject(std::string widget_class_name , std::string object_name , std::string widget_parameters);
   WidgetBase* CreateWidgetBaseObject(std::string widget_class_name , std::string object_name , AttributeValueMap avmap);


   bool FreeWidget(EAGLE_ID eid);/// Search groups for widget
   bool FreeWidget(EAGLE_ID eid , std::string group_name);
   bool FreeWidgetGroup(std::string group_name = DEFAULT_GROUP_NAME);
   void FreeAllWidgets();

};


bool FreeWidget(EAGLE_ID eid);
bool FreeWidget(EAGLE_ID eid , std::string group_name);
bool FreeWidgetGroup(std::string group_name = DEFAULT_GROUP_NAME);
void FreeAllWidgets();




WidgetBase* CreateWidgetObject(std::string widget_class_name , std::string object_name , std::string widget_parameters);
WidgetBase* CreateWidgetObject(std::string widget_class_name , std::string object_name , AttributeValueMap avmap);



template <class WIDGET_TYPE>
WIDGET_TYPE* CreateWidget(std::string widget_class_name , std::string object_name , AttributeValueMap avmap , WIDGET_TYPE** pwidget_store = 0);

template <class WIDGET_TYPE>
WIDGET_TYPE* CreateWidget(std::string widget_class_name , std::string object_name , std::string parameters , WIDGET_TYPE** pwidget_store = 0);

template <class WIDGET_TYPE>
WIDGET_TYPE* CreateWidget(const EGSDeclaration& dec , WIDGET_TYPE** pwidget_store = 0);


/// Template functions

template <class WIDGET_TYPE>
WIDGET_TYPE* CreateWidget(std::string widget_class_name , std::string object_name , AttributeValueMap avmap , WIDGET_TYPE** pwidget_store) {
   WIDGET_TYPE* pwidget = 0;
   pwidget = dynamic_cast<WIDGET_TYPE*>(CreateWidgetObject(widget_class_name , object_name , avmap));
   if (!pwidget) {
      std::string avstr = avmap.ToString();
      throw EagleException(StringPrintF("Failed to create %s class widget with specified parameters (%s)\n" ,
                                    typeid(WIDGET_TYPE).name() , avstr.c_str()));
   }
   if (pwidget_store) {
      *pwidget_store = pwidget;
   }
   return pwidget;
}



template <class WIDGET_TYPE>
WIDGET_TYPE* CreateWidget(const EGSDeclaration& dec , WIDGET_TYPE** pwidget_store) {
   return CreateWidget<WIDGET_TYPE>(dec.ClassName() , dec.ObjectName() , dec.ValueMap() , pwidget_store);
}


template <class WIDGET_TYPE>
WIDGET_TYPE* CreateWidget(std::string widget_class_name , std::string object_name , std::string parameters , WIDGET_TYPE** pwidget_store) {
   return CreateWidget<WIDGET_TYPE>(widget_class_name , object_name , ParseAttributeSet(parameters) , pwidget_store);
}

#endif // WidgetFactory_HPP

