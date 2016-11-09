



#ifndef GuiScript_HPP
#define GuiScript_HPP


#include <string>
#include <vector>
#include <map>

/**

Specification for an EagleGuiScript file (.egs). File is text, with declarations for EagleColor's, WidgetColorsets, and Widgets

Currently, there are two sections available in an .egs script. There is a "Declarations" section and a "Functions" section.

Declarations follow the format of a class name followed by whitespace then by the object name, optional whitespace, 
and an equals sign. The following lines consist of attribute name and value pairs separated by a colon.
Each line is treated as a set of attribute value pairs. You may separate these pairs by a newline or by a semi colon,
but they must be kept separate. Atrributes and values may be surrounded by whitespace, which will be trimmed.
A newline by itself signals the end of the object definition. Generally, value strings must not contain whitespace.
Anything after an equals sign and before a separate newline will be parsed as attribute value pair sets.

Supported class names include EagleColor, WidgetColorset, or any Widget class name

[Declarations]

CLASS NAME = ATTRIBUTE:VALUE ; ATTRIBUTE:VALUE;
ATTRIBUTE : VALUE

CLASS NAME =
ATTRIBUTE : VALUE ; ATTRIBUTE : VALUE

EagleColor white = RGBA:255,255,255,255;

WidgetColorset colorset1 = 
SDCOL : 0,0,0,255
...
TXTCOL : 255,255,255,255

[Functions]


*/

class EGSDeclaration {

   std::string class_name;
   std::string object_name;
   std::string attribute_value_set;

   friend class EagleGuiScript;
   
public :
   
   void Clear();
   
};






class EagleGuiScript {
   
   std::string script;
   
   std::vector<std::string> lines;

   std::vector<EGSDeclaration> decs;
///   std::vector<std::string> funcs;
   
   typedef std::multimap<std::string , EGSDeclaration*> CLASS_DEC_MAP; 
   typedef CLASS_DEC_MAP::iterator CDMIT;
   
   CLASS_DEC_MAP class_dec_map;

   

   void ClearScript();
   bool ReadScriptFile(std::string script_file_name);
   bool ReadSections();
   
   void MapClasses();
   
   bool RegisterColors();
   bool RegisterColorsets();

   bool LoadWidgets();

public :
   
   bool LoadScript(std::string script_file_name);
   
};



#endif // GuiScript_HPP
