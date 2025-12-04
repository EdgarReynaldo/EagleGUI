


/// We need to include the main Eagle header.
/// This gives us access to everything Eagle has to offer.
#include "Eagle.hpp"
#include "physfs.h"

/// We need to include the backend header that we are going to use. 
/// It imports all of the necessary system and graphics code to use Allegro 5.
#include "Eagle/backends/Allegro5Backend.hpp"

int main(int argc , char** argv) {
   
   (void)argc;
   (void)argv;
   
   PHYSFS_init(argv[0]);
   
   /// To do most anything in Eagle, we need a system driver
   Allegro5System* sys = GetAllegro5System();
   EAGLE_ASSERT(sys);/// EAGLE_ASSERT is a regular assert but it logs and
                     /// throws an exception if null. EAGLE_DEBUG must
                     /// be defined or it will reduce to nothing.
   
   /// Now we have to initialize the system. Generally EAGLE_FULL_SETUP is used
   int sysret = sys->Initialize(EAGLE_FULL_SETUP);/// Return value is the
                                                  /// degree of success
   if (EAGLE_FULL_SETUP != sysret) {
      /// Some part of setup failed
      PrintFailedEagleInitStates(EAGLE_FULL_SETUP , sysret);/// Log the failure
      /// Test for ...
      if (!(sysret & EAGLE_STANDARD_SETUP)) {
         /// Failed to init an essential service
         return -1;
      }
      /// Just keep going. Either touch or shaders failed but we don't need
      /// them in this case
   }
   
   EagleLog() << "Setup eagle successfully" << std::endl;
   sys->Rest(3.0);

   ///< Now let's explore some advamced features of Eagle
/* EagleInputHandler*  GetSystemInput();///< Get the Eagle input device
   EagleEventHandler*  GetSystemQueue();///< Get the Eagle system event handler queue 
   EagleTimer*         GetSystemTimer();///< Get the Eagle system timer, ticks at 60 Hz.
   EagleClipboard*     GetSystemClipboard();///< Get the system clipboard
   EagleWindowManager* GetWindowManager();///< Get the window manager
   Transformer*        GetSystemTransformer();///< Get the system transformer matrix
   
   FileSystem*         GetFileSystem();///< Get the FileSystem handle
   ResourceLibrary*    GetResourceLibrary();///< Get the system wide resource library
   GraphicsHardware*   GetGraphicsHardware();///< Get the graphics hardware specs
   DialogManager*      GetDialogManager();///< Get the system dialog manager
*/
   
   /// Invoke our dialog mananger to determine whether the user wants to run OpenGL or DirectX
   DialogManager* dlg = sys->GetDialogManager();
   
   /// Now to test our hardware for graphics and see what resolutions are available in GRAPHICS_DIRECTX and GRAPHICS_OPENGL
   GRAPHICS_DRIVER gfx_driver = GRAPHICS_NONE;
   /// Grab a pointer to our Graphics Hardware from the system
   GraphicsHardware* gfxhw = sys->GetGraphicsHardware();
   for (unsigned int i = 1 ; i < 4 ; ++i) {/// Test GRAPHICS_SOFTWARE, GRAPHICS_OPENGL, and GRAPHICS_DIRECTX
      gfx_driver = (GRAPHICS_DRIVER)i;
      EagleLog() << StringPrintF("Using graphics driver %s" , GraphicsDriverToString(gfx_driver).c_str()) << std::endl;
      /// Get the adapters for this graphics driver
      std::vector<ADAPTER_INFO> adapter_info = gfxhw?gfxhw->GetAdapters(gfx_driver):std::vector<ADAPTER_INFO>();
      for (unsigned int j = 0 ; j < adapter_info.size() ; ++j) {/// For each adapter on this driver
         MONITOR_INFO moninfo = gfxhw->GetMonitor(gfx_driver , j);/// Grab the monitor info for this adapter
         EagleLog() << "Monitor " << j << ": x= " << moninfo.x << " y= " << moninfo.y << " W X H = " << moninfo.w << " X " << moninfo.h << std::endl;
         std::vector<MODE_INFO> mdinfo = gfxhw->GetModes(gfx_driver , j);/// Grab the modes for this monitor
         for (unsigned int k = 0 ; k < mdinfo.size() ; ++k) {
            MODE_INFO mode_info = mdinfo[k];/// For each mode
            EagleLog() << StringPrintF("Mode %i WxH = %ix%i format = %i refresh rate = %i" , k , mode_info.w , mode_info.h , mode_info.format , mode_info.refresh) << std::endl;
         }
      }
   }
   
   
   
   
   
   EagleGraphicsContext* win = sys->CreateGraphicsContext("GUI" , 800 , 600 , EAGLE_WINDOWED | EAGLE_OPENGL);

   /// Eagle works with files, folders, and archives through the FileSystem class
   /// First, get a pointer to the system's filesystem
   FileSystem* fs = sys->GetFileSystem();
   
   /// For an example, we're going to recursively go through the contents of Eagle.zip
//   std::shared_ptr<ArchiveFile> archive = fs->ReadArchive(FilePath("Eagle.zip"));
   std::shared_ptr<ArchiveFile> archive = fs->ReadArchive(FilePath("Data/Fonts/Fonts.7z"));
///   std::shared_ptr<ArchiveFile> archive = fs->ReadArchive(FilePath("Data/Fonts/Eagle.zip"));
///   std::shared_ptr<ArchiveFile> archive = fs->ReadArchive(FilePath("Data/Fonts/Eagle.zip"));

   archive->PrintContentsAbsolute();
   
   
   bool quit = false;
   bool redraw = true;
   sys->GetSystemTimer()->Start();/// The system timer is created automatically, but needs to be manually started
   
   while (!quit) {
      if (redraw) {
         win->Clear();
         win->FlipDisplay();
      }
      do {
         /// Let's handle two birds with one stone. WaitForEventAndUpdateState will do this for us.
         /// WaitForEvent will wait for an event from the system queue. Update state will update the input handler for us.
         /// We rarely want to do this manually.
         EagleEvent ee = sys->WaitForSystemEventAndUpdateState();
         if (ee.type == EAGLE_EVENT_TIMER) {
            redraw = true;
         }
         if (ee.type == EAGLE_EVENT_DISPLAY_CLOSE) {
            quit = true;
         }
         if ((ee.type == EAGLE_EVENT_KEY_DOWN) && (ee.keyboard.keycode == EAGLE_KEY_ESCAPE)) {
            quit = true;
         }
      } while (!sys->UpToDate());
   }
   
   
   return 0;
}
