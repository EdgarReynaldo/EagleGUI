
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
 * @file Font.hpp
 * @brief The interface for working with fonts in Eagle
 */

#ifndef EagleFont_HPP
#define EagleFont_HPP



#include <string>



#include "Eagle/Image.hpp"
#include "Eagle/Object.hpp"
#include "Eagle/SharedMemory.hpp"



/**! @enum FONT_LOADING_FLAGS
 *   @brief A set of flags indicating how to load the font
 */

enum FONT_LOADING_FLAGS {
   LOAD_FONT_NORMAL         = 0,
   LOAD_FONT_NO_KERNING     = 1 << 0,///< Whether to monospace this font
   LOAD_FONT_MONOCHROME     = 1 << 1,///< Whether to alias this font or keep it pixelated
   LOAD_FONT_NOAUTOHINT     = 1 << 2,///< TODO : I Have no idea what this is
   LOAD_FONT_NOPREMULTALPHA = 1 << 3 ///< Whether to pre-multiply the alpha for this font
};


/**! @class EagleFont
 *   @brief An abstract base class for working with fonts in Eagle
 *
 *   See @ref Allegro5Font for a concrete implementation of EagleFont
 */




class EagleFont : public EagleObject {
   
protected :
   int height;
   std::string srcfile;

public :   
///   EagleFont();
///   EagleFont(std::string name);
   EagleFont(std::string objclass , std::string objname);///< Base constructor for font objects
   virtual ~EagleFont() {}

   virtual bool Load(std::string file , int size , int flags , IMAGE_TYPE type)=0;///< Pure virtual function to load a font
   virtual void Free()=0;///< Pure virtual function to free a font
   
   virtual bool Valid()=0;///< Pure virtual function to determine if a font is valid and ready
   
   virtual int Width(std::string str)=0;///< Pure virtual function to get the width of a specified string using this font
   virtual int Height()=0;///< Pure virtual function to get the line height of this font
   virtual int Height(std::string str , int ls)=0;///< Pure virtual function to get the height of a specified text string (may be multi-line)
   virtual int VHeight(std::string str , int letter_spacing)=0;///< Pure virtual function to get the height of a vertical string
   virtual int VWidth(std::string str , int line_spacing)=0;///< Pure virtual function to get the width of a vertical string (may be multi-line)
   
   
   virtual std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const ;///< Describe this font to a stream
};



/**! @fn GetFont <std::string>
 *   @brief Get the font associated with this name. Works through the EagleObjectRegistry.
 */

EagleFont* GetFont(std::string font_name);

/**! @fn SetDefaultFontPath @fn SetDefaultFontSize @fn SetDefaultFontFlags
 *   @brief Set the default font path, size, or flags
 *
 *   The default font path, size, and flags will be used whenever a new display is created.
 *   You can retrieve the default font from a display @ref EagleGraphicsContext with the @ref DefaultFont function
 */

void SetDefaultFontPath(std::string path);///< Set the default font path
void SetDefaultFontSize(int size);///< Set the default font size
void SetDefaultFontFlags(int flags);///< Set the default font flags

std::string DefaultFontPath();///< Get the default font path
int DefaultFontSize();///< Get the default font size
int DefaultFontFlags();///< Get the default font flags


/**! @typedef SHAREDFONT
 *   @brief Simple typedef for working with shared pointers to EagleFont objects
 */

typedef SHAREDOBJECT<EagleFont> SHAREDFONT;



#endif // EagleFont_HPP



