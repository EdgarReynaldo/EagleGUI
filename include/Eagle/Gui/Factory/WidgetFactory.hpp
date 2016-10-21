



#ifndef WidgetFactory_HPP
#define WidgetFactory_HPP



#include <map>
#include <string>

#include "Eagle/StringWork.hpp"
#include "Eagle/Error.hpp"
#include "Eagle/Gui/WidgetBase.hpp"


///class WidgetBase;
class WidgetFactory;

extern WidgetFactory eagle_widget_factory;

///WidgetFactory eagle_widget_factory;


class WidgetFactory {
   
   std::map<std::string , WIDGET_CREATION_FUNCTION> widget_maker_map;
   

public :
   
   void RegisterWidgetCreationFunction(string widget_class_name , WIDGET_CREATION_FUNCTION widget_creator);

   int RegisterBasicWidgetCreator(WIDGET_CREATION_FUNCTION widget_creator);
   int RegisterBasicTextWidgetCreator(WIDGET_CREATION_FUNCTION widget_creator);
   int RegisterBasicButtonWidgetCreator(WIDGET_CREATION_FUNCTION widget_creator);
   
   WidgetBase* CreateWidgetBaseObject(string widget_class_name , string widget_parameters);

   
   
   
};



WidgetBase* CreateWidgetObject(string widget_class_name , string widget_parameters);



template <class WIDGET_TYPE>
WIDGET_TYPE* CreateWidget(WIDGET_TYPE** pwidget_store , string widget_class_name , string widget_parameters);



/// Template functions

template <class WIDGET_TYPE>
WIDGET_TYPE* CreateWidget(WIDGET_TYPE** pwidget_store , string widget_class_name , string widget_parameters) {
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
