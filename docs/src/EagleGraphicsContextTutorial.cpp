


/// We need to include the main Eagle header.
/// This gives us access to everything Eagle has to offer.
#include "Eagle.hpp"

/// We need to include the backend header that we are going to use. 
/// It imports all of the necessary system and graphics code to use Allegro 5.
#include "Eagle/backends/Allegro5Backend.hpp"

int main(int argc , char** argv) {
   
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
      /// Test for essentials
      if (!(sysret & EAGLE_STANDARD_SETUP)) {
         /// Failed to init an essential service
         return -1;
      }
      /// Just keep going. Either touch or shaders failed but we don't need
      /// them in this case
   }
   
   EagleLog() << "Setup eagle successfully" << std::endl;
   
   /// Create a named window size 800x600 using an OpenGL window centered on the display
   EagleGraphicsContext* win = sys->CreateGraphicsContext("Main window" , 800 , 600 , EAGLE_OPENGL | EAGLE_WINDOWED , -1 , -1);
   
   /// Use our EAGLE_ASSERT mechanism to check the validity of the window
   EAGLE_ASSERT(win && win->Valid());
   
   /// Clear the window to the default color, which is black
   win->Clear();
   
   /// Update the screen by flipping the display - nothing will be shown until FlipDisplay() is called
   win->FlipDisplay();
   
   /// Wait 3 seconds so the window is visible
   sys->Rest(3.0);
   
   /// Clear the window to a specified EagleColor - in this case white
   win->Clear(EagleColor(255,255,255));
   
   /// Update the screen
   win->FlipDisplay();
   
   /// Wait 3 seconds
   sys->Rest(3.0);
   
   /// Return 0 to the system to indicate success - Eagle will clean itself up using the atexit routines
   return 0;
}











