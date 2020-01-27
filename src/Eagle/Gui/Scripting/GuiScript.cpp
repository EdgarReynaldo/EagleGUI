
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
#include "Eagle/ObjectRegistry.hpp"
#include "Eagle/Gui/Scripting/GuiScript.hpp"
#include "Eagle/Gui/Scripting/Parsing.hpp"
#include "Eagle/Gui/Factory/WidgetFactory.hpp"
#include "Eagle/Gui/WidgetBase.hpp"
#include "Eagle/Gui/WidgetColorset.hpp"



#include <cstdio>
#include <cstring>


using namespace std;




/// -------------------------      EGSDeclaration     ------------------------------------



EGSDeclaration::EGSDeclaration() :
      class_name(""),
      object_name(""),
      attribute_value_set(""),
      attribute_value_map()
{}



EGSDeclaration::EGSDeclaration(std::string declaration) :
      class_name(""),
      object_name(""),
      attribute_value_set(""),
      attribute_value_map()
{
   ParseDeclaration(declaration);
}



void EGSDeclaration::Clear() {
   class_name = "";
   object_name = "";
   attribute_value_set = "";
   attribute_value_map.Clear();
}



bool EGSDeclaration::ParseDeclaration(std::string declaration) {
   Clear();
   /// CLASSNAME OBJNAME = ATTRIBUTE:VALUE;
   std::vector<std::string> split = SplitByDelimiterString(declaration , "=");
   if (split.size() != 2) {
      if (split.size() == 1) {
         EagleError() << StringPrintF("No right hand side in EGSdeclaration \"%s\"" , declaration.c_str()) << std::endl;
         return false;
      }
      else {
         EagleError() << StringPrintF("Too many equals signs in EGSdeclaration \"%s\"" , declaration.c_str()) << std::endl;
         return false;
      }
   }
   EAGLE_ASSERT(split.size() == 2);
   std::string lhs = split[0];
   std::string rhs = split[1];

   std::vector<std::string> names = SplitByDelimiterString(lhs , " ");
   if (names.size() != 2) {
      if (names.size() == 1) {
         EagleError() << StringPrintF("No object name in EGSdeclaration \"%s\"" , declaration.c_str()) << std::endl;
         return false;
      }
      else {
         EagleError() << StringPrintF("Too much whitespace in EGSdeclaration \"%s\"" , declaration.c_str()) << std::endl;
         return false;
      }
   }
   EAGLE_ASSERT(names.size() == 2);
   class_name = names[0];
   object_name = names[1];
   attribute_value_set = rhs;
   attribute_value_map = ParseAttributeSet(attribute_value_set);

   EAGLE_ASSERT(!attribute_value_map.Empty());

   return true;
}





/// ------------------------    EagleGuiScript     -----------------------------------




void EagleGuiScript::ClearScript() {
   script_file_path = "";
   script_file_contents = "";
   section_map.clear();
   declaration_lines.clear();
   for (int i = 0 ; i < (int)decs.size() ; ++i) {
      delete decs[i];
   }
   decs.clear();
   funcs.clear();
   class_dec_map.clear();
   if (colreg) {
      delete colreg;
      colreg = 0;
   }
   for (std::map<int , WidgetBase*>::iterator it = wmap.begin() ; it != wmap.end() ; ++it) {
      WidgetBase* w = it->second;
      delete w;
   }
   wmap.clear();
}



bool EagleGuiScript::ReadScriptFile(std::string script_file_name) {
   /// Read file into 'script'
   FILE* file = fopen(script_file_name.c_str() , "r");
   if (!file) {
      EagleError() << StringPrintF("EagleGuiScript::ReadScriptFile : Could not open file %s for reading.\n" , script_file_name.c_str());
      return false;
   }

   /// Get length
   int oldcount = 0 , count = 0;
   while(!feof(file) && !ferror(file)) {
      fgetc(file);
      ++count;
   }
   if (ferror(file)) {
      EagleError() << "EagleGuiScript::ReadScriptFile : error reading file size" << endl;
      fclose(file);
      return false;
   }

   rewind(file);

   script_file_contents.resize(count);
   oldcount = count;
   count = 0;
   while (!feof(file) && !ferror(file)) {
      script_file_contents[count++] = fgetc(file);
   }
   if (ferror(file)) {
      EagleError() << "EagleGuiScript::ReadScriptFile : error reading file" << endl;
      fclose(file);
      return false;
   }
   if (oldcount != count) {
      EagleError() << "EagleGuiScript::ReadScriptFile : Did not fully read file" << endl;
      fclose(file);
      return false;
   }

   fclose(file);

   script_file_path = script_file_name;

   return true;
}





