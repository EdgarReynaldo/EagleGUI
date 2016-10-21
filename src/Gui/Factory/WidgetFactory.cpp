



#include "Eagle/Gui/Factory/WidgetCreators.hpp"
#include "Eagle/Gui/Factory/WidgetFactory.hpp"
#include "Eagle/StringWork.hpp"


WidgetFactory eagle_widget_factory;



/// --------------------------------      WidgetFactory     --------------------------------------



void WidgetFactory::RegisterWidgetCreationFunction(string widget_class_name , WIDGET_CREATION_FUNCTION widget_creator) {
   EAGLE_ASSERT(widget_creator);
   widget_maker_map[widget_class_name] = widget_creator;
}



WidgetBase* WidgetFactory::CreateWidgetBaseObject(string widget_class_name , string widget_parameters) {
   if (widget_maker_map.find(widget_class_name) == widget_maker_map.end()) {
      throw EagleError(StringPrintF("WidgetFactory::CreateWidgetBaseObject : Could not find a creation function registered for %s class widgets.\n",
                                    widget_class_name.c_str()));
   }
   WIDGET_CREATION_FUNCTION CreatorFunc = widget_maker_map[widget_class_name];
   if (!CreatorFunc) {
      throw EagleError(StringPrintF("WidgetFactory::CreateWidgetBaseObject : Creation function for %s class widgets is NULL!\n",
                       widget_class_name.c_str()));
   }
   return CreatorFunc(widget_parameters);
}



/// ------------------     Global functions     -----------------------------



WidgetBase* CreateWidgetObject(string widget_class_name , string widget_parameters) {
   return eagle_widget_factory.CreateWidgetBaseObject(widget_class_name , widget_parameters);
}



