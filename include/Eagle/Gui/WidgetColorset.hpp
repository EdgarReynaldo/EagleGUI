
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
 *    Copyright 2009-2019+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file WidgetColorset.hpp
 * @brief This file contains the interface for working with widget color sets and registering colors and colorsets
 * 
 */

#ifndef EagleGuiWidgetColorset_HPP
#define EagleGuiWidgetColorset_HPP



#include "Eagle/Color.hpp"
#include "Eagle/Object.hpp"
#include "Eagle/Exception.hpp"
#include "Eagle/SharedMemory.hpp"



#include <map>



/*! @enum WIDGETCOLOR 
 *  @brief Enumerates the standard color scheme for a widget and its corresponding place in a @ref WidgetColorset
 *
 * WIDGETCOLOR ranges from darkest (SDCOL shadow color) to lightest (HLCOL highlight color), with a few extras for
 * hover, and the CSS box model's margin, border, and padding areas.
 */

enum WIDGETCOLOR {
   SDCOL   = 0, ///< Shadow color
   BGCOL   = 1, ///< Background color
   MGCOL   = 2, ///< Middle ground color
   FGCOL   = 3, ///< Foreground color
   HLCOL   = 4, ///< Highlight color
   TXTCOL  = 5, ///< Text color
   FCSCOL  = 6, ///< Focus color
   HVRCOL  = 7, ///< Hover color
   PADCOL  = 8, ///< Padding color
   BORDCOL = 9, ///< Border color
   MARGCOL = 10,///< Margin color
   LASTCOL = 10 ///< For setting the number of colors
};


#define EAGLE_NUMCOLORS (LASTCOL + 1)

std::string WidgetColorName(WIDGETCOLOR wc);///< Gets the name for this @ref WIDGETCOLOR
WIDGETCOLOR WidgetColorFromName(std::string name);///< Gets the @ref WIDGETCOLOR for this name



/**! @class WidgetColorset
 *   @brief The WidgetColorset is simply an array of EagleColor objects, and provides easy access to their value
 */
 
class WidgetColorset {

protected :
   EagleColor wcolorset[EAGLE_NUMCOLORS];///< The stored color array

public :

   ///< Default constructor, uses the default_eagle_color_array for values
   WidgetColorset();

   ///< Initialize this color set with an array of EagleColor objects
   WidgetColorset(EagleColor colorset[EAGLE_NUMCOLORS]);

   ///< Assigns our colorset's values to those specified in colorset
   WidgetColorset& operator=(const EagleColor colorset[EAGLE_NUMCOLORS]);
   

   ///< Provides array style access to the EagleColor array, bounds are not checked
   EagleColor& operator[] (const WIDGETCOLOR& wc) {return wcolorset[wc];}

   ///< Returns a const reference to the specified EagleColor, for constant WidgetColorset objects
   const EagleColor& operator[] (const WIDGETCOLOR& wc) const {return wcolorset[wc];}

   EagleColor& operator[] (int wc) {return wcolorset[wc];}
   const EagleColor& operator[] (int wc) const {return wcolorset[wc];}
   
   std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const ;
   
   friend std::ostream& operator<<(std::ostream& os , const EagleColor& ecol);
};

extern EagleColor default_eagle_color_array[EAGLE_NUMCOLORS];///< A global color array used to initialize default WidgetColorset objects





/*! @class RegisteredColor
 *  @brief The RegisteredColor class provides a simple way to register and map colors to names 
 *  using the @ref ColorRegistry::GlobalColorRegistry() object.
 *
 *  Any RegisteredColor objects you create will register themselves with the global color registry.
 */

class RegisteredColor {

public :
   RegisteredColor(std::string name , float red , float green , float blue, float alpha);
   RegisteredColor(std::string name , int red , int green , int blue, int alpha);

};



/*! @class ColorRegistry
 *  @brief The ColorRegistry class stores colors for use by the gui system and other users
 * 
 *  Pre-defined colors are clear, white, black, light_gray, medium_gray, dark_gray,
 *  red, orange, yellow, lime_green , green, neon_green, cyan, sky_blue, blue, purple, magenta, and fuchsia
 *  Rather 5 shades of grey, and 12 fully saturated hues
 *
 *  A ColorRegistry object can store both named colors and named colorsets.
 */

class ColorRegistry {

protected :
   std::map<std::string , EagleColor> named_colors;///< Map of the named colors in the registry
   std::map<std::string , SHAREDOBJECT<WidgetColorset> > named_colorsets;///< Map of the named colorsets in the registry
   
public :
   
   ColorRegistry() : named_colors() , named_colorsets() {}///< Default empty constructor
   
   static ColorRegistry* GlobalColorRegistry();///< Registry for global colors and colorsets
   
   /// EagleColors
   bool HasColor(std::string name);///< Returns true if the named color is present in the registry

   EagleColor& GetColorByName(std::string name);///< Returns the color associated with name. Throws if not found.

   void RegisterColor(std::string name , const EagleColor& color);///< Registers or overwrites a color in the registry
   
   /// WidgetColorsets
   bool HasColorset(std::string name);///< Returns true if the named colorset is present in the registry
   
   SHAREDOBJECT<WidgetColorset> GetColorsetByName(std::string name);///< Get a shared handle to a named colorset
   SHAREDOBJECT<WidgetColorset> GetDefaultColorset() {return GetColorsetByName("Default");}///< Returns the default colorset
   
   void RegisterColorset(std::string name , const WidgetColorset& wc);///< Registers or overwrites the named colorset with wc
   
};



/// Functions for working with the global color registry

bool HasColor(std::string name);///< Whether the global registry has the named color
EagleColor GetColorByName(std::string name);///< Returns the named color. Throws if not found.
void RegisterColor(std::string name , const EagleColor& color);///< Registers the named color with the global registry

bool HasColorset(std::string name);///< Whether or not the global registry has the named colorset
SHAREDOBJECT<WidgetColorset> GetColorsetByName(std::string name);///< Get a shared handle to the named colorset.
void RegisterColorset(std::string name , const WidgetColorset& wc);///< Registers the named colorset with the global registry



#endif // EagleGuiWidgetColorset_HPP



