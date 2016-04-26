
/*
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
 *    Copyright 2009-2013+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */



#ifndef EagleGuiWidgetMsg_HPP
#define EagleGuiWidgetMsg_HPP


#include <iostream>
#include <string>
#include <map>
#include <utility>



std::string GetMessageString(unsigned int topic , int message);



class RegisteredWidgetMessage {

public :   
   unsigned int topic;
   int message;
   std::string message_str;

   RegisteredWidgetMessage(unsigned int _topic , int _message , std::string _message_str);
   
};

#define REGISTER_WIDGET_MESSAGE(topic , msg) \
   static RegisteredWidgetMessage RWM_##msg(topic , msg , #msg)



class WidgetMessageMapCleaner {

public :
   ~WidgetMessageMapCleaner();

};



/** Returns the next available message id value to prevent message collision.
   Use TOPIC_* naming with

   static const unsigned int TOPIC_WHATEVER = NextFreeTopicId();
*/
unsigned int NextFreeTopicId();

static const unsigned int TOPIC_NONE = NextFreeTopicId();
static const int MESSAGE_NONE = 0;

REGISTER_WIDGET_MESSAGE(TOPIC_NONE , MESSAGE_NONE);




///void SetTopicAndMessageToStringsCallback(void (*callback)(unsigned int , int , std::string* , std::string* , bool*));




class WidgetBase;

class WidgetMsg {

public :
   
typedef unsigned int UINT;

   WidgetBase* from; // Address of the widget returning this message.
   UINT        topic;// Whatever this is in regards to, like it being about a dialog. Each widget class is
                     // assigned a value for this field from the NextFreeTopicId function.
   int         msgs; // Bitfield for setting message flags, or single message id. Up to the widget class.


   friend class WIDGET_EVENT_DATA;



   WidgetMsg();
   WidgetMsg(WidgetBase* widget_address , UINT widget_topic , int messages);
   WidgetMsg(const WidgetMsg& wmsg);

   ~WidgetMsg() {}
   
   inline WidgetBase* From()       const {return from;}
   inline UINT        Topic()      const {return topic;}
   inline int         Messages()   const {return msgs;}
   inline std::string      MessageStr() const {return GetMessageString(topic,msgs);}
   
   inline bool IsAbout(const WidgetBase* widget , UINT widget_topic) const {
      return ((from == widget) && (topic == widget_topic));
   }
   
   inline bool IsMessageTopic(UINT widget_topic , int message) const {
      return ((topic == widget_topic) && (msgs == message));
   }
   
   inline WidgetMsg& operator=(const WidgetMsg& wmsg) {
      from  = wmsg.from;
      topic = wmsg.topic;
      msgs  = wmsg.msgs;
      return (*this);
   }
   
   inline bool operator==(const WidgetMsg& wmsg) const {
      return ((from == wmsg.from) && (topic == wmsg.topic) && (msgs == wmsg.msgs));
   }

   friend std::ostream& operator<<(std::ostream& os , const WidgetMsg& wmsg);

};

/**
const WidgetMsg play(&button1 , WBUTTON , BTN_CLICKED);

while (gui->HasMessages()) {
   WidgetMsg wmsg = gui.TakeNextMessage();
   if (wmsg == play) {Play();}
}

const WidgetMsg quit_btn_close(&quit_button , TOPIC_DIALOG , DIALOG_CLOSE);

WidgetMsg wmsg = Gui->CheckInputs(mx,my);

if (wmsg == quit_btn_close) {Quit();}

//*/

#endif // EagleGuiWidgetMsg_HPP

