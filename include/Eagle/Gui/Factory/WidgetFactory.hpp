



#ifndef WidgetFactory_HPP
#define WidgetFactory_HPP



#include <map>
#include <string>

#include "Eagle/StringWork.hpp"
#include "Eagle/Error.hpp"
#include "Eagle/Gui/WidgetBase.hpp"
#include "Eagle/Gui/Factory/WidgetCreators.hpp"



class WidgetFactory;

extern WidgetFactory eagle_widget_factory;

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
   
   void RegisterWidgetCreationFunction(string widget_class_name , WIDGET_CREATION_FUNCTION widget_creator);

   int RegisterBasicWidgetCreator(WIDGET_CREATION_FUNCTION widget_creator);
   int RegisterBasicTextWidgetCreator(WIDGET_CREATION_FUNCTION widget_creator);
   int RegisterBasicButtonWidgetCreator(WIDGET_CREATION_FUNCTION widget_creator);
   
   WidgetBase* CreateWidgetBaseObject(string widget_class_name , string widget_parameters);

   
   bool FreeWidget(EAGLE_ID eid);/// Search groups for widget
   bool FreeWidget(EAGLE_ID eid , std::string group_name);
   bool FreeWidgetGroup(std::string group_name = DEFAULT_GROUP_NAME);
   void FreeAllWidgets();

};


bool FreeWidget(EAGLE_ID eid);
bool FreeWidget(EAGLE_ID eid , std::string group_name);
bool FreeWidgetGroup(std::string group_name = DEFAULT_GROUP_NAME);
void FreeAllWidgets();




WidgetBase* CreateWidgetObject(string widget_class_name , string widget_parameters);



template <class WIDGET_TYPE>
WIDGET_TYPE* CreateWidget(string widget_class_name , string widget_parameters , WIDGET_TYPE** pwidget_store = 0);



/// Template functions

template <class WIDGET_TYPE>
WIDGET_TYPE* CreateWidget(string widget_class_name , string widget_parameters , WIDGET_TYPE** pwidget_store = 0) {
   EAGLE_ASSERT(pwidget_store);
   WIDGET_TYPE* pwidget = 0;
   pwidget = dynamic_cast<WIDGET_TYPE*>(CreateWidgetObject(widget_class_name , widget_parameters));
   if (!pwidget) {
      throw EagleError(StringPrintF("Failed to create %s class widget with specified parameters (%s)\n" ,
                                    widget_class_name.c_str() , widget_parameters.c_str()));
   }
   if (pwidget_store) {
      *pwidget_store = pwidget;
   }
   return pwidget;
}


#endif // WidgetFactory_HPP

