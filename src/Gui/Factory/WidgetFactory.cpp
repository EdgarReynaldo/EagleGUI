



#include "Eagle/Gui/Factory/WidgetFactory.hpp"
#include "Eagle/StringWork.hpp"

#include "Eagle/Gui/Text/BasicText.hpp"
#include "Eagle/Gui/Text/LinkText.hpp"
#include "Eagle/Gui/Text/SelectText.hpp"

#include "Eagle/Gui/Button/BasicButton.hpp"
#include "Eagle/Gui/Button/GuiButton.hpp"
#include "Eagle/Gui/Button/IconButton.hpp"
#include "Eagle/Gui/Button/RadioButton.hpp"
#include "Eagle/Gui/Button/ScrollButton.hpp"


using namespace std;


#include <cstring>
#include <cstdlib>


WidgetFactory eagle_widget_factory;



RegisteredWidgetCreator::RegisteredWidgetCreator(string widget_class , WIDGET_CREATION_FUNCTION widget_creator_func) {
   eagle_widget_factory.RegisterWidgetCreationFunction(widget_class , widget_creator_func);
}



REGISTERED_WIDGET_CREATOR(BasicText ,    CreateBasicTextWidget);
REGISTERED_WIDGET_CREATOR(SelectText ,   CreateSelectTextWidget);
REGISTERED_WIDGET_CREATOR(LinkText ,     CreateLinkTextWidget);
REGISTERED_WIDGET_CREATOR(BasicButton ,  CreateBasicButtonWidget);
REGISTERED_WIDGET_CREATOR(GuiButton ,    CreateGuiButtonWidget);
REGISTERED_WIDGET_CREATOR(IconButton ,   CreateIconButtonWidget);
REGISTERED_WIDGET_CREATOR(RadioButton ,  CreateRadioButtonWidget);
REGISTERED_WIDGET_CREATOR(ScrollButton , CreateScrollButtonWidget);



WidgetBase* CreateBasicTextWidget   (string widget_parameters) {
   WidgetBase* widget = 0;
   map<string , string> att_map = ParseWidgetParameters(widget_parameters);
   map<string , string>::const_iterator cit = att_map.end();
   
   if ((cit = att_map.find("SUBCLASS")) != att_map.end()) {
      string subclass = cit->second;
      if (strcmp(subclass.c_str() , "LinkText") == 0) {
         return CreateLinkTextWidget(widget_parameters);
      }
      else if (strcmp(subclass.c_str() , "SelectText") == 0) {
         return CreateSelectTextWidget(widget_parameters);
      }
   }
   
   ApplyWidgetBaseAttributes(widget , att_map);
   
   ApplyTextAttributes(widget , att_map);

   return widget;
}



WidgetBase* CreateSelectTextWidget  (string widget_parameters) {
   WidgetBase* widget = 0;
   map<string , string> att_map = ParseWidgetParameters(widget_parameters);
   map<string , string>::const_iterator cit = att_map.end();
   
   ApplyWidgetBaseAttributes(widget , att_map);
   
   ApplyTextAttributes(widget , att_map);

   return widget;
}



WidgetBase* CreateLinkTextWidget    (string widget_parameters) {
   WidgetBase* widget = 0;
   map<string , string> att_map = ParseWidgetParameters(widget_parameters);
   map<string , string>::const_iterator cit = att_map.end();
   
   ApplyWidgetBaseAttributes(widget , att_map);
   
   ApplyTextAttributes(widget , att_map);

   return widget;
}



WidgetBase* CreateBasicButtonWidget (string widget_parameters) {
   WidgetBase* widget = 0;
   map<string , string> att_map = ParseWidgetParameters(widget_parameters);
   map<string , string>::const_iterator cit = att_map.end();
   
   ApplyWidgetBaseAttributes(widget , att_map);

   return widget;
}



WidgetBase* CreateGuiButtonWidget   (string widget_parameters) {
   WidgetBase* widget = 0;
   map<string , string> att_map = ParseWidgetParameters(widget_parameters);
   map<string , string>::const_iterator cit = att_map.end();
   
   ApplyWidgetBaseAttributes(widget , att_map);

   return widget;
}



WidgetBase* CreateIconButtonWidget  (string widget_parameters) {
   WidgetBase* widget = 0;
   map<string , string> att_map = ParseWidgetParameters(widget_parameters);
   map<string , string>::const_iterator cit = att_map.end();
   
   ApplyWidgetBaseAttributes(widget , att_map);

   return widget;
}



WidgetBase* CreateRadioButtonWidget (string widget_parameters) {
   WidgetBase* widget = 0;
   map<string , string> att_map = ParseWidgetParameters(widget_parameters);
   map<string , string>::const_iterator cit = att_map.end();
   
   ApplyWidgetBaseAttributes(widget , att_map);

   return widget;
}



