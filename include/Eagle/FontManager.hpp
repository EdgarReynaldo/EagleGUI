



#ifndef FONT_MANAGER_HPP
#define FONT_MANAGER_HPP



#include "Eagle/Font.hpp"
#include "Eagle/MemFile.hpp"


#include <map>
#include <string>




/**! @fn SetDefaultFontPath @fn SetDefaultFontSize @fn SetDefaultFontFlags
 *   @brief Set the default font path, size, or flags
 *
 *   The default font path, size, and flags will be used whenever a new display is created.
 *   You can retrieve the default font from a display @ref EagleGraphicsContext with the @ref DefaultFont function
 */


class EagleGraphicsContext;



class FontManager {
   
protected :
   EagleGraphicsContext* owner;

   std::map<std::string , MemFile> memfiles;
   std::map<std::string , EagleFont*> fonts;
   std::map<EagleFont* , std::string> reversefontmap;

   typedef std::map<std::string , MemFile> MFMAP;
   typedef std::map<std::string , MemFile>::iterator MFMIT;
   typedef std::map<std::string , EagleFont*> FNTMAP;
   typedef std::map<std::string , EagleFont*>::iterator FNTIT;
   typedef std::map<EagleFont* , std::string> RFMAP;
   typedef std::map<EagleFont* , std::string>::iterator RFMIT;
   
   EagleFont* builtinfont;
   EagleFont* defaultfont;
   
   std::string default_font_path;// = "Data/Fonts/Verdana.ttf";
   int         default_font_size;// = -16;
   int         default_font_flags;// = LOAD_FONT_MONOCHROME;

   virtual EagleFont* CreateBuiltinFont()=0;
   virtual EagleFont* CreateDefaultFont()=0;
   
   
public :
   explicit FontManager(EagleGraphicsContext* window);
   virtual ~FontManager() {}
   
   virtual EagleFont* LoadFontPath(std::string path , int size , int loading_flags , IMAGE_TYPE type = VIDEO_IMAGE)=0;

   EagleFont* GetFont(std::string path , int size , int fontflags = FONT_NORMAL , int memflags = VIDEO_IMAGE);
   
   void FreeFont(EagleFont* font);
   void FreeFontFile(std::string path);
   void FreeAll();
   
   void SetDefaultFontPath(std::string path);///< Set the default font path
   void SetDefaultFontSize(int pointsize);///< Set the default font size
   void SetDefaultFontFlags(int flags);///< Set the default font flags

   EagleFont* BuiltinFont();
   EagleFont* DefaultFont();

   std::string DefaultFontPath();///< Get the default font path
   int DefaultFontSize();///< Get the default font point size
   int DefaultFontFlags();///< Get the default font flags
};





#endif // FONT_MANAGER_HPP



