
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



typedef pair<unsigned int,int> MESSAGE_KEY;
typedef pair<string,string> MESSAGE_VALUE;

typedef map<MESSAGE_KEY,MESSAGE_VALUE> MESSAGE_MAP; 



unsigned int NextFreeTopicId() {
   static unsigned int id = 0;
   return id++;
}



MESSAGE_MAP* GetMessageMap() {
   static MESSAGE_MAP mm;
   return &mm;
}



string GetMessageString(unsigned int topic , int message) {
   MESSAGE_MAP* mm = GetMessageMap();
   MESSAGE_MAP::iterator mit = mm->find(MESSAGE_KEY(topic,message));
   if (mit == mm->end()) {
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
   MESSAGE_MAP* mm = GetMessageMap();
   if (mm->find(MESSAGE_KEY(topic,message)) != mm->end()) {
      throw EagleException(StringPrintF("Message [%s] already registered!\n" , GetMessageString(topic,message).c_str()));
   }
   mm->insert(std::pair<MESSAGE_KEY , MESSAGE_VALUE>(MESSAGE_KEY(topic,message) , MESSAGE_VALUE(topic_str , message_str)));
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
   os << "Message [" << GetMessageString(wmsg.topic , wmsg.msgs) << 
         StringPrintF("] from %s at %p",wmsg.from?wmsg.from->FullName():"NULL" , wmsg.from);
   return os;
}



