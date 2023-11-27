
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



#include "Eagle/ObjectRegistry.hpp"
#include "Eagle/StringWork.hpp"
#include "Eagle/Font.hpp"
#include "Eagle/Gui/WidgetBase.hpp"
#include "Eagle/Gui/Factory/WidgetCreators.hpp"
#include "Eagle/Gui/Factory/WidgetFactory.hpp"


#include "Eagle/Gui/Text/BasicText.hpp"
#include "Eagle/Gui/Text/LinkText.hpp"
#include "Eagle/Gui/Text/SelectText.hpp"

#include "Eagle/Gui/Button/BasicButton.hpp"
#include "Eagle/Gui/Button/GuiButton.hpp"
#include "Eagle/Gui/Button/IconButton.hpp"
#include "Eagle/Gui/Button/RadioGroup.hpp"
#include "Eagle/Gui/Button/ScrollButton.hpp"
#include "Eagle/Gui/Button/TextIconButton.hpp"

#include "Eagle/Gui/Scripting/Parsing.hpp"


using namespace std;


#include <cstring>
#include <cstdlib>




RegisteredWidgetCreator::RegisteredWidgetCreator(string widget_class , WIDGET_CREATION_FUNCTION widget_creator_func) {
   EagleWidgetFactory().RegisterWidgetCreationFunction(widget_class , widget_creator_func);
}



REGISTERED_WIDGET_CREATOR(BasicText         , CreateBasicTextWidget);
REGISTERED_WIDGET_CREATOR(SelectText        , CreateSelectTextWidget);
REGISTERED_WIDGET_CREATOR(LinkText          , CreateLinkTextWidget);
REGISTERED_WIDGET_CREATOR(BasicButton       , CreateBasicButtonWidget);
REGISTERED_WIDGET_CREATOR(GuiButton         , CreateGuiButtonWidget);
REGISTERED_WIDGET_CREATOR(IconButton        , CreateIconButtonWidget);
REGISTERED_WIDGET_CREATOR(RadioButton       , CreateRadioButtonWidget);
REGISTERED_WIDGET_CREATOR(BasicScrollButton , CreateScrollButtonWidget);
REGISTERED_WIDGET_CREATOR(TextIconButton    , CreateTextIconButtonWidget);



WidgetBase* CreateBasicTextWidget(std::string object_name , AttributeValueMap avmap) {
   WidgetBase* widget = new BasicText(object_name);
   widget->SetAttributes(avmap);
   return widget;
}



WidgetBase* CreateSelectTextWidget(std::string object_name , AttributeValueMap avmap) {
   WidgetBase* widget = new SelectText(object_name);
   widget->SetAttributes(avmap);
   return widget;
}



WidgetBase* CreateLinkTextWidget (std::string object_name , AttributeValueMap avmap) {
   WidgetBase* widget = new LinkText(object_name);
   widget->SetAttributes(avmap);
   return widget;
}



WidgetBase* CreateBasicButtonWidget(std::string object_name , AttributeValueMap avmap) {
   WidgetBase* widget = new BasicButton(object_name);
   widget->SetAttributes(avmap);
   return widget;
}



WidgetBase* CreateGuiButtonWidget(std::string object_name , AttributeValueMap avmap) {
   WidgetBase* widget = new GuiButton(object_name);
   widget->SetAttributes(avmap);
   return widget;
}



WidgetBase* CreateIconButtonWidget(std::string object_name , AttributeValueMap avmap) {
   WidgetBase* widget = new IconButton(object_name);
   widget->SetAttributes(avmap);
   return widget;
}



WidgetBase* CreateRadioButtonWidget(std::string object_name , AttributeValueMap avmap) {
   (void)object_name;
   WidgetBase* widget = 0;///new RadioButton(object_name);
   throw EagleException("RadioButton is not implemented yet.");
   widget->SetAttributes(avmap);
   return widget;
}



WidgetBase* CreateScrollButtonWidget(std::string object_name , AttributeValueMap avmap) {
   WidgetBase* widget = new BasicScrollButton(object_name);
   widget->SetAttributes(avmap);
   return widget;
}


WidgetBase* CreateTextIconButtonWidget(std::string object_name , AttributeValueMap avmap) {
   /// TODO : Parse font and text from avmap
   TextIconButton* tbtn = new TextIconButton("TextIconButton" , object_name);
   tbtn->SetAttributes(avmap);
   return tbtn;
}