WidgetBase* CreateScrollButtonWidget(string widget_parameters) {
   WidgetBase* widget = 0;
   map<string , string> att_map = ParseWidgetParameters(widget_parameters);
   map<string , string>::const_iterator cit = att_map.end();
   
   ApplyWidgetBaseAttributes(widget , att_map);

   return widget;
}



/// Attributes are stored in semi-colon separated strings
/// Specific attributes are stored in two colon separated strings
/// Attributes and values may be surrounded by padding whitespace for readability
/// IE. " SUBCLASS : RadioButton ; POS : 100,50 ; DIM : 200,100 ; SDCOL : 0,64,0 ;"

map<string , string> ParseWidgetParameters(string widget_parameters) {
   
   map<string , string> attribute_map;
   
   vector<string> attributes = SplitByDelimiterString(widget_parameters , ";");
   
   for (size_t i = 0 ; i < attributes.size() ; ++i) {
      string attribute_str = attributes[i];
      vector<string> attribute_pair = SplitByDelimiterString(attribute_str , ":");
      if (attribute_pair.size() != 2) {
         throw EagleError(StringPrintF("ParseWidgetParameters : Illegal attribute pair (%s) of size %u found in attribute %u\n",
                                       attribute_str.c_str() , attribute_pair.size() , i));
      }
      string attribute = attribute_pair[0];
      string value = attribute_pair[1];
      char* attstr = strdup(attribute.c_str());
      char* valstr = strdup(value.c_str());
      TrimTrailingWhiteSpace(attstr);
      TrimTrailingWhiteSpace(valstr);
      const char* attstr_start = SkipWhiteSpace(attstr);
      const char* valstr_start = SkipWhiteSpace(valstr);
      
      attribute_map[attstr_start] = valstr_start;
      
      free(attstr);
      free(valstr);
      
      
   }
   return attribute_map;
}



/// Right now only POS, and DIM are supported
void ApplyWidgetBaseAttributes(WidgetBase* widget , const map<string , string>& attribute_map) {
   
   if (!widget) {
      throw EagleError("ApplyWidgetBaseAtributes : widget is NULL.\n");
   }
   
   map<string , string>::const_iterator cit = attribute_map.end();
   
   const char* cstr = 0;
      
   if ((cit = attribute_map.find("POS")) != attribute_map.end()) {
      cstr = cit->second.c_str();
      int x = 0 , y = 0;
      if (2 != sscanf(cstr , "%d,%d" , &x , &y)) {
         throw EagleError(StringPrintF("ApplyWidgetBaseAtributes:: (POS) Failed to read x,y pair from (%s)\n" , cstr));
      }
      widget->SetWidgetPos(x,y);
   }
   if ((cit = attribute_map.find("DIM")) != attribute_map.end()) {
      cstr = cit->second.c_str();
      int w = 0 , h = 0;
      if (2 != sscanf(cstr , "%d,%d" , &w , &h)) {
         throw EagleError(StringPrintF("ApplyWidgetBaseAtributes:: (DIM) Failed to read w,h pair from (%s)\n" , cstr));
      }
      int x = widget->Area().OuterArea().X();
      int y = widget->Area().OuterArea().Y();
      if (w < 0) {
         w = abs(w);
         x = x - w;
      }
      if (h < 0) {
         h = abs(h);
         y = y - h;
      }
      widget->SetWidgetPos(x,y);
      widget->SetWidgetDimensions(w,h);
   }
   
}



void ApplyTextAttributes(WidgetBase* widget ,  const std::map<std::string , std::string>& attribute_map) {
   
   BasicText* text_widget = dynamic_cast<BasicText*>(widget);
   if (!text_widget) {
      throw EagleError(StringPrintF("ApplyTextAttributes - widget %p is not a BasicText widget!!!\n" , widget));
   }
   
   map<string , string>::const_iterator cit = attribute_map.end();
   
   if ((cit = attribute_map.find("TEXT")) != attribute_map.end()) {
      text_widget->SetText(cit->second);
   }
   if ((cit = attribute_map.find("FONT")) != attribute_map.end()) {
      text_widget->SetFont(GetFont(cit->second));
   }
   if ((cit = attribute_map.find("SHRINKWRAP")) != attribute_map.end()) {
      if (strcmp(cit->second.c_str() , "YES") == 0) {
         text_widget->ShrinkWrap();
      }
   }
}



void SetWidgetBaseParameters(WidgetBase* widget , string widget_parameters) {
   map<string , string> attribute_map = ParseWidgetParameters(widget_parameters);
   ApplyWidgetBaseAttributes(widget , attribute_map);
}



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




