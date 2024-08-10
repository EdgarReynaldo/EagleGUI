
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
 * @file Texture.cpp
 * @brief Implementation for TexID and TEXTEX
 *
 */




#include "Eagle/Texture.hpp"
#include "Eagle/GraphicsContext.hpp"
#include "Eagle/Image.hpp"



const TexID BAD_TEXID = TexID();



TexID::TexID() :
      pimg(0)
{}



TexID::TexID(EagleImage* img) :
      pimg(img)
{}



TexID::TexID(const TexID& tid) :
      pimg(tid.pimg)
{}



TexID& TexID::operator=(const TexID& tid) {
   pimg = tid.pimg;
   return *this;
}



bool TexID::operator!=(const TexID& texid) {
   return (this->pimg != texid.pimg);
}


TexLib::TexLib() :
      texlib() 
{}



TexLib::~TexLib() {
   Clear();
}

void TexLib::Clear() {
   for (std::map<std::string , TexID>::iterator it = texlib.begin() ; it != texlib.end() ; ++it) {
      TexID& tid = it->second;
      if (tid.pimg) {
         EagleImage* p = tid.pimg;
         p->ParentContext()->FreeImage(p);
         tid.pimg = 0;
      }
   }
   texlib.clear();
}



TexID TexLib::LoadTexture(EagleGraphicsContext* win , std::string image_file) {
   EagleImage* img = win->LoadImageFromFile(image_file.c_str());
   if (img && img->Valid()) {
      texlib[image_file] = TexID(img);
      return texlib[image_file];
   }
   return BAD_TEXID;
}



TexID TexLib::TID(std::string texname) {
   if (texlib.find(texname) == texlib.end()) {
      return BAD_TEXID;
   }
   return texlib[texname];
}



