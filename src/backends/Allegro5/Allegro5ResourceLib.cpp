



#include "Eagle/Exception.hpp"



#include "Eagle/backends/Allegro5/Allegro5ResourceLib.hpp"




Allegro5ResourceLibrary* Allegro5ResourceLibrary::me = 0;



Allegro5ResourceLibrary::Allegro5ResourceLibrary() :
      ResourceLibrary()
{
   if (me) {
      throw EagleException("Allegro5ResourceLibrary reports multiple creation");
   }
   me = this;
}



std::set<std::string> Allegro5ResourceLibrary::GetSupportedTypes(RESOURCE_TYPE rt) {
   static std::vector<std::string> stypes[NUM_RT_TYPES];
   static int init = 1;
   if (init) {
      init = 0;
      stypes[RT_IMAGE].push_back("bmp");
      stypes[RT_IMAGE].push_back("png");
      stypes[RT_IMAGE].push_back("jpg");
      stypes[RT_IMAGE].push_back("jpeg");
      stypes[RT_IMAGE].push_back("tga");

      stypes[RT_FONT].push_back("ttf");
      stypes[RT_FONT].push_back("bmp");

      stypes[RT_AUDIO].push_back("ogg");
      stypes[RT_AUDIO].push_back("wav");

      stypes[RT_VIDEO].push_back("ogv");

      stypes[RT_ARCHIVE].push_back("zip");
      stypes[RT_ARCHIVE].push_back("7z");

      stypes[RT_BINFILE].push_back("dat");
      stypes[RT_BINFILE].push_back("bin");

      stypes[RT_TEXTFILE].push_back("txt");
      stypes[RT_TEXTFILE].push_back("cfg");
      stypes[RT_TEXTFILE].push_back("ini");

      stypes[RT_UNKNOWN].insert(stypes[RT_UNKNOWN].end() , stypes[RT_IMAGE   ].begin() , stypes[RT_IMAGE   ].end());
      stypes[RT_UNKNOWN].insert(stypes[RT_UNKNOWN].end() , stypes[RT_FONT    ].begin() , stypes[RT_FONT    ].end());
      stypes[RT_UNKNOWN].insert(stypes[RT_UNKNOWN].end() , stypes[RT_AUDIO   ].begin() , stypes[RT_AUDIO   ].end());
      stypes[RT_UNKNOWN].insert(stypes[RT_UNKNOWN].end() , stypes[RT_VIDEO   ].begin() , stypes[RT_VIDEO   ].end());
      stypes[RT_UNKNOWN].insert(stypes[RT_UNKNOWN].end() , stypes[RT_ARCHIVE ].begin() , stypes[RT_ARCHIVE ].end());
      stypes[RT_UNKNOWN].insert(stypes[RT_UNKNOWN].end() , stypes[RT_BINFILE ].begin() , stypes[RT_BINFILE ].end());
      stypes[RT_UNKNOWN].insert(stypes[RT_UNKNOWN].end() , stypes[RT_TEXTFILE].begin() , stypes[RT_TEXTFILE].end());
   }
   
   

   std::set<std::string> types;
   types.insert(stypes[rt].begin() , stypes[rt].end());
   return types;
}
