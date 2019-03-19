
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



#include "Eagle/StringWork.hpp"
#include "Eagle/Gui/Scripting/Parsing.hpp"

#include <cstdlib>
#include <cstring>

using namespace std;



/// Attribute value pairs are stored in semi-colon separated strings
/// Specific attributes are stored in two colon separated strings
/// Attributes and values may be surrounded by padding whitespace for readability
/// IE. " SUBCLASS : RadioButton ; POS : 100,50 ; DIM : 200,100 ; SDCOL : 0,64,0 ;"

AttributeValueMap ParseAttributeSet(string widget_parameters) {/// throw (EagleException) {

   AttributeValueMap attribute_map;

   vector<string> attributes = SplitByDelimiterString(widget_parameters , ";");

   for (size_t i = 0 ; i < attributes.size() ; ++i) {
      string attribute_str = attributes[i];
      vector<string> attribute_pair = SplitByDelimiterString(attribute_str , ":");
      if (attribute_pair.size() != 2) {
         throw EagleException(StringPrintF("ParseAttributeSet : Illegal attribute pair (%s) of size %d found in attribute %dbw\n",
                                       attribute_str.c_str() , (int)attribute_pair.size() , (int)i));
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




EagleColor ParseColor(const std::string color_dec) {
   /// Declaration follows format [RGB | RGBA | FRGB | FRGBA](%n,%n,%n[,%n]) eg. RGB(255,255,255) or FRGBA(1.0,1.0,1.0,1.0)
   EagleColor c;
   int r,g,b,a;
   float fr,fg,fb,fa;
   
   if (HasColor(color_dec)) {
      return GetColorByName(color_dec);
   }
   
   std::string type = ReadStringUntil(color_dec , '(');
   std::string args = GetArgumentList(color_dec , '(' , ')');

   if (strncmp(type.c_str() , "FRGBA" , 5) == 0) {
      if (4 == sscanf(args.c_str() , "%f,%f,%f,%f" , &fr , &fg , &fb , &fa)) {
         c.SetFloatColor(fr,fg,fb,fa);
      }
      else {
         throw EagleException(StringPrintF("ParseEagleColor : Failed to parse FRGBA from value string '%s'\n" , args.c_str()));
      }
   }
   else if (strncmp(type.c_str() , "FRGB" , 4) == 0) {
      if (3 == sscanf(args.c_str() , "%f,%f,%f" , &fr , &fg , &fb)) {
         c.SetFloatColor(fr,fg,fb);
      }
      else {
         throw EagleException(StringPrintF("ParseEagleColor : Failed to parse FRGB from value string '%s'\n" , args.c_str()));
      }
   }
   else if (strncmp(type.c_str() , "RGBA" , 4) == 0) {
      if (4 == sscanf(args.c_str() , "%d,%d,%d,%d" , &r , &g , &b , &a)) {
         c.SetColor(r,g,b,a);
      }
      else {
         throw EagleException(StringPrintF("ParseEagleColor : Failed to parse RGBA from value string '%s'\n" , args.c_str()));
      }
   }
   else if (strncmp(type.c_str() , "RGB" , 3) == 0) {
      if (3 == sscanf(args.c_str() , "%d,%d,%d" , &r , &g , &b)) {
         c.SetColor(r,g,b);
      }
      else {
         throw EagleException(StringPrintF("ParseEagleColor : Failed to parse RGB from value string '%s'\n" , args.c_str()));
      }
   }
   else {
      throw EagleException("ParseEagleColor : Failed to parse valid attribute for EagleColor from value given.\n");
   }
   return c;
}



WidgetColorset ParseWidgetColorset(const AttributeValueMap& avmap) {
   WidgetColorset wc;
   const ATTVALMAP& avpairs = avmap.GetAttributeValueMap();
   for (ATTVALMAP::const_iterator it = avpairs.begin() ; it != avpairs.end() ; ++it) {
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
         wc[color_index] = ParseColor(val);
      }
      else {
         continue;/// No need to throw an error - there may be other attributes in the map, just ignore them
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



