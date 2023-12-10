
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
 *    Copyright 2009-2023+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file Allegro5FontManager.hpp
 * @brief Our backend for Allegro 5 font handling
 *
 */


#ifndef ALLEGRO5_FONT_MANAGER_HPP
#define ALLEGRO5_FONT_MANAGER_HPP


#include "Eagle/FontManager.hpp"


class Allegro5FontManager : public FontManager {

public :
   explicit Allegro5FontManager(EagleGraphicsContext* window);
   ~Allegro5FontManager();
   
   virtual EagleFont* CreateBuiltinFont();
   virtual EagleFont* CreateDefaultFont();

   virtual EagleFont* LoadFontPath(std::string path , int size , int fontflags , IMAGE_TYPE type = VIDEO_IMAGE);
};



#endif // ALLEGRO5_FONT_MANAGER_HPP


