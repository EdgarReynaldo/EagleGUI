
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
 *    Copyright 2009-2019+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file Allegro5WindowManager.hpp
 * @brief The interface for working with the Allegro 5 window manager
 */

#ifndef Allegro5WindowManager_HPP
#define Allegro5WindowManager_HPP



#include "Eagle/WindowManager.hpp"
#include "Eagle/Events.hpp"

#include "allegro5/allegro.h"


/**! @fn A5WindowManagerProcess <EagleThread* , void*>
 *   @brief The main process for the Allegro 5 window manager thread
 */

void* A5WindowManagerProcess(EagleThread* thread , void* manager);



class ALLEGRO_DISPLAY;
class Allegro5GraphicsContext;


/**! @class Allegro5WindowManager
 *   @brief The Allegro 5 implementation of the @ref EagleWindowManager class
 */

class Allegro5WindowManager : public EagleWindowManager {


protected :
   std::map<ALLEGRO_DISPLAY* , EagleGraphicsContext*> display_context_map;///< 

   ALLEGRO_EVENT_QUEUE* window_queue;
   ALLEGRO_EVENT_SOURCE window_event_source;

   ALLEGRO_EVENT_QUEUE* response_queue;
   ALLEGRO_EVENT_SOURCE response_event_source;



   friend class Allegro5GraphicsContext;

   friend void* A5WindowManagerProcess(EagleThread* , void*);///< Friend function to give access to our internals

   void AddDisplay(EagleGraphicsContext* window , ALLEGRO_DISPLAY* display);
   void RemoveDisplay(ALLEGRO_DISPLAY* display);

   void SignalClose(Allegro5GraphicsContext* window);




   virtual EagleGraphicsContext* PrivateCreateWindow(std::string objname , int width , int height , int flags);

   virtual int PrivateGiveWindowFocus(int window_eid);

   EagleEvent GetEagleDisplayEvent(ALLEGRO_EVENT ev);

public :

   Allegro5WindowManager(EagleSystem* sys , std::string objname);
   ~Allegro5WindowManager();

   bool Create();///< Handled automatically by the system
   void Destroy();///< Handled automatically by the system

   ///< Get the graphics context associated with the ALLEGRO_DISPLAY*
   EagleGraphicsContext* GetAssociatedContext(ALLEGRO_DISPLAY* display);
};



Allegro5WindowManager* GetAllegro5WindowManager();///< Get the Allegro 5 window manager



#endif // Allegro5WindowManager_HPP



