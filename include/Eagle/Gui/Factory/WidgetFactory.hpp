



#ifndef WidgetFactory_HPP
#define WidgetFactory_HPP



#include <map>
#include <string>

#include "Eagle/StringWork.hpp"
#include "Eagle/Gui/WidgetBase.hpp"


///class WidgetBase;
class WidgetFactory;

extern WidgetFactory eagle_widget_factory;

///WidgetFactory eagle_widget_factory;

typedef WidgetBase* (*WIDGET_CREATION_FUNCTION)(string widget_parameters);

class RegisteredWidgetCreator {
public :
   RegisteredWidgetCreator(string widget_class , WIDGET_CREATION_FUNCTION widget_creator_func);
};

#define REGISTER_WIDGET_CREATOR(widget_class , widget_creator_func) \
   extern RegisteredWidgetCreator RWC_##widget_class_##widget_creator_func;

#define REGISTERED_WIDGET_CREATOR(widget_class , widget_creator_func) \
   RegisteredWidgetCreator RWC_##widget_class_##widget_creator_func(#widget_class , widget_creator_func);




WidgetBase* CreateBasicTextWidget   (string widget_parameters);
WidgetBase* CreateSelectTextWidget  (string widget_parameters);
WidgetBase* CreateLinkTextWidget    (string widget_parameters);
WidgetBase* CreateBasicButtonWidget (string widget_parameters);
WidgetBase* CreateGuiButtonWidget   (string widget_parameters);
WidgetBase* CreateIconButtonWidget  (string widget_parameters);
WidgetBase* CreateRadioButtonWidget (string widget_parameters);
WidgetBase* CreateScrollButtonWidget(string widget_parameters);



REGISTER_WIDGET_CREATOR(BasicText ,    CreateBasicTextWidget);
REGISTER_WIDGET_CREATOR(SelectText ,   CreateSelectTextWidget);
REGISTER_WIDGET_CREATOR(LinkText ,     CreateLinkTextWidget);
REGISTER_WIDGET_CREATOR(BasicButton ,  CreateBasicButtonWidget);
REGISTER_WIDGET_CREATOR(GuiButton ,    CreateGuiButtonWidget);
REGISTER_WIDGET_CREATOR(IconButton ,   CreateIconButtonWidget);
REGISTER_WIDGET_CREATOR(RadioButton ,  CreateRadioButtonWidget);
REGISTER_WIDGET_CREATOR(ScrollButton , CreateScrollButtonWidget);

std::map<std::string , std::string> ParseWidgetParameters(string widget_parameters);




/// Right now only POS:x,y , and DIM:w,h , and AREA:x,y,w,h are supported
/// TODO : Support WIDGETCOLORSET, and widget colors
void ApplyWidgetBaseAttributes(WidgetBase* widget , const std::map<std::string , std::string>& attribute_map);

/// Right now only TEXT:text_string , FONT:font_name, and SHRINKWRAP:YES are supported
/// TODO : In the future, support HALIGN, VALIGN, HPAD, VPAD, and LINESPACING
void ApplyTextAttributes(WidgetBase* widget ,  const std::map<std::string , std::string>& attribute_map);



void SetWidgetBaseParameters(WidgetBase* widget , std::string widget_parameters);



class WidgetFactory {
   
   std::map<std::string , WIDGET_CREATION_FUNCTION> widget_maker_map;
   

public :
   
   void RegisterWidgetCreationFunction(string widget_class_name , WIDGET_CREATION_FUNCTION widget_creator);

   int RegisterBasicWidgetCreator(WIDGET_CREATION_FUNCTION widget_creator);
   int RegisterBasicTextWidgetCreator(WIDGET_CREATION_FUNCTION widget_creator);
   int RegisterBasicButtonWidgetCreator(WIDGET_CREATION_FUNCTION widget_creator);
   
   WidgetBase* CreateWidgetBaseObject(string widget_class_name , string widget_parameters);

   
   
   
};




template <class WIDGET_TYPE>
void CreateWidget(WIDGET_TYPE** pwidget_store , string widget_class_name , string widget_parameters);



/// Template functions

template <class WIDGET_TYPE>
void CreateWidget(WIDGET_TYPE** pwidget_store , string widget_class_name , string widget_parameters) {
   EAGLE_ASSERT(pwidget_store);
   WIDGET_TYPE* pwidget = 0;
   pwidget = dynamic_cast<WIDGET_TYPE*>(eagle_widget_factory.CreateWidgetBaseObject(widget_class_name , widget_parameters));
   if (!pwidget) {
      throw EagleError(StringPrintF("Failed to create %s class widget with specified parameters (%s)\n" ,
                                    widget_class_name.c_str() , widget_parameters.c_str()));
   }
   *pwidget_store = pwidget;
}




#endif // WidgetFactory_HPP

