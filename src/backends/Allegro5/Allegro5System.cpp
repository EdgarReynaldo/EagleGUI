

#include "Eagle.hpp"

#include "Eagle/backends/Allegro5/Allegro5System.hpp"
#include "Eagle/backends/Allegro5/Allegro5Timer.hpp"
#include "Eagle/backends/Allegro5/Allegro5EventHandler.hpp"
#include "Eagle/backends/Allegro5/Allegro5InputHandler.hpp"
#include "Eagle/backends/Allegro5/Allegro5GraphicsContext.hpp"
#include "Eagle/backends/Allegro5/Allegro5Threads.hpp"
#include "Eagle/backends/Allegro5/Allegro5Mutex.hpp"
#include "Eagle/backends/Allegro5/Allegro5Clipboard.hpp"


#include "allegro5/allegro.h"
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_audio.h"
#include "allegro5/allegro_acodec.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_ttf.h"
#include "allegro5/allegro_primitives.h"

#include <cstdlib>



bool Allegro5System::PrivateInitializeSystem() {
   // We can't call al_install_system(...,atexit) here because the allegro dll sometimes goes out of scope before our dll does
///   bool ret = al_init();
///   bool ret = al_install_system(ALLEGRO_VERSION_INT  , atexit);
   bool ret = al_install_system(ALLEGRO_VERSION_INT  , NULL);// this means we have to shutdown allegro
   if (!ret) {EagleLog() << "Allegro failed to initialize." << std::endl;}
   return ret;
}



bool Allegro5System::PrivateInitializeImages() {
   bool ret = al_init_image_addon();
   if (!ret) {EagleLog() << "Allegro failed to initialize image addon." << std::endl;}
   return ret;
}



bool Allegro5System::PrivateInitializeFonts() {
   al_init_font_addon();
   return true;
}



bool Allegro5System::PrivateInitializeTTFFonts() {
   bool ret = al_init_ttf_addon();
   if (!ret) {EagleLog() << "Allegro failed to initialize ttf font addon." << std::endl;}
   return ret;
}



bool Allegro5System::PrivateInitializeAudio() {
   bool ret = al_install_audio();
   ret = ret && al_init_acodec_addon();
   if (!ret) {EagleLog() << "Allegro failed to initialize audio addon." << std::endl;}
   return ret;
}



bool Allegro5System::PrivateInitializeShaders() {
   bool ret = true;///NOT NECESSARY CURRENTLY - // al_init_shader_addon(); // TODO : What?
   if (!ret) {EagleLog() << "Allegro failed to initialize shader addon." << std::endl;}
   return ret;
}



bool Allegro5System::PrivateInitializePrimitives() {
   bool ret = al_init_primitives_addon();
   if (!ret) {EagleLog() << "Allegro 5 failed to initialize primitives addon." << std::endl;}
   return ret;
}



bool Allegro5System::PrivateInstallKeyboard() {
   bool ret = al_install_keyboard();
   if (!ret) {EagleLog() << "Allegro 5 failed to install keyboard." << std::endl;}
   return ret;
}



bool Allegro5System::PrivateInstallMouse() {
   bool ret = al_install_mouse();
   if (!ret) {EagleLog() << "Allegro 5 failed to install mouse." << std::endl;}
   return ret;
}



bool Allegro5System::PrivateInstallJoystick() {
   bool ret = al_install_joystick();
   if (!ret) {EagleLog() << "Allegro 5 failed to install joystick." << std::endl;}
   return ret;
}



bool Allegro5System::PrivateInstallTouch() {
   bool ret = al_install_touch_input();
   if (!ret) {EagleLog() << "Allegro 5 failed to install touch." << std::endl;}
   return ret;
}



EagleInputHandler* Allegro5System::PrivateCreateInputHandler() {
   return new Allegro5InputHandler();
}



EagleEventHandler* Allegro5System::PrivateCreateEventHandler(bool delay_events) {
   Allegro5EventHandler* a5_handler = new Allegro5EventHandler(delay_events);
   if (!a5_handler->Create()) {
      delete a5_handler;
      return 0;
   }
   return a5_handler;
}



EagleTimer* Allegro5System::PrivateCreateTimer() {
   return new Allegro5Timer();
}



EagleGraphicsContext* Allegro5System::PrivateCreateGraphicsContext(int width , int height , int flags) {
   return new Allegro5GraphicsContext(width , height , flags);
}



EagleThread* Allegro5System::PrivateCreateThread(void* (*process)(EagleThread* , void*) , void* data) {
   EagleThread* ethread = new Allegro5Thread();
   if (!ethread->Create(process , data)) {
      delete ethread;
      throw EagleError("Failed to create an Allegro 5 thread.");
      return 0;
   }
   return ethread;
}



EagleMutex* Allegro5System::PrivateCreateMutex(bool recursive) {
   Allegro5Mutex* mutex = new Allegro5Mutex();
   if (!mutex->Create(recursive)) {
      delete mutex;
      throw EagleError(StringPrintF("Failed to create %sAllegro 5 mutex" , recursive?"recursive ":""));
      return 0;
   }
   return mutex;
}



EagleClipboard* Allegro5System::PrivateCreateClipboard() {
   return new Allegro5Clipboard();
}



Allegro5System::Allegro5System() :
      EagleSystem(StringPrintF("Allegro5System at %p" , this))
{}



Allegro5System::~Allegro5System() {Shutdown();}



void Allegro5System::Shutdown() {
   EagleSystem::Shutdown();
   al_uninstall_system();
}



double Allegro5System::GetProgramTime() {
   return al_get_time();
}



void Allegro5System::Rest(double time) {
   al_rest(time);
}



EagleGraphicsContext* Allegro5System::GetGraphicsContext(ALLEGRO_DISPLAY* allegro_display) {
   EagleGraphicsContext* context = 0;
   for (unsigned int i = 0 ; i < windows.size() ; ++i) {
      context = windows[i];
      Allegro5GraphicsContext* a5context = dynamic_cast<Allegro5GraphicsContext*>(context);
      EAGLE_ASSERT(a5context);
      if (a5context->AllegroDisplay() == allegro_display) {
         return context;
      }
   }
   return 0;
}