/**
WidgetBase* CreateTextButtonWidget(std::string object_name , std::AttributeValueMap avmap) {
   map<string , string> att_map = ParseAttributeSet(widget_parameters);
   map<string , string>::const_iterator cit = att_map.end();
   
   TextButton* widget = new TextButton();
   
   if ((cit = att_map.find("TEXTNAME")) != att_map.end()) {
      EagleObject* text_obj = GetFirstObjectByName(cit->second);
      BasicText* text = dynamic_cast<BasicText*>(text_obj);
      if (!text) {
         throw EagleException(StringPrintF("Failed to retrieve TEXTNAME object by name '%s'\n" , cit->second.c_str()));
      }
      widget->GetTextDecorator()->UseTextWidget(text);
   }
   
   TextDecorator* text_decorator = widget->GetTextDecorator();
   
   WidgetBase* text_widget = text_decorator->GetTextWidget();

   ApplyTextAttributes(text_widget , att_map);
   
   ApplyWidgetBaseAttributes(widget->GetTextDecorator() , att_map);

///   text_decorator->RepositionText();
   
   return widget;
}

*/

/// Right now only NAME , POS, DIM, and AREA are supported
void ApplyWidgetBaseAttributes(WidgetBase* widget , const AttributeValueMap& avmap) {
   
   if (!widget) {
      throw EagleException("ApplyWidgetBaseAtributes : widget is NULL.\n");
   }
   
   const ATTVALMAP& attribute_map = avmap.GetAttributeValueMap();
   
///   widget = widget->GetDecoratorRoot();/// TODO : FIXME : ???
   
   map<string , string>::const_iterator cit = attribute_map.end();
   
   const char* cstr = 0;
      
   if ((cit = attribute_map.find("NAME")) != attribute_map.end()) {
      widget->SetShortName(cit->second);
   }

   if ((cit = attribute_map.find("POS")) != attribute_map.end()) {
      cstr = cit->second.c_str();
      int x = 0 , y = 0;
      if (2 != sscanf(cstr , "%d,%d" , &x , &y)) {
         throw EagleException(StringPrintF("ApplyWidgetBaseAtributes:: (POS) Failed to read x,y pair from (%s)\n" , cstr));
      }
      Rectangle warea = widget->OuterArea();
      warea.SetPos(x,y);
      widget->SetWidgetArea(warea);
   }
   if ((cit = attribute_map.find("DIM")) != attribute_map.end()) {
      cstr = cit->second.c_str();
      int w = 0 , h = 0;
      if (2 != sscanf(cstr , "%d,%d" , &w , &h)) {
         throw EagleException(StringPrintF("ApplyWidgetBaseAtributes:: (DIM) Failed to read w,h pair from (%s)\n" , cstr));
      }
      int x = widget->OuterArea().X();
      int y = widget->OuterArea().Y();
      if (w < 0 || h < 0) {
         if (w < 0) {
            w = abs(w);
            x = x - w;
         }
         if (h < 0) {
            h = abs(h);
            y = y - h;
         }
      }
      widget->SetWidgetArea(Rectangle(x,y,w,h));
//      Pos(x,y);
//      widget->SetWidgetDimensions(w,h);
   }
   if ((cit = attribute_map.find("AREA")) != attribute_map.end()) {
      cstr = cit->second.c_str();
      int x = 0 , y = 0 , w = 0 , h = 0;
      if (4 != sscanf(cstr , "%d,%d,%d,%d" , &x , &y , &w , &h)) {
         throw EagleException(StringPrintF("ApplyWidgetBaseAtributes:: (AREA) Failed to read x,y,w,h set from (%s)\n" , cstr));
      }
      widget->SetWidgetArea(Rectangle(x,y,w,h));
   }
   
}



void ApplyTextAttributes(WidgetBase* widget ,  const AttributeValueMap& avmap) {
   
   const ATTVALMAP& attribute_map = avmap.GetAttributeValueMap();
   
   BasicText* text_widget = dynamic_cast<BasicText*>(widget);
   
   if (!text_widget) {
      throw EagleException(StringPrintF("ApplyTextAttributes - widget %p is not a BasicText widget!!!\n" , (void*)widget));
   }
   
   map<string , string>::const_iterator cit = attribute_map.end();
   
   if ((cit = attribute_map.find("TEXT")) != attribute_map.end()) {
      text_widget->SetupText(cit->second);
   }
   if ((cit = attribute_map.find("FONT")) != attribute_map.end()) {
      text_widget->SetupText(text_widget->GetText() , GetFont(cit->second));
   }
   if ((cit = attribute_map.find("SHRINKWRAP")) != attribute_map.end()) {
      if (strcmp(cit->second.c_str() , "YES") == 0) {
         text_widget->ShrinkWrap();
      }
   }
}



void SetWidgetBaseParameters(WidgetBase* widget , string widget_parameters) {
   ApplyWidgetBaseAttributes(widget , ParseAttributeSet(widget_parameters));
}

