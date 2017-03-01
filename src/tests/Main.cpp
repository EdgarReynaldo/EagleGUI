

#include <cstdlib>


#include "TestRegistry.hpp"
#include "TestMenu.hpp"
#include "Eagle/backends/Allegro5Backend.hpp"


#include <cstdlib>


using namespace std;



string default_branch = "TextTestMain";

int main(int argc , char** argv) {
   
    
   SendOutputToFile("Libtest.txt" , "" , false);
   
/**
   vector<string> strs = SplitByDelimiterString("DIM:200,50 ; FONT:Verdana20 ; TEXT:Quit" , ";");
   EagleLog() << strs.size() << endl;
   for (int i = 0 ; i < (int)strs.size() ; ++i) {
      EagleLog() << strs[i] << endl;
   }
   return 0;
*/   
   
   sys = new Allegro5System();
   int state = 0;
   if ((state = sys->Initialize(EAGLE_FULL_SETUP)) != EAGLE_FULL_SETUP) {
      EagleLog() << PrintFailedEagleInitStates(EAGLE_FULL_SETUP , state) << std::endl;
      delete sys;
      return 1;
   }
   
   if (argc == 2) {
      int branch = atoi(argv[1]);
      if (branch < 0) {branch = 0;}
      int sz = TestRegistry::GetRegistryInstance().Size();
      if (branch >= sz) {branch = sz - 1;}
      vector<const Test*> tests = TestRegistry::GetRegistryInstance().GetRegisteredTests();
      
      MAINFUNC main_func = tests[branch]->MainFunc();
      
      TestRunner* runner = new TestRunner(0 , main_func);
      
      while (runner->Running()) {
         sys->Rest(0.001);
      }
      int ret = runner->return_value;
      if (ret == -1) {
         EagleLog() << runner->error_message << std::endl;
      }
      delete runner;
      return ret;
   }

   EagleGraphicsContext* main_window = sys->CreateGraphicsContext(800,600,EAGLE_WINDOWED);
   
   TestMenu test_menu(main_window);

   test_menu.Run();
   
   delete sys;
   sys = 0;
   
   return 0;
}



