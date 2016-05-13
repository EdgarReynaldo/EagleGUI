

#include "Allegro5TestMain.hpp"
#include "RecordInputTestMain.hpp"
#include "EventHandlerDemoMain.hpp"
#include "LayoutTestMain.hpp"
#include "DrawingTestMain.hpp"
#include "GuiTestMain.hpp"
#include "NewGuiTestMain.hpp"
#include "TextTestMain.hpp"

#include <cstdlib>

typedef int (*MAINFUNC)(int , char**);

#define NUM_BRANCHES 10

MAINFUNC branches[NUM_BRANCHES] = {
   Allegro5TestMain,
   RecordInputTestMain,
   EventHandlerDemoMain,
   LayoutTestMain,
   DrawingTestMain,
   GuiTestMain,
   GuiTestMain2,
   GuiTestMain3,
   NewGuiTestMain,
   TextTestMain
};


int branch = 9;


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



int main(int argc , char** argv) {
   
   
   signal(SIGABRT, signal_handler);
   // _set_abort_behavior(0, _WRITE_ABORT_MSG|_CALL_REPORTFAULT);

    std::set_terminate( crash );
    std::set_unexpected( crash );
   // _set_purecall_handler( &terminator );
   // _set_invalid_parameter_handler( &invalid_parameter_handler );
    
 
    int userbranch = branch;
   if (argc == 2) {
      userbranch = atoi(argv[1]);
      if (userbranch < 0) {userbranch = 0;}
      if (userbranch > NUM_BRANCHES - 1) {userbranch = NUM_BRANCHES - 1;}
   }
   
   return branches[userbranch](argc , argv);
   
   return 0;
}

