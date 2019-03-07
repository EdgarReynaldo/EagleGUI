
/**
 *
 *     _______       ___       ____      __       _______
 *    /\  ____\    /|   \     /  __\    /\ \     /\  ____\
 *    \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_
 *     \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\
 *      \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_
 *       \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *        \/______/|/_/  \/_/  \_\_____/    \/_____/ \/______/
 *
 *
 *    EAGLE
 *    Edgar's Agile Gui Library and Extensions
 *
 *    Copyright 2009-2019+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */



#ifndef Parsing_HPP
#define Parsing_HPP


#include "Eagle/Exception.hpp"
#include "Eagle/Color.hpp"
#include "Eagle/Gui/WidgetColorset.hpp"
#include "Eagle/Gui/WidgetAttributes.hpp"

#include <map>
#include <string>




AttributeValueMap ParseAttributeSet(std::string attribute_value_set);

/// Valid attributes for an EagleColor are RGB, RGBA, FRGB, and FRGBA. 
/// Value takes form of parenthesis enclosed comma separated list of values.
/**
EagleColor lavender = RGB(127,64,255)
*/

/// Declaration follows format [RGB | RGBA | FRGB | FRGBA](%n,%n,%n[,%n]) eg. RGB(255,255,255) or FRGBA(1.0,1.0,1.0,1.0)
EagleColor ParseColor(const std::string color_dec);



/// Valid format for a WidgetColorset declaration
/**
WidgetColorset mycolors = 
SDCOL : RGB(0,0,0)
BGCOL : dark-gray
*/

WidgetColorset ParseWidgetColorset(const AttributeValueMap& avmap);


#endif // Parsing_HPP





