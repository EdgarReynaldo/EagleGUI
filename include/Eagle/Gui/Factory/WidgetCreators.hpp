


#ifndef WidgetCreators_HPP
#define WidgetCreators_HPP



#include <string>
#include <map>



class WidgetBase;



typedef WidgetBase* (*WIDGET_CREATION_FUNCTION)(std::string widget_parameters);



class RegisteredWidgetCreator {
public :
   RegisteredWidgetCreator(std::string widget_class , WIDGET_CREATION_FUNCTION widget_creator_func);
};

#define REGISTER_WIDGET_CREATOR(widget_class , widget_creator_func) \
   extern RegisteredWidgetCreator RWC_##widget_class_##widget_creator_func;

#define REGISTERED_WIDGET_CREATOR(widget_class , widget_creator_func) \
   RegisteredWidgetCreator RWC_##widget_class_##widget_creator_func(#widget_class , widget_creator_func);




WidgetBase* CreateBasicTextWidget   (std::string widget_parameters);
WidgetBase* CreateSelectTextWidget  (std::string widget_parameters);
WidgetBase* CreateLinkTextWidget    (std::string widget_parameters);
WidgetBase* CreateBasicButtonWidget (std::string widget_parameters);
WidgetBase* CreateGuiButtonWidget   (std::string widget_parameters);
WidgetBase* CreateIconButtonWidget  (std::string widget_parameters);
WidgetBase* CreateRadioButtonWidget (std::string widget_parameters);
WidgetBase* CreateScrollButtonWidget(std::string widget_parameters);
WidgetBase* CreateTextButtonWidget  (std::string widget_parameters);




REGISTER_WIDGET_CREATOR(BasicText         , CreateBasicTextWidget);
REGISTER_WIDGET_CREATOR(SelectText        , CreateSelectTextWidget);
REGISTER_WIDGET_CREATOR(LinkText          , CreateLinkTextWidget);
REGISTER_WIDGET_CREATOR(BasicButton       , CreateBasicButtonWidget);
REGISTER_WIDGET_CREATOR(GuiButton         , CreateGuiButtonWidget);
REGISTER_WIDGET_CREATOR(IconButton        , CreateIconButtonWidget);
REGISTER_WIDGET_CREATOR(RadioButton       , CreateRadioButtonWidget);
REGISTER_WIDGET_CREATOR(BasicScrollButton , CreateScrollButtonWidget);
REGISTER_WIDGET_CREATOR(TextButton        , CreateTextButtonWidget);





/// Right now only POS:x,y , and DIM:w,h , and AREA:x,y,w,h are supported
/// TODO : Support WIDGETCOLORSET, and widget colors
void ApplyWidgetBaseAttributes(WidgetBase* widget , const std::map<std::string , std::string>& attribute_map);

/// Right now only TEXT:text_string , FONT:font_name, and SHRINKWRAP:YES are supported
/// TODO : In the future, support HALIGN, VALIGN, HPAD, VPAD, and LINESPACING
void ApplyTextAttributes(WidgetBase* widget ,  const std::map<std::string , std::string>& attribute_map);



void SetWidgetBaseParameters(WidgetBase* widget , std::string widget_parameters);






#endif // WidgetCreators_HPP
