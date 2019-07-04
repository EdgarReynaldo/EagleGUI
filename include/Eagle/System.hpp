
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
 * @file System.hpp
 * @brief The interface for the @ref EagleSystem base class, which constitutes the base of every system driver in Eagle
 *
 *
 *
 *
 */

#ifndef EagleSystem_HPP
#define EagleSystem_HPP



#include "Eagle/Color.hpp"
#include "Eagle/Events.hpp"
#include "Eagle/GraphicsContext.hpp"
#include "Eagle/Image.hpp"
#include "Eagle/InputHandler.hpp"
#include "Eagle/Threads.hpp"
#include "Eagle/Mutexes.hpp"
#include "Eagle/Clipboard.hpp"
#include "Eagle/WindowManager.hpp"



/**! @enum EAGLE_INIT_STATE
 *   @brief This enum is for flags that you can pass to @ref System::Initialize
 * 
 *   Bitwise OR these flags together to specify the system components you wish to initialize
 */

enum EAGLE_INIT_STATE {
   EAGLE_NOT_INSTALLED =   0, ///< Nothing installed yet
   EAGLE_SYSTEM =     1 << 0, ///< Install the system
   EAGLE_IMAGES =     1 << 1, ///< Support images
   EAGLE_FONTS =      1 << 2, ///< Support fonts
   EAGLE_TTF_FONTS =  1 << 3, ///< Support ttf fonts
   EAGLE_AUDIO =      1 << 4, ///< Support audio
   EAGLE_SHADERS =    1 << 5, ///< Support shaders
   EAGLE_PRIMITIVES = 1 << 6, ///< Support primitive drawing
   EAGLE_KEYBOARD =   1 << 7, ///< Install the keyboard
   EAGLE_MOUSE =      1 << 8, ///< Install the mouse
   EAGLE_JOYSTICK =   1 << 9, ///< Install the joystick
   EAGLE_TOUCH =      1 << 10,///< Install the touch driver

   EAGLE_STANDARD_INPUT  = EAGLE_KEYBOARD | EAGLE_MOUSE | EAGLE_JOYSTICK,
   EAGLE_STANDARD_SYSTEM = EAGLE_SYSTEM | EAGLE_IMAGES | EAGLE_FONTS | EAGLE_TTF_FONTS | EAGLE_AUDIO | EAGLE_PRIMITIVES,
   EAGLE_STANDARD_SETUP  = EAGLE_STANDARD_INPUT | EAGLE_STANDARD_SYSTEM,

   EAGLE_FULL_INPUT      = EAGLE_STANDARD_INPUT | EAGLE_TOUCH,
   EAGLE_FULL_SYSTEM     = EAGLE_STANDARD_SYSTEM | EAGLE_SHADERS,
   EAGLE_FULL_SETUP      = EAGLE_FULL_INPUT | EAGLE_FULL_SYSTEM
};



extern const char* const eagle_init_state_strs[12];///< To convert an EAGLE_INIT_STATE bit flag to a string



class EagleSystem;



extern EagleEvent most_recent_system_event;///< A copy of the most recent system event



std::string PrintEagleInitState(int state);///< Print out the init flags

std::string PrintFailedEagleInitStates(int desired_state , int actual_state);///< Print out the failed init flags



class FileSystem;
class ResourceLibrary;



/**! @class EagleSystem
 *   @brief The base class for all system drivers in Eagle
 */

class EagleSystem : public EagleObject {

protected :

   PointerManager<EagleEventHandler> queues;
   PointerManager<EagleInputHandler> inputs;
   PointerManager<EagleTimer> timers;
   PointerManager<EagleThread> threads;
   PointerManager<EagleMutex> mutexes;
   PointerManager<EagleClipboard> clipboards;


   EagleInputHandler* input_handler;
   EagleTimer* system_timer;
   static float system_timer_rate;

   EagleEventHandler* system_queue;

   EagleClipboard* system_clipboard;

   EagleWindowManager* window_manager;

   FileSystem* file_system;
   
   ResourceLibrary* resource_library;
   
   
   
   bool system_up;
   bool images_up;
   bool fonts_up;
   bool ttf_fonts_up;
   bool audio_up;
   bool shaders_up;
   bool primitives_up;

   bool keyboard_running;
   bool mouse_running;
   bool joystick_running;
   bool touch_running;



   virtual bool PrivateInitializeSystem()=0;

   virtual bool PrivateInitializeImages()=0;
   virtual bool PrivateInitializeFonts()=0;
   virtual bool PrivateInitializeTTFFonts()=0;
   virtual bool PrivateInitializeAudio()=0;
   virtual bool PrivateInitializeShaders()=0;
   virtual bool PrivateInitializePrimitives()=0;

   virtual bool PrivateInstallKeyboard()=0;
   virtual bool PrivateInstallMouse()=0;
   virtual bool PrivateInstallJoystick()=0;
   virtual bool PrivateInstallTouch()=0;

   virtual EagleInputHandler*    PrivateCreateInputHandler   (std::string objname)=0;
   virtual EagleEventHandler*    PrivateCreateEventHandler   (std::string objname , bool delay_events = true)=0;
   virtual EagleTimer*           PrivateCreateTimer          (std::string objname)=0;
   virtual EagleGraphicsContext* PrivateCreateGraphicsContext(std::string objname , int width , int height , int flags)=0;
   virtual EagleThread*          PrivateCreateThread         (std::string objname , void* (*process)(EagleThread* , void*) , void* data)=0;
   virtual EagleMutex*           PrivateCreateMutex          (std::string objname , bool recursive , bool timed)=0;
   virtual EagleClipboard*       PrivateCreateClipboard      (std::string objname)=0;

