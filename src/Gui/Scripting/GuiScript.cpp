



#include "Eagle/Gui/Scripting/GuiScript.hpp"
#include "Eagle/Gui/Scripting/Parsing.hpp"
#include "Eagle/StringWork.hpp"
#include "Eagle/Gui/Factory/WidgetFactory.hpp"
#include "Eagle/Gui/WidgetBase.hpp"


#include <cstdio>
#include <cstring>


using namespace std;




/// -------------------------      EGSDeclaration     ------------------------------------



void EGSDeclaration::Clear() {
   class_name = "";
   object_name = "";
   attribute_value_set = "";
}





/// ------------------------    EagleGuiScript     -----------------------------------



void EagleGuiScript::ClearScript() {
   script = "";
   lines.clear();
   decs.clear();
///   funcs.clear();
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
   
   script.resize(count);
   oldcount = count;
   count = 0;
   while (!feof(file) && !ferror(file)) {
      script[count++] = fgetc(file);
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
   return true;
}



bool EagleGuiScript::ReadSections() {
   bool declarations_found = false;
///   bool functions_found = false;
   bool in_declarations = false;
   bool in_functions = false;
   bool in_declaration = false;
   bool in_function = false;
   
   EGSDeclaration egs_dec;
   
   for (int i = 0 ; i < (int)lines.size() ; ++i) {
      if (strncmp(lines[i].c_str() , "[Declarations]" , 14) == 0) {
         in_declarations = true;
         in_functions = false;
         declarations_found = true;
      }
      else if (strncmp(lines[i].c_str() , "[Functions]" , 11) == 0) {
         in_functions = true;
         in_declarations = false;
///         functions_found = true;
      }
      else if (strncmp(lines[i].c_str() , "\n" , 1) == 0) {/// TODO : Test if this actually catches newlines
         if (in_declarations && in_declaration) {
            decs.push_back(egs_dec);
         }
         if (in_functions && in_function) {
            EagleError() << "EagleGuiScript::ReadSections : Not pushing back function. Implement me." << endl;
         }
         /// A newline ends a declaration or a function
         in_declaration = false;
         in_function = false;
         continue;
      }
      else {
         if (in_declarations) {
            /// On first line, read class name and object name and optional attribute value pair set
            /// separated by an equals sign
            int equals_index = lines[i].find_first_of('=');
            if (equals_index != (int)string::npos) {
               
               if (in_declaration) {
                  /// User started another declaration, this completes the previous declaration
                  decs.push_back(egs_dec);
               }
               /// Found declaration
               in_declaration = true;

               if ((int)lines[i].find_last_of('=') != equals_index) {
                  EagleError() << "EagleGuiScript::ReadSections - declaration line contains multiple = operators!" << endl;
                  return false;
               }
               egs_dec.Clear();
               vector<string> substrs = SplitByDelimiterString(lines[i] , "=");
               EAGLE_ASSERT(substrs.size() == 2);
               std::string dec = substrs[0];
               std::string attset = substrs[1];
               
               /// Parse class name and object name from dec string
               char class_buf[1024];
               char object_buf[1024];
               memset(class_buf , 0 , 1024);
               memset(object_buf , 0 , 1024);
               
               if (2 != sscanf(dec.c_str() , "%s%s" , class_buf , object_buf)) {
                  EagleError() << StringPrintF("Failed to read class and object name from declaration string '%s'" , dec.c_str()) << endl;
                  return false;
               }
               
               egs_dec.class_name = class_buf;
               egs_dec.object_name = object_buf;
               
               /// Push right hand side back onto attribute set
               if (attset.find_last_of(";") != (attset.size() - 1)) {
                  attset.push_back(';');
               }
               egs_dec.attribute_value_set += attset;
               
            }
            else {
               /// Continuing declaration, push line back onto attribute set with separating semi-colon
               std::string attset = lines[i];
               if (attset.find_last_of(";") != (attset.size() - 1)) {
                  attset.push_back(';');
               }
               egs_dec.attribute_value_set += attset;
            }
         }
         else if (in_functions) {
            EagleWarn() << "EagleGuiScript::ReadSections : Function line ignored. Implement me." << endl;
         }
      }
   }
   return declarations_found;
   /// TODO : Implement functions
   /// TODO : return declarations_found && functions_found;
}



void EagleGuiScript::MapClasses() {
   class_dec_map.clear();
   for (int i = 0 ; i < (int)decs.size() ; ++i) {
      class_dec_map.insert(std::pair<std::string , EGSDeclaration*>(decs[i].class_name , &decs[i]));
   }
}



bool EagleGuiScript::RegisterColors() {
   bool ret = true;
   std::pair<CDMIT , CDMIT> range = class_dec_map.equal_range("EagleColor");
   for (CDMIT it = range.first ; it != range.second ; ++it) {
      EGSDeclaration* egs_dec = it->second;
      EagleColor c;
      try {
         c = ParseColor(egs_dec->attribute_value_set);
         RegisterColor(egs_dec->object_name , c);
      }
      catch (...) {
         ret = false;
      }
   }
   return ret;
}



bool EagleGuiScript::RegisterColorsets() {
   bool ret = true;
   std::pair<CDMIT , CDMIT> range = class_dec_map.equal_range("WidgetColorset");
   for (CDMIT it = range.first ; it != range.second ; ++it) {
      EGSDeclaration* egs_dec = it->second;
      ATTRIBUTE_VALUE_MAP avmap = ParseAttributeSet(egs_dec->attribute_value_set);
      WidgetColorset wc;
      try {
         wc = ParseWidgetColorset(avmap);
         RegisterColorset(egs_dec->object_name , wc);
         GetColorsetByName(egs_dec->object_name).SetName(egs_dec->object_name);
      }
      catch (...) {
         ret = false;
      }
   }
   return ret;
}



bool EagleGuiScript::LoadWidgets() {
   bool ret = true;
   for (CDMIT it = class_dec_map.begin() ; it != class_dec_map.end() ; ++it) {
      std::string class_name = it->first;
      if (class_name.compare("EagleColor") == 0 || class_name.compare("WidgetColorset") == 0) {
         continue;
      }
      EGSDeclaration* egs_dec = it->second;
      WidgetBase* w = CreateWidget<WidgetBase>(class_name , egs_dec->attribute_value_set);
      if (!w) {
         EagleError() << StringPrintF("EagleGuiScript::LoadWidgets : Failed to load widget '%s' with attributes '%s'" ,
                                    class_name.c_str() , egs_dec->attribute_value_set.c_str()) << endl;
         ret = false;
      }
      else {
         w->SetName(egs_dec->object_name);
      }
   }
   return ret;
}



bool EagleGuiScript::LoadScript(std::string script_file_name) {
   
   ClearScript();
   
   if (!ReadScriptFile(script_file_name)) {
      EagleError() << "EagleGuiScript::LoadScript : Failed to read script file." << endl;
      return false;
   }
   
   lines = SplitByDelimiterString(script , "\n");
   
   if (!ReadSections()) {
      EagleError() << "EagleGuiScript::LoadScript : Failed to read sections from script file." << endl;
      return false;
   }
   
   MapClasses();
   
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





