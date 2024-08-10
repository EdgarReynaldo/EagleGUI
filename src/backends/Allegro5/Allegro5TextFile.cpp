





#include "Eagle/backends/Allegro5/Allegro5TextFile.hpp"
#include "Eagle/StringWork.hpp"
#include "allegro5/allegro.h"


Allegro5TextFile::Allegro5TextFile() :
      TextFile()
{}



bool Allegro5TextFile::LoadFile(std::string txtfilepath) {
   path = txtfilepath;
   ALLEGRO_FILE* file = al_fopen(txtfilepath.c_str() , "rb");
   if (!file) {return false;}
   contents.clear();
   bool complete = false;
   while (!complete) {
      int c = al_fgetc(file);
      if (c == EOF) {complete = true;}
      else {contents.push_back(c);}
   }
   al_fclose(file);
   lines = SplitByNewLinesChomp(contents);
   return (bool)contents.size();
}



bool Allegro5TextFile::SaveFile(std::string filepath , const char* newline) {
   path = filepath;
   ALLEGRO_FILE* file = al_fopen(filepath.c_str() , "wb");
   if (!file) {return false;}
   for (unsigned int i = 0 ; i < lines.size() ; ++i) {
      if (0 > al_fprintf(file , "%s%s" , lines[i].c_str() , newline)) {
         al_fclose(file);
         return false;
      }
   }
   al_fclose(file);
   return true;
}

