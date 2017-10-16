

#ifndef Allegro5_System_HPP
#define Allegro5_System_HPP


#include "Eagle/System.hpp"

#include "allegro5/allegro.h"


class Allegro5System : public EagleSystem {

private :

   virtual bool PrivateInitializeSystem();

   virtual bool PrivateInitializeImages();
   virtual bool PrivateInitializeFonts();
   virtual bool PrivateInitializeTTFFonts();
   virtual bool PrivateInitializeAudio();
   virtual bool PrivateInitializeShaders();
   virtual bool PrivateInitializePrimitives();

   virtual bool PrivateInstallKeyboard();
   virtual bool PrivateInstallMouse();
   virtual bool PrivateInstallJoystick();
   virtual bool PrivateInstallTouch();

   EagleInputHandler*    PrivateCreateInputHandler   (std::string objname);
   EagleEventHandler*    PrivateCreateEventHandler   (std::string objname , bool delay_events = true);
   EagleTimer*           PrivateCreateTimer          (std::string objname);
   EagleGraphicsContext* PrivateCreateGraphicsContext(std::string objname , int width , int height , int flags);
   EagleThread*          PrivateCreateThread         (std::string objname , void* (*process)(EagleThread* , void*) , void* data);
   EagleMutex*           PrivateCreateMutex          (std::string objname , bool recursive , bool timed);
   EagleClipboard*       PrivateCreateClipboard      (std::string objname);

   EagleWindowManager*   PrivateCreateWindowManager();

   friend class EagleLibrary;

   static EagleSystem* CreateAllegro5System();

   Allegro5System();

public :
   ~Allegro5System();

   void Shutdown();

   double GetProgramTime();
   void Rest(double time);

   EagleGraphicsContext* GetGraphicsContext(ALLEGRO_DISPLAY* allegro_display);

	const char* GetSystemName();

   static int RegisterSystem();
};

///EagleSystem* CreateA5System();

Allegro5System* GetAllegro5System();

#endif // Allegro5_System_HPP

