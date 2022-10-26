
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




#include "Eagle/ConfigFile.hpp"

#include "Eagle/Lib.hpp"
#include "Eagle/System.hpp"
#include "Eagle/FileSystem.hpp"
#include "Eagle/Exception.hpp"
#include "Eagle/StringWork.hpp"
#include "Eagle/GraphicsContext.hpp"

#include <sstream>
#include <fstream>
#include <cstring>


bool STOB(std::string b) {
   bool ret = false;
   if (strcmp(b.c_str() , "On") == 0) {ret = true;}
   if (strcmp(b.c_str() , "True") == 0) {ret = true;}
   return ret;
}



int STOI(std::string i) {
   int n = 0;
   sscanf(i.c_str() , "%d" , &n);
   return n;
}



float STOF(std::string f) {
   float n = 0;
   sscanf(f.c_str() , "%f" , &n);
   return n;
}



double STOD(std::string d) {
   double n = 0;
   sscanf(d.c_str() , "%lf" , &n);
   return n;
}



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
      size_t j = line.find_first_of('=');
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



ConfigSection::~ConfigSection() {
   ClearConfigSection();
}



void ConfigSection::ClearConfigSection() {
   for (unsigned int i = 0 ; i < clines.size() ; ++i) {
      delete clines[i];
   }
   clines.clear();
}



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



ConfigFile::~ConfigFile() {
   ClearContents();
}



void ConfigFile::ClearContents() {
   sectionmap.clear();
   contents = "";
}



bool ConfigFile::LoadFromFile(const char* path) {

   ClearContents();
   
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
   
   std::vector<std::string> lines = SplitByNewLinesChomp(contents);
   
   ConfigSection* section = &sectionmap["GLOBAL"];
   for (int i = 0 ; i < (int)lines.size() ; ++i) {
      std::string l = lines[i];
      
      size_t idx1 = l.find_first_of('[');
      size_t idx2 = l.find_first_of(']');
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




/// --------------------------      GraphicsConfig      ----------------------




GraphicsConfig::GraphicsConfig() :
      ConfigFile(),
      file(),
      setup(false),
      gl(true),
      flags(0),
      fs(false),
      fullscreen(0),
      fsw(1920),
      fsh(1280),
      sw(800),
      sh(600),
      gw(800),
      gh(600),
      sys(0),
      win(0)
{}



GraphicsConfig::GraphicsConfig(std::string path) :
      ConfigFile(),
      file(path),
      setup(false),
      gl(true),
      flags(0),
      fs(false),
      fullscreen(0),
      fsw(1920),
      fsh(1280),
      sw(800),
      sh(600),
      gw(800),
      gh(600),
      sys(0),
      win(0)
{
   setup = Setup(path);
}



bool GraphicsConfig::Load(std::string path) {
   file = path;
   return LoadFromFile(path.c_str());
}



bool GraphicsConfig::Save(std::string path) {
   file = path;
   return SaveToFile(path.c_str());
}



void GraphicsConfig::Create() {
   ConfigSection& s = (*this)["Graphics"];
   s.AddConfigLine("Driver" , "OpenGL");
   s.AddConfigLine("Fullscreen" , "False");
   s.AddConfigLine("FullscreenWidth" , "1920");
   s.AddConfigLine("FullscreenHeight" , "1080");
   s.AddConfigLine("ScreenWidth" , "800");
   s.AddConfigLine("ScreenHeight" , "600");
}



bool GraphicsConfig::Setup(std::string filepath) {
   if (!Load(filepath)) {
      EagleInfo() << "Failed to load config file. Attempting creation." << std::endl;
      Create();
      if (!Save(filepath)) {
         EagleWarn() << "Failed to create config file! Proceeding." << std::endl;
      }
   }
   return setup = Parse();
}



bool GraphicsConfig::Parse() {
   bool error = false;
   fsw = STOI((*this)["Graphics"]["FullscreenWidth"]);
   fsh = STOI((*this)["Graphics"]["FullscreenHeight"]);
   sw = STOI((*this)["Graphics"]["ScreenWidth"]);
   sh = STOI((*this)["Graphics"]["ScreenHeight"]);

   if (fsw == 0 || fsh == 0) {error = true;}
   if (sw == 0 || sh == 0) {error = true;}

   gl = (*this)["Graphics"]["Driver"].compare("OpenGL") == 0;
   flags = gl?EAGLE_OPENGL:0;
   fs = STOB((*this)["Graphics"]["Fullscreen"]);
   fullscreen = fs?EAGLE_FULLSCREEN_WINDOW:EAGLE_WINDOWED;
   flags |= fullscreen;
   
   
   return !error;
}



EagleGraphicsContext* GraphicsConfig::SetupWindow(EagleSystem* system) {
   sys = system;
   win = sys->CreateGraphicsContext("Graphics Window" , sw , sh , flags);
   if (fs) {
      fsw = win->Width();
      fsh = win->Height();
   }
   gw = fs?fsw:sw;
   gh = fs?fsh:sh;

   return win;
}








