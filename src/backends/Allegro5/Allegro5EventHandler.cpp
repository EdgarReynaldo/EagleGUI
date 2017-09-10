

#include "Eagle/Lib.hpp"
#include "Eagle/Logging.hpp"
#include "Eagle/StringWork.hpp"
#include "Eagle/CXX11Mutexes.hpp"

#include "Eagle/backends/Allegro5/Allegro5GraphicsContext.hpp"
#include "Eagle/backends/Allegro5/Allegro5EventHandler.hpp"
#include "Eagle/backends/Allegro5/Allegro5InputHandler.hpp"
#include "Eagle/backends/Allegro5/Allegro5System.hpp"
#include "Eagle/backends/Allegro5/Allegro5WindowManager.hpp"




EagleEvent GetEagleEvent(ALLEGRO_EVENT ev) {
   EagleEvent ee;

   ee.type = (EAGLE_EVENT_TYPE)(int)ev.type;/// TODO : Translate allegro events into eagle events once they are actually different
   ee.timestamp = ev.any.timestamp;

   ee.window = GetAllegro5WindowManager()->GetActiveWindow();


/*
   EAGLE_EVENT_TYPE type;

   EagleEventSource* source;

   EagleGraphicsContext* window;

   double timestamp;// In seconds since program started
   union {
      KEYBOARD_EVENT_DATA keyboard;// keycode display unicode modifiers repeat
      MOUSE_EVENT_DATA mouse;// x y z w dx dy dz dw button display
      JOYSTICK_EVENT_DATA joystick;// id stick axis button pos
      TOUCH_EVENT_DATA touch;// display id x y dx dy primary
      TIMER_EVENT_DATA timer;// source count
      DISPLAY_EVENT_DATA display;// source x y width height orientation
      WIDGET_EVENT_DATA widget;
      USER_EVENT_DATA data;
   };

*/

   switch (ev.type) {
      case ALLEGRO_EVENT_JOYSTICK_AXIS :
      case ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN :
      case ALLEGRO_EVENT_JOYSTICK_BUTTON_UP :
      case ALLEGRO_EVENT_JOYSTICK_CONFIGURATION :
         ee.joystick.nid = get_joystick_n(ev.joystick.id);
         if (ee.joystick.nid == -1) {
            ee.joystick.id = 0;
         }
         else {
				ee.joystick.id = &joysticks[ee.joystick.nid];
         }
         ee.joystick.stick = ev.joystick.stick;
         ee.joystick.axis = ev.joystick.axis;
         ee.joystick.button = ev.joystick.button;
         ee.joystick.pos = ev.joystick.pos;
         break;
      case ALLEGRO_EVENT_KEY_DOWN :
      case ALLEGRO_EVENT_KEY_UP :
      case ALLEGRO_EVENT_KEY_CHAR :
         ee.window = GetAllegro5WindowManager()->GetAssociatedContext(ev.keyboard.display);
         ee.keyboard.keycode = ev.keyboard.keycode;
         ee.keyboard.unicode = ev.keyboard.unichar;
         ee.keyboard.modifiers = ev.keyboard.modifiers;
         ee.keyboard.repeat = ev.keyboard.repeat;
         break;
      case ALLEGRO_EVENT_MOUSE_AXES :
      case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN :
      case ALLEGRO_EVENT_MOUSE_BUTTON_UP :
      case ALLEGRO_EVENT_MOUSE_WARPED :
      case ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY :
      case ALLEGRO_EVENT_MOUSE_LEAVE_DISPLAY :
         ee.window = GetAllegro5WindowManager()->GetAssociatedContext(ev.mouse.display);
         ee.mouse.x = ev.mouse.x;
         ee.mouse.y = ev.mouse.y;
         ee.mouse.z = ev.mouse.z;
         ee.mouse.w = ev.mouse.w;
         ee.mouse.dx = ev.mouse.dx;
         ee.mouse.dy = ev.mouse.dy;
         ee.mouse.dz = ev.mouse.dz;
         ee.mouse.dw = ev.mouse.dw;
         ee.mouse.button = ev.mouse.button;
         break;
      case ALLEGRO_EVENT_TIMER :
         ee.timer.eagle_timer_source = 0;// did not come from an Eagletimer
         ee.timer.raw_source = ev.timer.source;
         ee.timer.count = ev.timer.count;
         break;
      case ALLEGRO_EVENT_DISPLAY_EXPOSE :
      case ALLEGRO_EVENT_DISPLAY_RESIZE :
      case ALLEGRO_EVENT_DISPLAY_CLOSE :
      case ALLEGRO_EVENT_DISPLAY_LOST :
      case ALLEGRO_EVENT_DISPLAY_FOUND :
      case ALLEGRO_EVENT_DISPLAY_SWITCH_OUT :
      case ALLEGRO_EVENT_DISPLAY_SWITCH_IN :
      case ALLEGRO_EVENT_DISPLAY_ORIENTATION :
      case ALLEGRO_EVENT_DISPLAY_HALT_DRAWING :
      case ALLEGRO_EVENT_DISPLAY_RESUME_DRAWING :
         ee.window = GetAllegro5WindowManager()->GetAssociatedContext(ev.display.source);
         ee.display.x = ev.display.x;
         ee.display.y = ev.display.y;
         ee.display.width = ev.display.width;
         ee.display.height = ev.display.height;
         ee.display.orientation = ev.display.orientation;
         break;

      case ALLEGRO_EVENT_TOUCH_BEGIN :
      case ALLEGRO_EVENT_TOUCH_END :
      case ALLEGRO_EVENT_TOUCH_MOVE :
      case ALLEGRO_EVENT_TOUCH_CANCEL :
         ee.window = GetAllegro5WindowManager()->GetAssociatedContext(ev.touch.display);
         ee.touch.id = ev.touch.id;
         ee.touch.x = ev.touch.x;
         ee.touch.y = ev.touch.y;
         ee.touch.dx = ev.touch.dx;
         ee.touch.dy = ev.touch.dy;
         ee.touch.primary = ev.touch.primary;
         break;
      default : break;
   }
   return ee;
}



