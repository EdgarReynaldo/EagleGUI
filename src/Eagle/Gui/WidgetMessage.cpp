
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
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2019+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */



#include "Eagle/Gui/WidgetMessage.hpp"
#include "Eagle/Gui/WidgetBase.hpp"
#include "Eagle/StringWork.hpp"
#include "Eagle/Exception.hpp"



using std::ostream;
using std::string;
using std::map;
using std::pair;




WidgetMessageMapCleaner map_cleaner;/// This needs to be here to free the message map, don't remove

typedef map<pair<unsigned int,int>,pair<string,string> > MESSAGE_MAP; 
typedef pair<unsigned int,int> MESSAGE_KEY;
typedef pair<string,string> MESSAGE_VALUE;

MESSAGE_MAP* message_map = 0;



string GetMessageString(unsigned int topic , int message) {
   EAGLE_ASSERT(message_map);
   MESSAGE_MAP::iterator mit = message_map->find(MESSAGE_KEY(topic,message));
   if (mit == message_map->end()) {
      return "Message unknown";
   }
   string s;
   s += mit->second.first;
   s += " ";
   s += mit->second.second;
   return s;
}



RegisteredWidgetMessage::RegisteredWidgetMessage(unsigned int _topic , string _topic_str , int _message , string _message_str) :
      topic(_topic),
      topic_str(_topic_str),
      message(_message),
      message_str(_message_str)
{
   static int init = 1;
   if (init) {
      message_map = new MESSAGE_MAP();
      init = 0;
   }
   if (message_map->find(MESSAGE_KEY(topic,message)) != message_map->end()) {
      throw EagleException(StringPrintF("Message [%s] already registered!\n" , GetMessageString(topic,message).c_str()));
   }
   (*message_map)[MESSAGE_KEY(topic,message)] = MESSAGE_VALUE(topic_str , message_str);
}



WidgetMessageMapCleaner::~WidgetMessageMapCleaner() {
   static int destruct = 1;
   if (destruct) {
      delete message_map;
      message_map = 0;
      destruct = 0;
   }
}



#define NONE 0
unsigned int NextFreeTopicId() {
   static unsigned int id = NONE;
   id += 1;
   return id;
}



extern const unsigned int TOPIC_NONE = NextFreeTopicId();



const int MESSAGE_NONE = 0;




REGISTERED_WIDGET_MESSAGE(TOPIC_NONE , MESSAGE_NONE);



WidgetMsg::WidgetMsg() :
      from(0),
      topic(TOPIC_NONE),
      msgs(MESSAGE_NONE)
   {}



WidgetMsg::WidgetMsg(WidgetBase* widget_address , UINT widget_topic , int message) :
      from(widget_address),
      topic(widget_topic),
      msgs(message)
   {}



WidgetMsg::WidgetMsg(const WidgetMsg& wmsg) :
      from(wmsg.from),
      topic(wmsg.topic),
      msgs(wmsg.msgs)
   {}

   

std::ostream& operator<<(std::ostream& os , const WidgetMsg& wmsg) {
   os << "Message [" << GetMessageString(wmsg.topic , wmsg.msgs) << StringPrintF("] from %s at %p",wmsg.from?wmsg.from->FullName():"NULL" , wmsg.from);
   return os;
}


