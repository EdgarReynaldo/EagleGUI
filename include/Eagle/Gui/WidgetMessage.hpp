
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
 * @file WidgetMessage.hpp
 * @brief Interface for working with widget messages.
 * 
 */



#ifndef EagleGuiWidgetMsg_HPP
#define EagleGuiWidgetMsg_HPP


#include <iostream>
#include <string>
#include <map>
#include <utility>



std::string GetMessageString(unsigned int topic , int message);///< Get a message string for the specified topic and message


/*! @class RegisteredWidgetMessage
 *  @brief This class registers a new message upon creation 
 */

class RegisteredWidgetMessage {

public :   
   unsigned int topic;     ///< An unsigned integer holding the topic identifier
   std::string topic_str;  ///< A string representation of the topic
   int message;            ///< The integer message
   std::string message_str;///< A string representation of the message

   RegisteredWidgetMessage(unsigned int _topic , std::string _topic_str , int _message , std::string _message_str);
   
};

/*! \brief REGISTER_WIDGET_MESSAGE is a macro to declare an external widget message. Pair
           with REGISTERED_WIDGET_MESSAGE in a source file to finish the definition. */

#define REGISTER_WIDGET_MESSAGE(topic , msg) \
   extern RegisteredWidgetMessage RWM_##topic_##msg

/*! \brief REGISTERED_WIDGET_MESSAGE defines the widget message so it can be referred to elsewhere */
   
#define REGISTERED_WIDGET_MESSAGE(topic , msg) \
   RegisteredWidgetMessage RWM_##topic_##msg(topic , #topic , msg , #msg)


   
extern const unsigned int TOPIC_NONE;/// TOPIC used to indicate no topic

extern const int MESSAGE_NONE;/// MESSAGE used to indicate no message

REGISTER_WIDGET_MESSAGE(TOPIC_NONE , MESSAGE_NONE);/// Empty widget message used to signify no message



/*! \brief Returns the next available message id value to prevent message collision.

   Use TOPIC_* naming with
   ```
   static const unsigned int TOPIC_WHATEVER = NextFreeTopicId();
   ```
*/

unsigned int NextFreeTopicId();



class WidgetBase;

/*! @class WidgetMsg
 *  @brief The WidgetMsg class helps users distinguish between messages between different widgets.
 * 
 * Typical usage examples :
 * ```
 *     const WidgetMsg play(&button1 , TOPIC_BUTTON , BTN_CLICKED);
 *     
 *     while (gui->HasMessages()) {
 *        WidgetMsg wmsg = gui.TakeNextMessage();
 *        if (wmsg == play) {Play();}
 *     }
 *     
 *     const WidgetMsg quit_btn_close(&quit_button , TOPIC_DIALOG , DIALOG_CLOSE);
 *     
 *     WidgetMsg wmsg = Gui->CheckInputs(mx,my);
 *     
 *     if (wmsg == quit_btn_close) {Quit();}
 * ```
 * TODO : Separate out inline functions
 */

class WidgetMsg {

public :
   
typedef unsigned int UINT;

   WidgetBase* from; ///< Address of the widget returning this message.
   UINT        topic;///< Whatever this is in regards to, like it being about a dialog. Each widget class is
                     ///< assigned a value for this field from the NextFreeTopicId function.
   int         msgs; ///< Bitfield for setting message flags, or single message id. Up to the widget class.


   friend class WIDGET_EVENT_DATA;



   WidgetMsg();///< Empty widget message constructor
   WidgetMsg(WidgetBase* widget_address , UINT widget_topic , int messages);///< Stores the specified widget, topic, and message in this
   WidgetMsg(const WidgetMsg& wmsg);///< WidgetMsg copy constructor

   ~WidgetMsg() {}
   
   inline WidgetBase*  From()       const {return from;} ///< Returns the widget that this message originated from
   inline unsigned int Topic()      const {return topic;}///< Returns the topic of the widget message
   inline int          Message()    const {return msgs;} ///< Returns the message
   inline int          Messages()   const {return msgs;} ///< Returns the messages

   inline std::string      MessageStr() const {return GetMessageString(topic,msgs);}///< Returns a string holding the widget message
   
   inline bool IsAbout(const WidgetBase* widget , UINT widget_topic) const {///< Returns true if this message is about the specified
                                                                            ///< widget and topic
      return ((from == widget) && (topic == widget_topic));
   }
   
   inline bool IsMessageTopic(UINT widget_topic , int message) const {///< Returns true if this message matches the topic and message passed
      return ((topic == widget_topic) && (msgs == message));
   }
   
   inline WidgetMsg& operator=(const WidgetMsg& wmsg) {///< Widget msg assignment operator
      from  = wmsg.from;
      topic = wmsg.topic;
      msgs  = wmsg.msgs;
      return (*this);
   }
   
   inline bool operator==(const WidgetMsg& wmsg) const {///< WidgetMsg equivalence operator
      return ((from == wmsg.from) && (topic == wmsg.topic) && (msgs == wmsg.msgs));
   }

   friend std::ostream& operator<<(std::ostream& os , const WidgetMsg& wmsg);///< For stream output

};



#endif // EagleGuiWidgetMsg_HPP


