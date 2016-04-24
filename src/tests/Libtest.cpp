

#include "Allegro5TestMain.hpp"
#include "RecordInputTestMain.hpp"
#include "EventHandlerDemoMain.hpp"
#include "LayoutTestMain.hpp"
#include "DrawingTestMain.hpp"
#include "GuiTestMain.hpp"

#include <cstdlib>

typedef int (*MAINFUNC)(int , char**);

#define NUM_BRANCHES 8

MAINFUNC branches[NUM_BRANCHES] = {
   Allegro5TestMain,
   RecordInputTestMain,
   EventHandlerDemoMain,
   LayoutTestMain,
   DrawingTestMain,
   GuiTestMain,
   GuiTestMain2,
   GuiTestMain3
};


int branch = 6;

int main(int argc , char** argv) {
   
   int userbranch = branch;
   if (argc == 2) {
      userbranch = atoi(argv[1]);
      if (userbranch < 0) {userbranch = 0;}
      if (userbranch > NUM_BRANCHES - 1) {userbranch = NUM_BRANCHES - 1;}
   }
   
   return branches[userbranch](argc , argv);
   
   return 0;
}

