



#include "Eagle/StringWork.hpp"
#include "Eagle/Gui/Scripting/Parsing.hpp"

#include <cstdlib>
#include <cstring>

using namespace std;



/// Attribute value pairs are stored in semi-colon separated strings
/// Specific attributes are stored in two colon separated strings
/// Attributes and values may be surrounded by padding whitespace for readability
/// IE. " SUBCLASS : RadioButton ; POS : 100,50 ; DIM : 200,100 ; SDCOL : 0,64,0 ;"

map<string , string> ParseAttributeSet(string widget_parameters) throw (EagleException) {
   
   map<string , string> attribute_map;
   
   vector<string> attributes = SplitByDelimiterString(widget_parameters , ";");
   
   for (size_t i = 0 ; i < attributes.size() ; ++i) {
      string attribute_str = attributes[i];
      vector<string> attribute_pair = SplitByDelimiterString(attribute_str , ":");
      if (attribute_pair.size() != 2) {
         throw EagleException(StringPrintF("ParseAttributeSet : Illegal attribute pair (%s) of size %u found in attribute %u\n",
                                       attribute_str.c_str() , attribute_pair.size() , i));
      }
      string attribute = attribute_pair[0];
      string value = attribute_pair[1];
      char* attstr = CStrDup(attribute.c_str());
      char* valstr = CStrDup(value.c_str());
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





EagleColor ParseColor(std::string color_dec) throw (EagleException) {
   EagleColor c;
   int r,g,b,a;
   float fr,fg,fb,fa;
   /// Declaration follows format [RGB | RGBA | FRGB | FRGBA](%n,%n,%n[,%n])
   vector<string> args = SplitByDelimiterString(color_dec , "(");
   
   string val = args[1];
   
   char buf[256];
   memset(buf , 0 , 256);
   
   if (1 != sscanf(args[0].c_str() , "%5s" , buf)) {
      throw EagleException(StringPrintF("ParseEagleColor : Failed to read attribute name from string '%s'\n" , args[0].c_str()));
   }
   if (strncmp(buf , "FRGBA" , 5) == 0) {
      if (4 == sscanf(val.c_str() , "%f,%f,%f,%f" , &fr , &fg , &fb , &fa)) {
         c.SetFloatColor(fr,fg,fb,fa);
      }
      else {
         throw EagleException(StringPrintF("ParseEagleColor : Failed to parse FRGBA from value string '%s'\n" , val.c_str()));
      }
   }
   else if (strncmp(buf , "FRGB" , 4) == 0) {
      if (3 == sscanf(val.c_str() , "%f,%f,%f" , &fr , &fg , &fb)) {
         c.SetFloatColor(fr,fg,fb);
      }
      else {
         throw EagleException(StringPrintF("ParseEagleColor : Failed to parse FRGB from value string '%s'\n" , val.c_str()));
      }
   }
   else if (strncmp(buf , "RGBA" , 4) == 0) {
      if (4 == sscanf(val.c_str() , "%d,%d,%d,%d" , &r , &g , &b , &a)) {
         c.SetColor(r,g,b,a);
      }
      else {
         throw EagleException(StringPrintF("ParseEagleColor : Failed to parse RGBA from value string '%s'\n" , val.c_str()));
      }
   }
   else if (strncmp(buf , "RGB" , 3) == 0) {
      if (3 == sscanf(val.c_str() , "%d,%d,%d" , &r , &g , &b)) {
         c.SetColor(r,g,b);
      }
      else {
         throw EagleException(StringPrintF("ParseEagleColor : Failed to parse RGB from value string '%s'\n" , val.c_str()));
      }
   }
   else {
      throw EagleException("ParseEagleColor : Failed to parse valid attribute for EagleColor from value given.\n");
   }
   return c;
}



WidgetColorset ParseWidgetColorset(const ATTRIBUTE_VALUE_MAP& avmap) throw (EagleException) {
   WidgetColorset wc;
   for (ATTRIBUTE_VALUE_MAP::const_iterator it = avmap.begin() ; it != avmap.end() ; ++it) {
      std::string att = it->first;
      std::string val = it->second;
      int color_index = -1;
      if (strncmp(val.c_str() , "SDCOL" , 5) == 0) {
         color_index = SDCOL;
      }
      else if (strncmp(val.c_str() , "BGCOL" , 5) == 0) {
         color_index = BGCOL;
      }
      else if (strncmp(val.c_str() , "MGCOL" , 5) == 0) {
         color_index = MGCOL;
      }
      else if (strncmp(val.c_str() , "FGCOL" , 5) == 0) {
         color_index = FGCOL;
      }
      else if (strncmp(val.c_str() , "HLCOL" , 5) == 0) {
         color_index = HLCOL;
      }
      else if (strncmp(val.c_str() , "TXTCOL" , 6) == 0) {
         color_index = TXTCOL;
      }
      if (color_index != -1) {
         EagleColor c;
         try {
            c = ParseColor(val);
         }
         catch (...) {
            char color_name[256];
            memset(color_name , 0 , 256);
            if ((1 == sscanf(val.c_str() , "%255s" , color_name) && HasColor(color_name))) {
               c = GetColorByName(color_name);
            }
            else {
               throw EagleException(StringPrintF("ParseWidgetColorset : Failed to parse color for value '%s'\n" , val.c_str()));
            }
         }
         wc[color_index] = c;
      }
      else {
         /// No known attribute found
         throw EagleException(StringPrintF("ParseWidgetColorset : Failed to parse attribute '%s' for colorset.\n" , att.c_str()));
      }
   }
   return wc;
}




/**
EagleColor ParseEagleColor(const ATTRIBUTE_VALUE_MAP& avmap) throw EagleException {
   EagleColor c;
   int r,g,b,a;
   float fr,fg,fb,fa;
   AVMIT it = avmap.end();
   if ((it = avmap.find("RGB")) != avmap.end()) {
      string val = it->second;
      if (3 == sscanf(val.c_str() , "%d,%d,%d" , &r , &g , &b)) {
         c.SetColor(r,g,b);
      }
      else {
         throw EagleException(StringPrintF("Failed to parse RGB from value string '%s'\n" , val.c_str()));
      }
   }
   else if ((it = avmap.find("RGBA")) != avmap.end()) {
      string val = it->second;
      if (4 == sscanf(val.c_str() , "%d,%d,%d,%d" , &r , &g , &b , &a)) {
         c.SetColor(r,g,b,a);
      }
      else {
         throw EagleException(StringPrintF("Failed to parse RGBA from value string '%s'\n" , val.c_str()));
      }
   }
   else if ((it = avmap.find("FRGB")) != avmap.end()) {
      string val = it->second;
      if (3 == sscanf(val.c_str() , "%f,%f,%f" , &fr , &fg , &fb)) {
         c.SetFloatColor(fr,fg,fb);
      }
      else {
         throw EagleException(StringPrintF("Failed to parse FRGB from value string '%s'\n" , val.c_str()));
      }
   }
   else if ((it = avmap.find("FRGBA")) != avmap.end()) {
      string val = it->second;
      if (4 == sscanf(val.c_str() , "%f,%f,%f,%f" , &fr , &fg , &fb , &fa)) {
         c.SetFloatColor(fr,fg,fb,fa);
      }
      else {
         throw EagleException(StringPrintF("Failed to parse FRGB from value string '%s'\n" , val.c_str()));
      }
   }
   else {
      throw EagleException("Failed to parse valid attribute for EagleColor from value given.\n");
   }
}
//*/



