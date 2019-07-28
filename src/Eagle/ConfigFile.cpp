
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




#include "Eagle/ConfigFile.hpp"

#include "Eagle/Lib.hpp"
#include "Eagle/System.hpp"
#include "Eagle/FileSystem.hpp"
#include "Eagle/Exception.hpp"
#include "Eagle/StringWork.hpp"


#include <sstream>
#include <fstream>



/// --------------------------     ConfigLine      -------------------------



void ConfigLine::ParseLine() {
   spacer = !line.size();
   comment = false;
   key = "";
   value = "";
   if (!spacer) {
      if (line[0] == '#') {
         comment = true;
      }
   }
   if (!comment && !spacer) {
      unsigned int j = line.find_first_of('=');
      EAGLE_ASSERT(line.find_first_of('=') != std::string::npos);
      if (j != std::string::npos) {
         /// Should have a key value pair
         key = line.substr(0 , j);
         value = line.substr(j + 1);
         while (j > 0 && isspace(key[j-1])) {--j;}
         key = key.substr(0 , j);/// Trim trailing white space off of key
         j = 0;
         while (j < value.size() && isspace(value[j])) {++j;}/// Trim leading whitespace off of value
         value = value.substr(j);
      }
   }
}



ConfigLine::ConfigLine() :
      comment(false),
      spacer(true),
      line(""),
      key(""),
      value("")
{}



ConfigLine::ConfigLine(std::string ln) :
      comment(false),
      spacer(true),
      line(""),
      key(""),
      value("")
{
   SetLine(ln);
}



void ConfigLine::SetLine(std::string ln) {
   line = ln;
   ParseLine();
}



void ConfigLine::SetKeyAndValue(std::string k , std::string v) {
   SetLine(k + " = " + v);
}



void ConfigLine::SetKey(std::string k) {
   SetKeyAndValue(k , value);
}



void ConfigLine::SetValue(std::string v) {
   SetKeyAndValue(key , v);
}



std::string ConfigLine::Line() {
   if (comment || spacer) {
      return line;
   }
   return key + " = " + value;
}



/// --------------------------       ConfigSection       -----------------------




std::vector<ConfigLine*>::iterator ConfigSection::GetConfigIterator(std::string key) {
   std::vector<ConfigLine*>::iterator it = clines.begin();
   /// linear search, lame I know
   while (it != clines.end()) {
      ConfigLine* c = *it;
      if (c->IsComment()) {
         ++it;
         continue;
      }
      if (key.compare(c->Key()) == 0) {
         return it;
      }
      ++it;
   }
   return it;
}



std::vector<ConfigLine*>::const_iterator ConfigSection::GetConfigIteratorConst(std::string key) const {
   std::vector<ConfigLine*>::const_iterator it = clines.begin();
   /// linear search, lame I know
   while (it != clines.end()) {
      ConfigLine* c = *it;
      if (!c->IsKeyValuePair()) {
         ++it;
         continue;
      }
      if (key.compare(c->Key()) == 0) {
         return it;
      }
      ++it;
   }
   return it;
}



ConfigSection::ConfigSection() :
      clines()
{}



ConfigLine* ConfigSection::FindConfig(std::string key) {
   std::vector<ConfigLine*>::iterator it = GetConfigIterator(key);
   if (it != clines.end()) {
      return *it;
   }
   return 0;
}



const ConfigLine* ConfigSection::FindConfigConst(std::string key) const {
   std::vector<ConfigLine*>::const_iterator it = GetConfigIteratorConst(key);
   if (it != clines.end()) {
      return *it;
   }
   return 0;
}



ConfigLine* ConfigSection::GetConfigByKey(std::string key) {
   ConfigLine* c = FindConfig(key);
   if (!c) {
      /// Key not found
      c = new ConfigLine;
      c->SetKey(key);
      clines.push_back(c);
   }
   return c;
}



void ConfigSection::SetKeyValuePair(std::string key , std::string value) {
   ConfigLine* c = GetConfigByKey(key);
   c->SetValue(value);
}



void ConfigSection::RemoveLineByKey(std::string key) {
   std::vector<ConfigLine*>::iterator it = GetConfigIterator(key);
   if (it != clines.end()) {
      clines.erase(it);
   }
}



std::string& ConfigSection::operator[](std::string key) {
   ConfigLine* cl = GetConfigByKey(key);
   return cl->Value();
}



const std::string& ConfigSection::operator[] (std::string key) const {
   static std::string empty;
   const ConfigLine* cl = FindConfigConst(key);
   return cl?cl->Value():empty;
}



void ConfigSection::AddSpacer() {
   clines.push_back(new ConfigLine(""));
}



void ConfigSection::AddComment(std::string comment) {
   AddConfigLine(std::string("# ") + comment);
}



void ConfigSection::AddConfigLine(std::string line) {
   clines.push_back(new ConfigLine(line));
}



void ConfigSection::AddConfigLine(std::string key , std::string value) {
   GetConfigByKey(key)->SetValue(value);
}