EagleGuiScript::SECTION_MAP EagleGuiScript::ReadSections(std::string script_file) {

   SECTION_MAP smap;

   /// Read everything not in [section] tags into the current section
   std::string section = "Global";
   std::string content = "";

   unsigned int brace_start = script_file.find_first_of('[');
   unsigned int brace_stop = script_file.find_first_of(brace_start , ']');
   unsigned int content_start = brace_stop + 1;
   unsigned int content_stop = script_file.find_first_of(content_start , '[');

   /// Global content
   content = script_file.substr(0 , brace_start);
   smap[section] = content;

   /// Named sections
   while (brace_start != std::string::npos && brace_stop != std::string::npos) {
      section = script_file.substr(brace_start + 1 , brace_stop);
      content = script_file.substr(content_start , content_stop);
      smap[section] = content;
      brace_start = content_stop;
      brace_stop = script_file.find_first_of(brace_start , ']');
      content_start = brace_stop + 1;
      content_stop = script_file.find_first_of(content_start , '[');
   }

   return section_map;
}



std::vector<std::string> EagleGuiScript::GetSectionLines(std::string content) {
   std::vector<std::string> lines = SplitByNewLinesChomp(content);
   std::vector<std::string> adjusted;
   string current = lines[0];


   /// Assume there is a statement. If we get an empty line, we push it back. If there are more empty lines after that we skip them.

   int i = 0;
   do {
      /// Skip empty lines
      while (lines[i].compare("") == 0 && i < (int)lines.size()) {
         ++i;
      }
      /// We now have content, or EOF
      if (i == (int)lines.size()) {
         break;
      }
      current = "";
      while (lines[i].compare("") != 0 && i < (int)lines.size()) {
         if (lines[i][0] != '#') {
            current.append(lines[i]);
            current.append(";");
         }
         ++i;
      }
      adjusted.push_back(current);

   } while (i < (int)lines.size());

   return adjusted;
}



std::vector<EGSDeclaration*> EagleGuiScript::ParseDeclarations(std::vector<std::string> declarations) {
   DECLIST d(declarations.size());
   for (int i = 0 ; i < (int)declarations.size() ; ++i) {
      d[i] = new EGSDeclaration(declarations[i]);
   }
   return d;
}



EagleGuiScript::CLASS_DEC_MAP EagleGuiScript::MakeClassDeclarationMap() {
   /// Sorts eagle gui script declarations by class name
   CLASS_DEC_MAP dec_map;
   for (int i = 0 ; i < (int)decs.size() ; ++i) {
      EGSDeclaration* dec = decs[i];
      std::string class_name = dec->ClassName();
      if (class_name.compare("EagleColor") != 0 &&
          class_name.compare("WidgetColorset") != 0)
      {
         class_name = "WidgetBase";
      }
      if (dec_map.find(class_name) == dec_map.end()) {
         dec_map[class_name] = std::vector<EGSDeclaration*>();
      }
      dec_map[class_name].push_back(dec);
   }
   return dec_map;
}



bool EagleGuiScript::RegisterColors() {
   bool ret = true;
   DECLIST d = class_dec_map["EagleColor"];
   for (int i = 0 ; i < (int)d.size() ; ++i) {
      EGSDeclaration* egs_dec = d[i];
      EagleColor c;
      try {
         c = ParseColor(egs_dec->AttributeSet());
         colreg->RegisterColor(egs_dec->ObjectName() , c);
      }
      catch (...) {
         ret = false;
      }
   }
   return ret;
}



