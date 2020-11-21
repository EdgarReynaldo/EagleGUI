
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
 *    Copyright 2009-2021+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */



#ifndef WidgetCreators_HPP
#define WidgetCreators_HPP



#include <string>
#include <map>



#include "Eagle/Gui/WidgetAttributes.hpp"



class WidgetBase;



typedef WidgetBase* (*WIDGET_CREATION_FUNCTION)(std::string object_name , AttributeValueMap avmap);



class RegisteredWidgetCreator {
public :
   RegisteredWidgetCreator(std::string widget_class , WIDGET_CREATION_FUNCTION widget_creator_func);
};

#define REGISTER_WIDGET_CREATOR(widget_class , widget_creator_func) \
   extern RegisteredWidgetCreator RWC_##widget_class_##widget_creator_func;

#define REGISTERED_WIDGET_CREATOR(widget_class , widget_creator_func) \
   RegisteredWidgetCreator RWC_##widget_class_##widget_creator_func(#widget_class , widget_creator_func);




WidgetBase* CreateBasicTextWidget   (std::string object_name , AttributeValueMap avmap);
WidgetBase* CreateSelectTextWidget  (std::string object_name , AttributeValueMap avmap);
WidgetBase* CreateLinkTextWidget    (std::string object_name , AttributeValueMap avmap);
WidgetBase* CreateBasicButtonWidget (std::string object_name , AttributeValueMap avmap);
WidgetBase* CreateGuiButtonWidget   (std::string object_name , AttributeValueMap avmap);
WidgetBase* CreateIconButtonWidget  (std::string object_name , AttributeValueMap avmap);
WidgetBase* CreateRadioButtonWidget (std::string object_name , AttributeValueMap avmap);
WidgetBase* CreateScrollButtonWidget(std::string object_name , AttributeValueMap avmap);
///WidgetBase* CreateTextButtonWidget  (std::string object_name , AttributeValueMap avmap);




REGISTER_WIDGET_CREATOR(BasicText         , CreateBasicTextWidget);
REGISTER_WIDGET_CREATOR(SelectText        , CreateSelectTextWidget);
REGISTER_WIDGET_CREATOR(LinkText          , CreateLinkTextWidget);
REGISTER_WIDGET_CREATOR(BasicButton       , CreateBasicButtonWidget);
REGISTER_WIDGET_CREATOR(GuiButton         , CreateGuiButtonWidget);
REGISTER_WIDGET_CREATOR(IconButton        , CreateIconButtonWidget);
REGISTER_WIDGET_CREATOR(RadioButton       , CreateRadioButtonWidget);
REGISTER_WIDGET_CREATOR(BasicScrollButton , CreateScrollButtonWidget);
///REGISTER_WIDGET_CREATOR(TextButton        , CreateTextButtonWidget);





/// Right now only POS:x,y , and DIM:w,h , and AREA:x,y,w,h are supported
/// TODO : Support WIDGETCOLORSET, and widget colors
void ApplyWidgetBaseAttributes(WidgetBase* widget , const AttributeValueMap& avmap);

/// Right now only TEXT:text_string , FONT:font_name, and SHRINKWRAP:YES are supported
/// TODO : In the future, support HALIGN, VALIGN, HPAD, VPAD, and LINESPACING
void ApplyTextAttributes(WidgetBase* widget ,  const AttributeValueMap& avmap);



void SetWidgetBaseParameters(WidgetBase* widget , std::string widget_parameters);






#endif // WidgetCreators_HPP