std::string ConfigSection::GetConfigLine(int index) const {
   if (index >= 0 && index < (int)clines.size()) {
      return clines[index]->Line();
   }
   EAGLE_ASSERT(index >= 0 && index < (int)clines.size());
   return "";
}



unsigned int ConfigSection::NConfigLines() const {
   return clines.size();
}



std::vector<std::string> ConfigSection::GetKeys() const {
   std::vector<std::string> keys;
   for (unsigned int i = 0 ; i < clines.size() ; ++i) {
      ConfigLine* cl = clines[i];
      if (cl->IsKeyValuePair()) {
         keys.push_back(cl->Key());
      }
   }
   return keys;
}



/// --------------------------      ConfigFile      --------------------------------



void ConfigFile::UpdateContents() {
   contents = "";
   std::stringstream ss;
   SMIT it = sectionmap.begin();
   while (it != sectionmap.end()) {
      /// Write section header here
      ss << "[" << it->first << "]" << std::endl;
      
      ConfigSection& cs = it->second;
      for (int i = 0 ; i < (int)cs.NConfigLines() ; ++i) {
         ss << cs.GetConfigLine(i) << std::endl;
      }
      ss << std::endl;
      ++it;
   }
   ss << std::endl;
   contents = ss.str();
   ss.clear();
}



ConfigFile::ConfigFile() :
      contents(""),
      sectionmap()
{}



void ConfigFile::Clear() {
   sectionmap.clear();
   contents = "";
}



bool ConfigFile::LoadFromFile(const char* path) {

   Clear();
   
   EagleSystem* sys = Eagle::EagleLibrary::System("Any");
   
   EAGLE_ASSERT(sys);

   FSInfo finfo = sys->GetFileSystem()->GetFileInfo(std::string(path));
   
   std::string fpath = finfo.Path();
   
   if (!finfo.Exists()) {
      EagleWarn() << StringPrintF("ConfigFile::LoadFromFile - failed to load file on path '%s'\n" , fpath.c_str());
      return false;
   }
   if (!finfo.Mode().IsFile()) {
      EagleError() << StringPrintF("ConfigFile::LoadFromFile - path '%s' does not refer to a file!\n" , fpath.c_str());
      return false;
   }
   
   MemFile mem(finfo);
   if (!mem.ReadFileIntoMemory()) {
      return false;
   }
   
   contents.insert(contents.begin() , mem.Begin() , mem.End());
   
   mem.Clear();
   
   std::vector<std::string> lines = SplitByNewLines(contents);
   
   ConfigSection* section = &sectionmap["GLOBAL"];
   for (int i = 0 ; i < (int)lines.size() ; ++i) {
      std::string l = lines[i];
      
      unsigned int idx1 = l.find_first_of('[');
      unsigned int idx2 = l.find_first_of(']');
      if (idx1 != std::string::npos && idx2 != std::string::npos && idx1 < idx2 && idx1 == 0) {
         /// Found a section name
         std::string section_str = l.substr(idx1 + 1 , idx2 - idx1 - 1);
         section = &sectionmap[section_str];
      }
      else {
         section->AddConfigLine(l);
      }
   }
   return true;
}



bool ConfigFile::SaveToFile(const char* path) {

   EagleSystem* sys = Eagle::EagleLibrary::System("Any");
   
   EAGLE_ASSERT(sys);

   FSInfo finfo = sys->GetFileSystem()->GetFileInfo(std::string(path));
   
   if (finfo.Exists() && !finfo.Mode().CanWrite()) {
      throw EagleException(StringPrintF("ConfigFile::SaveToFile - file %s is read only!\n" , path));
   }
   
   UpdateContents();
   
   std::ofstream fout(finfo.Path() , std::ios_base::out);
   if (!fout.good()) {
      return false;
   }
   fout << contents << std::endl;
   fout.close();
   return true;
}



void ConfigFile::Absorb(const ConfigFile& c) {
   if (&c == this) {return;}
   SECTIONMAP::const_iterator cit = c.sectionmap.begin();
   while (cit != c.sectionmap.end()) {
      const ConfigSection& cs = cit->second;
      std::vector<std::string> keys = cs.GetKeys();
      for (unsigned int i = 0 ; i < keys.size() ; ++i) {
         sectionmap[cit->first][keys[i]] = cs[keys[i]];
      }
      ++cit;
   }
}



ConfigSection* ConfigFile::FindSection(std::string section) {
   SMIT it = sectionmap.find(section);
   if (it != sectionmap.end()) {
      return &(it->second);
   }
   return 0;
}



ConfigSection& ConfigFile::operator[] (std::string section) {
   return sectionmap[section];
}



const ConfigSection& ConfigFile::operator[] (std::string section) const {
   SECTIONMAP::const_iterator it = sectionmap.find(section);
   if (it == sectionmap.end()) {
      throw EagleException(StringPrintF("ConfigFile::operator[](std::string) const - Section '%s' doesn't exist in sectionmap!" , section.c_str()));
   }
   return it->second;
}




