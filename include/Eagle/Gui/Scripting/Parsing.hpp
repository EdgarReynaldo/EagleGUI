



#ifndef Parsing_HPP
#define Parsing_HPP


#include "Eagle/Exception.hpp"
#include "Eagle/Color.hpp"
#include "Eagle/Gui/WidgetColorset.hpp"


#include <map>
#include <string>




typedef std::map<std::string , std::string> ATTRIBUTE_VALUE_MAP;
typedef ATTRIBUTE_VALUE_MAP::iterator AVMIT;



ATTRIBUTE_VALUE_MAP ParseAttributeSet(std::string attribute_value_set);/// throw (EagleException);

/// Valid attributes for an EagleColor are RGB, RGBA, FRGB, and FRGBA. 
/// Value takes form of parenthesis enclosed comma separated list of values.
/**
EagleColor lavender = RGB(127,64,255)
*/

/// Declaration follows format [RGB | RGBA | FRGB | FRGBA](%n,%n,%n[,%n]) eg. RGB(255,255,255) or FRGB(1.0,1.0,1.0)
EagleColor ParseColor(std::string color_dec);/// throw (EagleException);

/// Valid format for a WidgetColorset declaration
/**
WidgetColorset mycolors = 
SDCOL : RGB(0,0,0)
BGCOL : dark-gray
*/

WidgetColorset ParseWidgetColorset(const ATTRIBUTE_VALUE_MAP& avmap);/// throw (EagleException);


#endif // Parsing_HPP