bool EagleGuiScript::RegisterColorsets() {
   bool ret = true;
   DECLIST d = class_dec_map["WidgetColorset"];
   for (int i = 0 ; i < (int)d.size() ; ++i) {
      EGSDeclaration* egs_dec = d[i];

      WidgetColorset wc;
      try {
         wc = ParseWidgetColorset(egs_dec->ValueMap());
         colreg->RegisterColorset(egs_dec->ObjectName() , wc);
      }
      catch (...) {
         ret = false;
      }
   }
   return ret;
}



bool EagleGuiScript::LoadWidgets() {
   bool ret = true;

   DECLIST d = class_dec_map["WidgetBase"];
   for (int i = 0 ; i < (int)d.size() ; ++i) {

      EGSDeclaration* egs_dec = d[i];

      WidgetBase* w = CreateWidget<WidgetBase>(egs_dec->ClassName() , egs_dec->ObjectName() , egs_dec->AttributeSet());
      if (!w) {
         EagleError() << StringPrintF("EagleGuiScript::LoadWidgets : Failed to load widget '%s' with attributes '%s'" ,
                                    egs_dec->ObjectName().c_str() , egs_dec->AttributeSet().c_str()) << endl;
         ret = false;
      }
      else {
         wmap[w->GetEagleId()] = w;
      }
   }
   return ret;
}




EagleGuiScript::EagleGuiScript() :
      script_file_path(""),
      script_file_contents(""),
      section_map(),
      declaration_lines(),
      decs(),
      funcs(),
      class_dec_map(),
      colreg(0),
      wmap()
{
   colreg = new ColorRegistry();
}



EagleGuiScript::~EagleGuiScript() {
   ClearScript();
}



bool EagleGuiScript::LoadScript(std::string script_file_name) {

   ClearScript();

   if (!ReadScriptFile(script_file_name)) {
      EagleError() << "EagleGuiScript::LoadScript : Failed to read script file." << endl;
      return false;
   }

   section_map = ReadSections(script_file_contents);

   SMIT it = section_map.find("Declarations");
   EAGLE_ASSERT(it != section_map.end());

   if (it != section_map.end()) {
      declaration_lines = GetSectionLines(it->second);
      decs = ParseDeclarations(declaration_lines);
   }

   it = section_map.find("Functions");
   if (it != section_map.end()) {
      /// TODO : IMPLEMENT ME
   }

   class_dec_map = MakeClassDeclarationMap();

   if (!RegisterColors()) {
      EagleError() << "EagleGuiScript::LoadScript : Failed to register EagleColors." << endl;
      return false;
   }
   if (!RegisterColorsets()) {
      EagleError() << "EagleGuiScript::LoadScript : Failed to register WidgetColorsets." << endl;
      return false;
   }
   if (!LoadWidgets()) {
      EagleError() << "EagleGuiScript::LoadScript : Failed to load widgets." << endl;
      return false;
   }

   return true;
}



bool EagleGuiScript::SaveScript() {
   return SaveScriptAs(script_file_path);
}



bool EagleGuiScript::SaveScriptAs(std::string script_file_name) {
   EAGLE_ASSERT(script_file_name.compare("") != 0);

   FILE* f = fopen(script_file_name.c_str() , "w");
   if (!f) {return false;}

   /// For each section

   /// For each declaration

   return true;
}



EagleColor* EagleGuiScript::GetColor(std::string name) {
   if (colreg->HasColor(name)) {
      return &colreg->GetColorByName(name);
   }
   return 0;
}



SHAREDOBJECT<WidgetColorset> EagleGuiScript::GetColorset(std::string name) {
   if (colreg->HasColorset(name)) {
      return colreg->GetColorsetByName(name);
   }
///   throw EagleException(StringPrintF("EagleGuiScript::GetColorset - color registry does not have colorset %s\n." , name.c_str()));
   return HeapObject((WidgetColorset*)0);/// TODO : to throw or not to throw
}



WidgetBase* EagleGuiScript::GetWidget(std::string name) {
   return dynamic_cast<WidgetBase*>(GetFirstObjectByName(name));
}





