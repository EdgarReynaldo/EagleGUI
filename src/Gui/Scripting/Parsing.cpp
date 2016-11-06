



#include "Eagle/StringWork.hpp"
#include "Eagle/Gui/Scripting/Parsing.hpp"


using namespace std;



/// Attributes are stored in semi-colon separated strings
/// Specific attributes are stored in two colon separated strings
/// Attributes and values may be surrounded by padding whitespace for readability
/// IE. " SUBCLASS : RadioButton ; POS : 100,50 ; DIM : 200,100 ; SDCOL : 0,64,0 ;"

map<string , string> ParseAttributeSet(string widget_parameters) throw EagleError {
   
   map<string , string> attribute_map;
   
   vector<string> attributes = SplitByDelimiterString(widget_parameters , ";");
   
   for (size_t i = 0 ; i < attributes.size() ; ++i) {
      string attribute_str = attributes[i];
      vector<string> attribute_pair = SplitByDelimiterString(attribute_str , ":");
      if (attribute_pair.size() != 2) {
         throw EagleError(StringPrintF("ParseAttributeSet : Illegal attribute pair (%s) of size %u found in attribute %u\n",
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




