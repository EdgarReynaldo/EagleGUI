
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
 * @file Parsing.hpp
 * @brief A collection of parsing functions used to read widget attributes and colors, etc...
 *
 */

#ifndef Parsing_HPP
#define Parsing_HPP


#include <map>
#include <string>

#include "Eagle/Exception.hpp"
#include "Eagle/Color.hpp"
#include "Eagle/Gui/WidgetColorset.hpp"
#include "Eagle/Gui/WidgetAttributes.hpp"



/**! @brief Parses the string and creates an attribute value map
 *   @sa AttributeValueMap
 */

AttributeValueMap ParseAttributeSet(std::string attribute_value_set);



/**! @brief Parses a color from text
 *  
 *   Valid attributes for an EagleColor are RGB, RGBA, FRGB, and FRGBA. 
 *   Value takes form of parenthesis enclosed comma separated list of values.
 *   Ex.
 *   EagleColor lavender = RGB(127,64,255)
 *   Declaration follows format [RGB | RGBA | FRGB | FRGBA](%n,%n,%n[,%n]) eg. RGB(255,255,255) or FRGBA(1.0,1.0,1.0,1.0)
 *   Valid values for RGB and RGBA are integers from [0,255]
 *   Valid values for FRGB and FRGBA are floats or doubles from [0.0,1.0]
 */

EagleColor ParseColor(const std::string color_dec);



/**! @brief Parses a widget colorset from an attribute value map
 *   
 *   Valid format for a WidgetColorset declaration
 *   WidgetColorset mycolors = 
 *   SDCOL : RGB(0,0,0)
 *   BGCOL : dark-gray
 *   
 *   @sa WIDGETCOLOR
 *   @sa ParseColor(const std::string)
 */

WidgetColorset ParseWidgetColorset(const AttributeValueMap& avmap);




#endif // Parsing_HPP



