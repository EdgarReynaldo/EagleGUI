
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
   FONT_NORMAL         = 0,
   FONT_NO_KERNING     = 1 << 0,///< Whether to monospace this font
   FONT_MONOCHROME     = 1 << 1,///< Whether to alias this font or keep it pixelated
   FONT_NOAUTOHINT     = 1 << 2,///< TODO : I Have no idea what this is
   FONT_NOPREMULTALPHA = 1 << 3 ///< Whether to pre-multiply the alpha for this font
};

std::string TranslateFontFlags(FONT_LOADING_FLAGS flags);


/**! @enum FONT_STYLE_FLAGS {
 *   @brief Font style flags
 */
 enum FONT_STYLE_FLAGS {
   FONT_ROMAN = 0,
   FONT_ITALIC = 0x00000001,
   FONT_BOLD = 0x00000010,
   FONT_BOLD_ITALIC = 0x00000011
};

/**! @enum FONT_SOURCE_FLAGS 
 *   @brief Flags to track source of font
 */
enum FONT_SOURCE_FLAGS {
   FONT_UNKNOWN = 0,
   FONT_BUILTIN = 1,
   FONT_SRCFILE = 2,
   FONT_MEMFILE = 4
}


/**! @class EagleFont
 *   @brief An abstract base class for working with fonts in Eagle
 *
 *   See @ref Allegro5Font for a concrete implementation of EagleFont
 */



class EagleGraphicsContext;

class EagleFont : public EagleObject {
   
protected :
   EagleGraphicsContext* owner;
   FontManager* fontman;
   int height;
   std::string srcfile;
   int fontflags;
   int styleflags;
   int srcflags;
   int memtype;
   
   friend class FontManager;
   
public :   
   EagleFont(std::string objclass = "EagleFont" , std::string objname = "Nemo");///< Base constructor for font objects

   virtual ~EagleFont() {}

   virtual bool LoadFromMemory(MemFile* memfile , int pt , int flags , std::string src , IMAGE_TYPE type = VIDEO_IMAGE)=0;///< Only loads ttf fonts from a memfile
   virtual bool Load(std::string file , int size , int flags , IMAGE_TYPE type)=0;///< Pure virtual function to load any supported font
   
   virtual void Free()=0;///< Pure virtual function to free a font
   
   virtual bool Valid()=0;///< Pure virtual function to determine if a font is valid and ready
   
   virtual int Width(std::string str)=0;///< Pure virtual function to get the width of a specified string using this font
   virtual int Height()=0;///< Pure virtual function to get the line height of this font
   virtual int Height(std::string str , int ls)=0;///< Pure virtual function to get the height of a specified text string (may be multi-line)
   virtual int VHeight(std::string str , int letter_spacing)=0;///< Pure virtual function to get the height of a vertical string
   virtual int VWidth(std::string str , int line_spacing)=0;///< Pure virtual function to get the width of a vertical string (may be multi-line)
   
   EagleGraphicsContext* Owner() {return owner;}
   int FontPoint() {return height;}
   std::string File() {return srcfile;}
   int Flags() {return fontflags;}
   int Style() {return styleflags;}
   int SrcFlags() {returnn srcflags;}
   int ImageType() {return memtype;}
   
   virtual std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const ;///< Describe this font to a stream
};



/**! @fn GetFont <std::string>
 *   @brief Get the font associated with this name. Works through the EagleObjectRegistry.
 */

EagleFont* GetFont(std::string font_name);


/**! @typedef SHAREDFONT
 *   @brief Simple typedef for working with shared pointers to EagleFont objects
 */

typedef SHAREDOBJECT<EagleFont> SHAREDFONT;






#endif // EagleFont_HPP















