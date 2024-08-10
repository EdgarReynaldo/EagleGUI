



#include "Eagle/backends/Allegro5/Allegro5ObjModel.hpp"

#include "Eagle/Lib.hpp"
#include "Eagle/System.hpp"
#include "Eagle/ResourceLib.hpp"
#include "Eagle/Resources.hpp"
#include "Eagle/ResourceLib.hpp"
#include "Eagle/FileSystem.hpp"

#include "Eagle/backends/Allegro5/Allegro5Resources.hpp"



std::vector<std::string> Allegro5MaterialFile::GetLines() {
   return TextResource::FileLines();
}



std::string Allegro5MaterialFile::GetFileName() {
   return ResourceBase::GetFilePath().Path();
}



bool Allegro5MaterialFile::LoadFile(std::string file) {

   bool ld = LoadFromFile(FilePath(file));

   return ld && ProcessFile();
}



/**
bool MaterialFile::SaveFile(std::string filepath , const char* newline) {
   (void)filepath;
   (void)newline;
   return false;
}
//*/







std::vector<std::string> Allegro5ObjectFile::GetLines() {
   return TextResource::FileLines();
}



std::string Allegro5ObjectFile::GetFileName() {
   return ResourceBase::GetFilePath().Path();
}



bool Allegro5ObjectFile::LoadFile(std::string file) {

   LoadFromFile(FilePath(file));
   
   return ProcessObjectFile();
}



//bool ObjectFile::SaveFile(std::string filepath , const char* newline) {
//   (void)filepath;
//   (void)newline;
//   return false;
//}




