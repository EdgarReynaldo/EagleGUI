


#include "Eagle/Gui/Alignment.hpp"
#include "Eagle/StringWork.hpp"
#include "Eagle/Error.hpp"



#include <sstream>
using std::stringstream;



std::string PrintAlignment(HALIGNMENT halign , VALIGNMENT valign) {
   stringstream ss;
   ss << "Layout alignment : ";
   ss << "Horizontal alignment [";
   switch (halign) {
      case HALIGN_LEFT : ss << "HALIGN_LEFT";break;
      case HALIGN_CENTER : ss << "HALIGN_CENTER";break;
      case HALIGN_RIGHT : ss << "HALIGN_RIGHT";break;
      default : throw EagleError(StringPrintF("Horizontal alignment value unknown (%d)\n",(int)halign));break;
   }
   ss << "] Vertical alignment [";
   switch (valign) {
      case VALIGN_TOP : ss << "VALIGN_TOP";break;
      case VALIGN_CENTER : ss << "VALIGN_CENTER";break;
      case VALIGN_BOTTOM : ss << "VALIGN_BOTTOM";break;
      default : throw EagleError(StringPrintF("Vertical alignment value unknown (%d)\n",(int)valign));break;
   }
   ss << "]";
   return ss.str();
}


