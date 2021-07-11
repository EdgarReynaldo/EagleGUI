
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
 * @file Allegro5System.hpp
 * @brief The interface for the Allegro 5 system driver
 */

#ifndef Allegro5_System_HPP
#define Allegro5_System_HPP


#include "Eagle/System.hpp"

#include "allegro5/allegro.h"



extern const int ALLEGRO5_REGISTERED;///< TODO : This doesn't work in a DLL, because it doesn't exist until the DLL is loaded



/**! @class Allegro5System
 *   @brief The Allegro 5 system driver class
 */

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
   Transformer*          PrivateCreateTransformer();
   FileSystem*           PrivateCreateFileSystem();
   ResourceLibrary*      PrivateCreateResourceLibrary();

   friend class EagleLibrary;

   static EagleSystem* CreateAllegro5System();

   Allegro5System();

public :
   ~Allegro5System();

   void Shutdown();///< Shutdown the Allegro 5 system driver

   void Rest(double time);///< rests for 'time' seconds

   EagleGraphicsContext* GetGraphicsContext(ALLEGRO_DISPLAY* allegro_display);///< Get the graphics context for a given ALLEGRO_DISPLAY*

	const char* GetSystemName();///< Get the system name

   static int RegisterSystem();///< Registers the Allegro 5 system driver with Eagle, called automatically
};



Allegro5System* GetAllegro5System();///< Get the Allegro 5 system driver



#endif // Allegro5_System_HPP