void* Allegro5EventThreadProcess(EagleThread* thread , void* event_handler) {

   Allegro5EventHandler* a5_event_handler = dynamic_cast<Allegro5EventHandler*>((EagleEventHandler*)event_handler);
   EAGLE_ASSERT(a5_event_handler);
   EAGLE_ASSERT(a5_event_handler->Valid());

   ALLEGRO_EVENT_QUEUE* queue = a5_event_handler->event_queue;
   ALLEGRO_EVENT_SOURCE* main_source = &(a5_event_handler->main_source);
   EagleConditionVar* cond_var = a5_event_handler->cond_var;

   EagleInfo() << StringPrintF("Allegro5EventThreadProcess process starting on EagleThread %p" , thread) << std::endl;

   bool close = false;
   while (!thread->ShouldStop() && !close) {
      ALLEGRO_EVENT ev;
      al_wait_for_event(queue , &ev);

//      EagleLog() << "Event #" << ev.type << " received." << std::endl;
      if (ev.type == EAGLE_EVENT_USER_START) {
         if (ev.user.source == main_source) {// signalled through queue by another thread
            switch (ev.user.data1) {
               case CLOSE_EVENT_THREAD :
                  EagleInfo() << StringPrintF("Allegro5EventThreadProcess signaled to close thread %p." , thread) << std::endl;
                  close = true;
                  break;
               default :
                  EagleWarn() << "Event not recognized by event thread process." << std::endl;
                  break;

            }
         }
      }
      else {
         EagleEvent ee = GetEagleEvent(ev);
         ee.source = a5_event_handler;
         a5_event_handler->PushEvent(ee , thread);
         cond_var->BroadcastCondition();// alert any thread waiting on the condition (an event)
      }
   }

   EagleInfo() << StringPrintF("Allegro5EventThreadProcess process stopping on EagleThread %p" , thread) << std::endl;

   return event_handler;
}



