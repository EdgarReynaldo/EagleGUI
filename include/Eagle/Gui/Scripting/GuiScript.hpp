
/**
 *
 *         _______       ___       ____      __       _______
 *        /\  ____\    /|   \     /  __\    /\ \     /\  ____\
 *        \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_
 *         \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\
 *          \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_
 *           \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *            \/______/|/_/  \/_/  \_\_____/    \/_____/ \/______/
 *
 *
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2021+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file GuiScript.hpp
 * @brief The interface for working with .egs eagle gui script files (text files)
 *
 *  TODO : This is a Work In Progress^TM, it is not complete
 *
 *  Specification for an EagleGuiScript file (.egs). File is text, with declarations for EagleColor's, WidgetColorsets, and Widgets
 *
 *  Currently, there are two sections available in an .egs script. There is a "Declarations" section and a "Functions" section.
 *
 *  Declarations follow the format of a class name followed by whitespace then by the object name, optional whitespace,
 *  and an equals sign. The following lines consist of attribute name and value pairs separated by a colon.
 *  Each line is treated as a set of attribute value pairs. You may separate these pairs by a newline or by a semi colon,
 *  but they must be kept separate. Atrributes and values may be surrounded by whitespace, which will be trimmed.
 *  A newline by itself signals the end of the object definition. Generally, value strings must not contain whitespace.
 *  Anything after an equals sign and before a separate newline will be parsed as attribute value pair sets.
 *
 *  Supported class names include EagleColor, WidgetColorset, or any Widget class name
 *
 *  New section named "Commands" which includes commands to create relationships between widgets
 * 
 *  [Declarations]
 *
 *  CLASSNAME OBJECTNAME = ATTRIBUTE:VALUE ; ATTRIBUTE:VALUE;
 *  ATTRIBUTE : VALUE
 *
 *  CLASSNAME OBJECTNAME =
 *  ATTRIBUTE : VALUE ; ATTRIBUTE : VALUE
 *
 *  EagleColor white = RGBA(255,255,255,255)
 *
 *  WidgetColorset colorset1 =
 *  SDCOL : RGBA(0,0,0,255)
 *  HLCOL : FRGBA(1.0,1.0,1.0,1.0)
 *  TXTCOL : white
 *
 * [Commands]
 *  LAYOUT_CHILDREN root_layout = {abc,def,ghi,jkl,mno,pqr,stu,vwx,yz}
 *  GUI_LAYOUT gui = root_layout
 *
 *  LAYOUT_CHILD_RELATIVE root_layout = widget:%f,%f,%f,%f
 *
 *  METHOD OBJECT = ARGUMENTS
 *
 *  [Functions] TODO : Implment GuiScript2 scriptability
 */

#ifndef GuiScript_HPP
#define GuiScript_HPP



#include "Eagle/Gui/WidgetAttributes.hpp"
#include "Eagle/Gui/WidgetColorset.hpp"



#include <string>
#include <vector>
#include <map>



class EGSDeclaration {

   std::string class_name;
   std::string object_name;
   std::string attribute_value_set;

   AttributeValueMap attribute_value_map;


public :

   EGSDeclaration();
   EGSDeclaration(std::string declaration);

   void Clear();

   bool ParseDeclaration(std::string declaration);

   std::string ClassName() const {return class_name;}
   std::string ObjectName() const {return object_name;}
   std::string AttributeSet() const {return attribute_value_set;}
   AttributeValueMap ValueMap() const {return attribute_value_map;}
};


class WidgetBase;

class EagleGuiScript {

protected:

   typedef std::vector<EGSDeclaration*> DECLIST;

   typedef std::map<std::string , std::string> SECTION_MAP;
   typedef SECTION_MAP::iterator SMIT;

   typedef std::map<std::string , DECLIST> CLASS_DEC_MAP;
   typedef CLASS_DEC_MAP::iterator CDMIT;




   std::string script_file_path;

   std::string script_file_contents;

   SECTION_MAP section_map;

   std::vector<std::string> declaration_lines;

   std::vector<EGSDeclaration*> decs;
   std::vector<std::string> funcs;/// Once our script has scripting capabilities

   CLASS_DEC_MAP class_dec_map;

   ColorRegistry* colreg;

   std::map<int , WidgetBase*> wmap;




   void ClearScript();

   bool ReadScriptFile(std::string script_file_name);

   SECTION_MAP ReadSections(std::string script_file);

   std::vector<std::string> GetSectionLines(std::string content);

   DECLIST ParseDeclarations(std::vector<std::string> declarations);

   CLASS_DEC_MAP MakeClassDeclarationMap();

   bool RegisterColors();
   bool RegisterColorsets();

   bool LoadWidgets();
/*
   bool RunCommands();
   bool RunCommands() {
      std::vector<std::string> lines = GetSectionLines(section_map["Commands"]);

      for (int i = 0 ; i < (int)lines.size() ; ++i) {
         std::string l = lines[i];
         std::vector<std::string>
      }

   }
*/
public :

   EagleGuiScript();
   ~EagleGuiScript();

   bool LoadScript(std::string script_file_name);

///   bool SaveScript(std::string script_file_name);
   bool SaveScript();/// Saves over previously loaded script - danger!
   bool SaveScriptAs(std::string script_file_name);

   EagleColor* GetColor(std::string name);
   SHAREDOBJECT<WidgetColorset> GetColorset(std::string name);

   WidgetBase* GetWidget(std::string name);




};



#endif // GuiScript_HPP
