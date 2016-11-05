

#include <cstdlib>


#include "TestRegistry.hpp"

#include "Eagle/backends/Allegro5Backend.hpp"


#include <signal.h>
#include <cstdlib>

inline void crash() 
{
   int x = 1/0;
   (void)x;
}

inline void signal_handler(int)
{
    crash();
}

inline void __cdecl invalid_parameter_handler(const wchar_t *, const wchar_t *, const wchar_t *, unsigned int, uintptr_t)
{
   crash();
}


const char* default_branch = "TextTestMain";

int main(int argc , char** argv) {
   
   signal(SIGABRT, signal_handler);
   // _set_abort_behavior(0, _WRITE_ABORT_MSG|_CALL_REPORTFAULT);

    std::set_terminate( crash );
    std::set_unexpected( crash );
   // _set_purecall_handler( &terminator );
   // _set_invalid_parameter_handler( &invalid_parameter_handler );
    
   sys = new Allegro5System();
   if (sys->Initialize(EAGLE_FULL_SETUP) != EAGLE_FULL_SETUP) {
      delete sys;
      return 1;
   }
   
   EagleGraphicsContext* main_window = sys->CreateGraphicsContext(800,600,EAGLE_WINDOWED);
   
   TestMenu menu(main_window);
   
   
   bool user_selected_branch = false;
   
   const char* userbranch = default_branch;
   if (argc == 2) {
      user_selected_branch = true;
      int branch = atoi(argv[1]);
      if (branch < 0) {branch = 0;}
      if (branch > test_registry.NumBranches() - 1) {branch = test_registry.NumBranches() - 1;}
   }

   bool quit = false;
   while (!quit) {
   
      if (user_selected_branch) {
         int retval = 0;
         try {
            /// TODO : Set argc and argv
            retval = test_registry.Run(userbranch , argc , argv);
         }
         catch (EagleError error) {
            /// Ignore error and continue running test, it will be logged anyway
         }
      }
      
      TestMenu.Run();
      
      quit = test_menu.Quit();
      
      if (!quit) {
         userbranch = test_menu.SelectedBranch();
         user_selected_branch = true;
      }
   
   }
   
   delete sys;
   sys = 0;
   
   return 0;
}