   virtual EagleWindowManager*   PrivateCreateWindowManager()=0;
   virtual FileSystem*           PrivateCreateFileSystem()=0;
   virtual ResourceLibrary*      PrivateCreateResourceLibrary()=0;

   EagleWindowManager* CreateWindowManager() {return PrivateCreateWindowManager();}

   FileSystem* CreateFileSystem() {return PrivateCreateFileSystem();}
   
   ResourceLibrary* CreateResourceLibrary() {return PrivateCreateResourceLibrary();}
   
   
public :

   EagleSystem(std::string objclass = "EagleSystem" , std::string objname = "Nemo");

   virtual ~EagleSystem() {}///< Call Shutdown from your destructor please

   virtual void Shutdown();///< Call EagleSystem::Shutdown from your overridden Shutdown methods please...



   /**! @fn Initialize <int>
    *   @brief The main start up function for Eagle
    *   @param state
    *   Pass any combination of @ref EAGLE_INIT_STATE flags here, and those subsystems will be initialized.
    *   @retval The actual state of the system, composed of a bit field of successful EAGLE_INIT_STATE flags
    */
    
   virtual int Initialize(int state);

   bool InitializeSystem();
   bool FinalizeSystem();

   bool InitializeImages();
   bool InitializeFonts();
   bool InitializeTTFFonts();
   bool InitializeAudio();
   bool InitializeShaders();
   bool InitializePrimitives();

   virtual bool InitializeAll();// call this in overridden method calls

   bool InstallKeyboard();
   bool InstallMouse();
   bool InstallJoystick();
   bool InstallTouch();

   virtual bool InstallAll();// call this in overridden method calls


   bool SystemUp()        {return system_up;}
   bool ImagesUp()        {return images_up;}
   bool FontsUp()         {return fonts_up;}
   bool TTFFontsUp()      {return ttf_fonts_up;}
   bool AudioUp()         {return audio_up;}
   bool ShadersUp()       {return shaders_up;}
   bool PrimitivesUp()    {return primitives_up;}

   bool KeyboardRunning() {return keyboard_running;}
   bool MouseRunning()    {return mouse_running;}
   bool JoystickRunning() {return joystick_running;}
   bool TouchRunning()    {return touch_running;}

   int EagleInitState();

   EagleInputHandler*  GetSystemInput();
   EagleEventHandler*  GetSystemQueue();
   EagleTimer*         GetSystemTimer();
   EagleClipboard*     GetSystemClipboard();
   EagleWindowManager* GetWindowManager();
   
   FileSystem*         GetFileSystem();
   ResourceLibrary*    GetResourceLibrary();

	EagleInputHandler*    CreateInputHandler(std::string objname = "Nemo");
	
	EagleEventHandler*    CreateEventHandler(std::string objname = "Nemo" , bool delay_events = false);

	EagleTimer*           CreateTimer          (std::string objname = "Nemo");
   EagleGraphicsContext* CreateGraphicsContext(std::string objname = "Nemo" , int width = 0 , int height = 0 , int flags = 0);
   EagleThread*          CreateThread         (std::string objname = "Nemo" , void* (*process)(EagleThread* , void*) = 0 , void* data = 0);
   EagleMutex*           CreateMutex          (std::string objname = "Nemo" , bool recursive = false , bool timed = false);
   EagleClipboard*       CreateClipboard      (std::string objname = "Nemo");

   void FreeInputHandler(EagleInputHandler* handler);
   void FreeEventHandler(EagleEventHandler* event_handler);
   void FreeTimer(EagleTimer* timer);
   void FreeGraphicsContext(EagleGraphicsContext* window);
   void FreeThread(EagleThread* thread);
   void FreeMutex(EagleMutex* mutex);
   void FreeClipboard(EagleClipboard* clipboard);



   ///< Returns true if there are no system events waiting in the system queue
   bool UpToDate();

   /**! @fn UpdateSystemState
    *   @brief Sends one event to the input handler and updates itself 
    *   @retval The event processed 
    */
   EagleEvent UpdateSystemState();/// Keep calling this until UpToDate() returns true!

   /**! @fn WaitForSystemEventAndUpdateState
    *   @brief
    *
    *   If you don't install any input or create any displays
    *   WaitForSystemEventAndUpdateState might never return
    */
   EagleEvent WaitForSystemEventAndUpdateState();
   
   /**! @fn TimedWaitForSystemEventAndUpdateState <double>
    *   @param timeout Duration to wait in seconds
    *   @brief Waits up to @ref timeout seconds for an event and updates state if one is received.
    *   @retval The event processed
    */
   EagleEvent TimedWaitForSystemEventAndUpdateState(double timeout);

	/**! @fn Rest <double>
	 *   @brief Rest for @ref time seconds
	 *   @param time Duration to wait in seconds
	 * 
	 *   Pure virtual function. Must be implemented by system driver
	 */
	virtual void Rest(double time)=0;

	EagleGraphicsContext* GetActiveWindow();///< Get the active graphics context
	
	virtual const char* GetSystemName()=0;///< Pure virtual function to get the driver name

	static double GetProgramTime();///< Seconds since program start

};



#endif // EagleSystem_HPP





