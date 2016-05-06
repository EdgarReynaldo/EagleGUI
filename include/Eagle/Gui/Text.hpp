



#ifndef Text_HPP
#define Text_HPP

#include "Eagle/Gui/Alignment.hpp"
#include "Eagle/Gui/WidgetBase.hpp"
#include "Eagle/StringWork.hpp"

#include <vector>
#include <string>

void GetTextAttributes(std::string text , EagleFont* font , int line_spacing , int* pnlines ,
                       std::vector<std::string>* plines , std::vector<int>* pwidths_vector ,
                       int* pmaxwidth , int* ptotalheight);


using std::string;


static const int TOPIC_TEXT_WIDGET = NextFreeTopicId();

static const int TEXT_COPIED = 1;
static const int LINK_LAUNCHED = 0;

REGISTER_WIDGET_MESSAGE(TOPIC_TEXT_WIDGET , TEXT_COPIED);
REGISTER_WIDGET_MESSAGE(TOPIC_TEXT_WIDGET , LINK_LAUNCHED);






#endif // Text_HPP