/// ---------------------      Allegro5EventHandler      ----------------------------



bool Allegro5EventHandler::Running() {
   return (event_thread && event_thread->Running());
}



Allegro5EventHandler::Allegro5EventHandler(bool delay_emitted_events , std::string objname) :
      EagleEventHandler("Allegro5EventHandler" , objname , delay_emitted_events),
      event_queue(0),
      main_source(),
      event_thread(0)
{
   SetName("A5EH");
   al_init_user_event_source(&main_source);
}



Allegro5EventHandler::~Allegro5EventHandler() {
   Destroy();
}



bool Allegro5EventHandler::Create() {
   EAGLE_ASSERT(Eagle::EagleLibrary::Eagle()->System("Allegro5"));// System must be initialized and running
   Destroy();

   event_queue = al_create_event_queue();
   al_register_event_source(event_queue , &main_source);

   mutex = new CXX11Mutex();
   mutex->SetShortName("A5EH");
   
   event_thread = new Allegro5Thread();
   event_thread->SetShortName("A5EH");
///   SetName(StringPrintF("%s Thread (EID = %d)" , GetNameCStr() , event_thread->GetEagleId()));
   SetOurThread(event_thread);
   
   cond_var = new Allegro5ConditionVar();

   /// don't call create on thread until queue, mutex, and condvar are in place
   if (!event_queue ||
       !mutex->Create(false , false) ||
       !cond_var->Create() ||
       !event_thread->Create(Allegro5EventThreadProcess , this)
      )
   {
      if (!event_queue) {
         EagleError() << "Allegro5EventHandler::Create : Failed to create event_queue." << std::endl;
      }
      else if (!mutex->Valid()) {
         EagleError() << "Allegro5EventHandler::Create : Failed to create valid mutex." << std::endl;
      }
      else if (!cond_var->Valid()) {
         EagleError() << "Allegro5EventHandler::Create : Failed to create valid condition variable." << std::endl;
      }
      else {
         EagleError() << "Allegro5EventHandler::Create : Failed to create Allegro5EventThreadProcess thread." << std::endl;
      }

      Destroy();
      return false;
   }

   event_thread->Start();

   return true;
}




void Allegro5EventHandler::Destroy() {

   if (!event_thread) {return;}
   /// MUST STOP PROCESS FIRST
   bool running = Running();
   EagleInfo() << StringPrintF("Allegro5EventHandler::Destroy (this = %p) - on entrance : Running = %s , on EagleThread %p" , this , running?"true":"false" , event_thread) << std::endl;
   if (running) {
      ALLEGRO_EVENT ev;
      ev.type = EAGLE_EVENT_USER_START;
      ev.any.source = &main_source;
      ev.any.timestamp = al_get_time();
      ev.user.source = &main_source;
      ev.user.data1 = CLOSE_EVENT_THREAD;
      EagleInfo() << StringPrintF("Allegro5EventHandler::Destroy (this = %p) - emitting close event to EagleThread %p" , this , event_thread) << std::endl;
      al_emit_user_event(&main_source , &ev , NULL);// tell event thread to close
      event_thread->FinishThread();
   }

   if (event_thread) {
      delete event_thread;
      event_thread = 0;
      our_thread = 0;
   }
   if (mutex) {
      delete mutex;
      mutex = 0;
   }
   if (cond_var) {
      delete cond_var;
      cond_var = 0;
   }
   if (event_queue) {
      al_destroy_event_queue(event_queue);
      event_queue = 0;
   }

}



bool Allegro5EventHandler::Valid() {
   return (event_queue &&
           mutex &&
           mutex->Valid() &&
           cond_var &&
           cond_var->Valid() &&
           event_thread &&
           event_thread->Valid()
         );
}



void Allegro5EventHandler::RespondToEvent(EagleEvent ee , EagleThread* thread) {
   EagleEventHandler::RespondToEvent(ee , thread);// emits and queues message
   /// now wake any threads waiting on us
   cond_var->BroadcastCondition();
}





