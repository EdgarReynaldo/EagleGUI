
/**
 *
 *         _______       ___       ____      __       _______
 *        /\  ____\    /|   \     /  __\    /\ \     /\  ____\
 *        \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_
 *         \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\
 *          \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_
 *           \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *            \/______/|/_/  \/_/  \_\____/     \/_____/ \/______/
 *
 *
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2024+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file Texture.hpp
 * @brief TexID, TextureVertex, and TEXTEX classes
 *
 */




#ifndef Texture_HPP
#define Texture_HPP


#include "Eagle/Vec2.hpp"
#include <map>
#include <string>



class EagleImage;

class TexID {
   
public :
   EagleImage* pimg;
   
   TexID();
   TexID(EagleImage* img);
   TexID(const TexID& tid);
   TexID& operator=(const TexID& tid);
   
   bool operator!=(const TexID& texid);
};


extern const TexID BAD_TEXID;




class EagleGraphicsContext;

class TexLib {
protected :
   std::map<std::string , TexID> texlib;

public :
   
   TexLib();
   ~TexLib();
   
   void Clear();
   
   
   TexID LoadTexture(EagleGraphicsContext* win , std::string image_file);

   TexID TID(std::string texname);


};



class TextureVertex {
public :

///   TextureVertex(ALLEGRO_BITMAP* tex , Vec2 tuv);
   TextureVertex();
   TextureVertex(TexID texid , Vec2 tuv);
   TexID tid;
   Vec2 uv;///double u,v;
};

typedef TextureVertex TEXTEX;




#endif // Texture_HPP
