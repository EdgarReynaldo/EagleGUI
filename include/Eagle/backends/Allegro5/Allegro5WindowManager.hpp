

#ifndef Allegro5WindowManager_HPP
#define Allegro5WindowManager_HPP



#include "Eagle/WindowManager.hpp"
#include "Eagle/Events.hpp"


#include "allegro5/allegro.h"



void* A5WindowManagerProcess(EagleThread* thread , void* manager);


class ALLEGRO_DISPLAY;
class Allegro5GraphicsContext;



class Allegro5WindowManager : public EagleWindowManager {

   friend void* A5WindowManagerProcess(EagleThread* , void*);

protected :


   std::map<ALLEGRO_DISPLAY* , EagleGraphicsContext*> display_context_map;

   ALLEGRO_EVENT_QUEUE* window_queue;
   ALLEGRO_EVENT_SOURCE window_event_source;

   ALLEGRO_EVENT_QUEUE* response_queue;
   ALLEGRO_EVENT_SOURCE response_event_source;



   friend class Allegro5GraphicsContext;

   void AddDisplay(EagleGraphicsContext* window , ALLEGRO_DISPLAY* display);
   void RemoveDisplay(ALLEGRO_DISPLAY* display);

   void SignalClose(Allegro5GraphicsContext* window);




   virtual EagleGraphicsContext* PrivateCreateWindow(int width , int height , int flags);

   virtual int PrivateGiveWindowFocus(int window_eid);

   EagleEvent GetEagleDisplayEvent(ALLEGRO_EVENT ev);

public :

   Allegro5WindowManager(EagleSystem* sys , std::string objname = "Nemo");
   ~Allegro5WindowManager();

   bool Create();
   void Destroy();

   EagleGraphicsContext* GetAssociatedContext(ALLEGRO_DISPLAY* display);

///   virtual void AcknowledgeDrawingHalt(EagleGraphicsContext* window);

};

Allegro5WindowManager* GetAllegro5WindowManager();



#endif // Allegro5WindowManager_HPP
