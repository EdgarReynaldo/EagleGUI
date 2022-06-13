
/**
 *
 *         _______       ___       ____      __       _______
 *        /\  ____\    /|   \     /  __\    /\ \     /\  ____\
 *        \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_
 *         \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\
 *          \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_
 *           \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *            \/______/|/_/  \/_/  \_\_____/    \/_____/ \/______/
 *
 *
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2021+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file Allegro5Font.hpp
 * @brief The interface for working with fonts in Eagle using Allegro 5
 */

#ifndef Allegro5Font_HPP
#define Allegro5Font_HPP



#include "Eagle/Font.hpp"

#include "allegro5/allegro_font.h"
#include "allegro5/allegro_ttf.h"


/**! @class Allegro5Font
 *   @brief The Allegro 5 implementation of an EagleFont
 */

class Allegro5Font : public EagleFont {

private :
   ALLEGRO_FONT* allegro_font;
   ALLEGRO_FILE* allegro_file;
   
   
public :
   ///< Default constructor
   Allegro5Font(std::string objclass = "Allegro5Font" , std::string objname = "Nemo");
   
   ///< Constructor that takes ownership of an ALLEGRO_FONT*
   Allegro5Font(ALLEGRO_FONT* font , std::string objname = "Nemo");

   ///< Constructor that loads a font from a file using the specified size and flags
   Allegro5Font(std::string file , int size , int flags , std::string objname = "Nemo" , IMAGE_TYPE type = VIDEO_IMAGE);
   
   ///< Load a font file from a memory file, only ttf fonts are supported
   bool LoadFromMemory(MemFile* memfile , int size , int flags , IMAGE_TYPE type = VIDEO_IMAGE);

   ///< Load any supported font using the specified size, flags and image type
   bool Load(std::string file , int size , int flags , IMAGE_TYPE type = VIDEO_IMAGE);

   bool Adopt(ALLEGRO_FONT* f);
   
   ///< Free the underlying font
   void Free();
   
   ///< Returns true if valid and ready to use
   bool Valid();

   int Width(std::string str);///< Get the width of a string
   int Height();///< Get the height of a line of text
   int Height(std::string str , int ls);///< Get the height of multi-line text using the specified line spacing
   int VHeight(std::string str , int letter_spacing);///< Get the height of a vertical string
   int VWidth(std::string str , int line_spacing);///< Get the width of a vertical string (may be multi-line)
   
   ALLEGRO_FONT* AllegroFont() {return allegro_font;}///< Get a handle to the underlying font
};




class Allegro5FontFamily : public EagleFontFamily {

protected :
   
   EagleFont* Load(std::string basename , std::string basepath , int point , FONT_STYLE_FLAGS style = FONT_ROMAN);
   
EagleFont* Allegro5FontFamily::Load(std::string basename , std::string basepath , int point , FONT_STYLE_FLAGS style) {
   
}
   
   
public :
   
   virtual ~Allegro5FontFamily();
Allegro5FontFamily::~Allegro5FontFamily() {
   Free();
}
   
};







#endif // Allegro5Font_HPP




















