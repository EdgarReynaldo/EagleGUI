

#include "Eagle/Platform.hpp"
#include "Eagle/Gui/Text.hpp"

#include "Eagle/GraphicsContext.hpp"
#include "Eagle/System.hpp"
#include "Eagle/Gui/WidgetHandler.hpp"


#include <cstdlib>



const int TOPIC_TEXT_WIDGET = NextFreeTopicId();



const int TEXT_COPIED = 0;
const int LINK_LAUNCHED = 1;



REGISTERED_WIDGET_MESSAGE(TOPIC_TEXT_WIDGET , TEXT_COPIED);
REGISTERED_WIDGET_MESSAGE(TOPIC_TEXT_WIDGET , LINK_LAUNCHED);
