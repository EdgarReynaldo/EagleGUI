

#include "Eagle.hpp"

#include "Eagle/backends/Allegro5/Allegro5System.hpp"
#include "Eagle/backends/Allegro5/Allegro5Timer.hpp"
#include "Eagle/backends/Allegro5/Allegro5EventHandler.hpp"
#include "Eagle/backends/Allegro5/Allegro5InputHandler.hpp"
#include "Eagle/backends/Allegro5/Allegro5GraphicsContext.hpp"
#include "Eagle/backends/Allegro5/Allegro5GraphicsHardware.hpp"
#include "Eagle/backends/Allegro5/Allegro5Threads.hpp"
#include "Eagle/backends/Allegro5/Allegro5Clipboard.hpp"
#include "Eagle/backends/Allegro5/Allegro5WindowManager.hpp"
#include "Eagle/backends/Allegro5/Allegro5FileSystem.hpp"
#include "Eagle/backends/Allegro5/Allegro5ResourceLib.hpp"



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
   if (!ret) {
      EagleCritical() << "Allegro failed to initialize." << std::endl;
   }
   else {
      al_register_assert_handler(EagleAssertHandler);
   }
   return ret;
}



bool Allegro5System::PrivateInitializeImages() {
   bool ret = al_init_image_addon();
   if (!ret) {EagleError() << "Allegro failed to initialize image addon." << std::endl;}
   return ret;
}



bool Allegro5System::PrivateInitializeFonts() {
   al_init_font_addon();
   return true;
}



bool Allegro5System::PrivateInitializeTTFFonts() {
   bool ret = al_init_ttf_addon();
   if (!ret) {EagleError() << "Allegro failed to initialize ttf font addon." << std::endl;}
   return ret;
}



bool Allegro5System::PrivateInitializeAudio() {
   bool ret = al_install_audio();
   ret = ret && al_init_acodec_addon();
   if (!ret) {EagleError() << "Allegro failed to initialize audio addon." << std::endl;}
   return ret;
}



bool Allegro5System::PrivateInitializeShaders() {
   bool ret = true;///NOT NECESSARY CURRENTLY - // al_init_shader_addon(); // TODO : What?
   if (!ret) {EagleError() << "Allegro failed to initialize shader addon." << std::endl;}
   return ret;
}



bool Allegro5System::PrivateInitializePrimitives() {
   bool ret = al_init_primitives_addon();
   if (!ret) {EagleError() << "Allegro 5 failed to initialize primitives addon." << std::endl;}
   return ret;
}



bool Allegro5System::PrivateInstallKeyboard() {
   bool ret = al_install_keyboard();
   if (!ret) {EagleError() << "Allegro 5 failed to install keyboard." << std::endl;}
   return ret;
}



bool Allegro5System::PrivateInstallMouse() {
   bool ret = al_install_mouse();
   if (!ret) {EagleError() << "Allegro 5 failed to install mouse." << std::endl;}
   return ret;
}



bool Allegro5System::PrivateInstallJoystick() {
   bool ret = al_install_joystick();
   if (!ret) {EagleError() << "Allegro 5 failed to install joystick." << std::endl;}
   return ret;
}



bool Allegro5System::PrivateInstallTouch() {
   bool ret = al_install_touch_input();
   if (!ret) {EagleError() << "Allegro 5 failed to install touch." << std::endl;}
   return ret;
}



EagleInputHandler* Allegro5System::PrivateCreateInputHandler(std::string objname) {
   return new Allegro5InputHandler(objname);
}



EagleEventHandler* Allegro5System::PrivateCreateEventHandler(std::string objname , bool delay_events) {
   Allegro5EventHandler* a5_handler = new Allegro5EventHandler(objname , delay_events);
   if (!a5_handler->Create()) {
      delete a5_handler;
      return 0;
   }
   return a5_handler;
}



EagleTimer* Allegro5System::PrivateCreateTimer(std::string objname) {
   return new Allegro5Timer(objname);
}



EagleGraphicsContext* Allegro5System::PrivateCreateGraphicsContext(std::string objname , int width , int height , int flags , int newx , int newy) {
   return window_manager->CreateGraphicsContext(objname , width , height , flags , newx , newy);
}



EagleThread* Allegro5System::PrivateCreateThread(std::string objname , void* (*process)(EagleThread* , void*) , void* data) {
   EagleThread* ethread = new Allegro5Thread(objname);
   if (!process) {return ethread;}
   if (!ethread->Create(process , data)) {
      delete ethread;
      throw EagleException("Failed to create an Allegro 5 thread.");
      return 0;
   }
   return ethread;
}



EagleMutex* Allegro5System::PrivateCreateMutex(std::string objname , bool recursive , bool timed) {
   CXX11Mutex* mutex = new CXX11Mutex(objname);
   if (!mutex->Create(recursive , timed)) {
      delete mutex;
      throw EagleException(StringPrintF("Failed to create %sAllegro 5 mutex" , recursive?"recursive ":""));
      return 0;
   }
   return mutex;
}



EagleClipboard* Allegro5System::PrivateCreateClipboard(std::string objname) {
   return new Allegro5Clipboard(objname);
}



EagleWindowManager* Allegro5System::PrivateCreateWindowManager() {
   return new Allegro5WindowManager(this , "A5WindowManager");
}



Transformer* Allegro5System::PrivateCreateTransformer() {
   return new Allegro5Transformer;
}



FileSystem* Allegro5System::PrivateCreateFileSystem() {
   return new Allegro5FileSystem();
}



ResourceLibrary* Allegro5System::PrivateCreateResourceLibrary() {
   return new Allegro5ResourceLibrary();
}



GraphicsHardware* Allegro5System::PrivateCreateGraphicsHardware() {
   return new Allegro5GraphicsHardware();
}



EagleSystem* Allegro5System::CreateAllegro5System() {
   return new Allegro5System();
}



Allegro5System::Allegro5System() :
      EagleSystem("Allegro5System" , "A5SYS")
{

}



Allegro5System::~Allegro5System() {
   Shutdown();
}



void Allegro5System::Shutdown() {
   EagleSystem::Shutdown();
   al_uninstall_system();
}



void Allegro5System::Rest(double time) {
   al_rest(time);
}



EagleGraphicsContext* Allegro5System::GetGraphicsContext(ALLEGRO_DISPLAY* allegro_display) {
   return dynamic_cast<Allegro5WindowManager*>(window_manager)->GetAssociatedContext(allegro_display);
}



const char* Allegro5System::GetSystemName() {
   static const char* name = "Allegro5";
   return name;
}



int Allegro5System::RegisterSystem() {
   return Eagle::EagleLibrary::RegisterSystemCreator("Allegro5" , Allegro5System::CreateAllegro5System);
}



const int ALLEGRO5_REGISTERED = Allegro5System::RegisterSystem();



Allegro5System* GetAllegro5System() {
   return dynamic_cast<Allegro5System*>(Eagle::EagleLibrary::System("Allegro5"));
}



