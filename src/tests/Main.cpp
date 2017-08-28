

#include <cstdlib>


#include "Eagle.hpp"
#include "TestRegistry.hpp"
#include "TestMenu.hpp"
#include "Eagle/backends/Allegro5Backend.hpp"


#include <cstdlib>


using namespace std;

void* bad_thread(EagleThread* t , void* data) {

   void* ret = (void*)-1;

   EagleGraphicsContext* new_win = GetAllegro5System()->CreateGraphicsContext(240 , 180 , EAGLE_WINDOWED);
   new_win->SetOurThread(t);
   
   EagleEventHandler* q = GetAllegro5System()->CreateEventHandler(false);

   Allegro5Thread* a5thread = dynamic_cast<Allegro5Thread*>(t);

   EagleFont* dfont = new_win->DefaultFont();
   EAGLE_ASSERT(dfont);

///   q->ListenTo(GetAllegro5System()->GetInputHandler());
   q->ListenTo(new_win);
   q->ListenTo(GetAllegro5System()->GetSystemTimer());

   double pt = 0.0;
   double hrs = 0.0;
   double mins = 0.0;
   double secs = 0.0;
   bool redraw = true;
   bool quit = true;

   while (!quit && !a5thread->ShouldStop()) {


      if (redraw && new_win->StartDrawing()) {

         new_win->DrawToBackBuffer();
         EagleColor cc = EagleColor(0,0,0);
         if (GetAllegro5System()->GetActiveWindow() == new_win) {
            cc = EagleColor(0,255,0);
         }
         new_win->Clear(cc);
         new_win->DrawTextString(dfont , StringPrintF("ID : %d" , new_win->GetEagleId()) , 10 , 10 , EagleColor(255,255,255));
         new_win->DrawTextString(dfont , StringPrintF("%4.5lf" , pt) , 10 , new_win->Height() - 40 , EagleColor(255,255,255));
         new_win->DrawTextString(dfont , StringPrintF("%d:%d:%2.5lf" , (int)hrs , (int)mins , secs) , 10 , new_win->Height() - 20 , EagleColor(255,255,255));
         new_win->FlipDisplay();

         redraw = false;
         new_win->CompleteDrawing();
      }

      do {

         EagleEvent ee = q->WaitForEvent(1.0 , 0);
         if (ee.type == EAGLE_EVENT_DISPLAY_CLOSE) {
            ret = (void*)0;
            quit = true;
            break;
         }
/**
         else if (ee.type == EAGLE_EVENT_DISPLAY_HALT_DRAWING) {
            if (ee.window == new_win) {
               GetAllegro5System()->GetWindowManager()->AcknowledgeDrawingHalt(new_win);
               ret = (void*)0;
               break;
            }
         }
//*/
         if (ee.type == EAGLE_EVENT_TIMER) {
            pt = GetAllegro5System()->GetProgramTime();
///            pt = al_get_time();
            hrs = pt/3600;
            mins = hrs - (int)hrs;
            mins *= 60.0;
            secs = mins - (int)mins;
            mins = (int)mins;
            secs*= 60.0;
            redraw = true;
         }
      } while (q->HasEvent(0));
   }

   GetAllegro5System()->FreeEventHandler(q);
   GetAllegro5System()->FreeGraphicsContext(new_win);

   return ret;
}



void shutdown_main() {
   EagleInfo() << "EAGLE INFO : Shutting down main." << std::endl;
   return;
}


int main2(int argc , char** argv) {
   return 0;

}

int main(int argc , char** argv) {

   atexit(shutdown_main);

   SendOutputToFile("Libtest.txt" , "" , false);

   Allegro5System* a5sys = GetAllegro5System();//Eagle::EagleLibrary::System("Allegro5");

   a5sys->Initialize(EAGLE_FULL_SETUP);

   EagleGraphicsContext* win1 = a5sys->CreateGraphicsContext(400,300,EAGLE_WINDOWED);
   EagleGraphicsContext* win2 = a5sys->CreateGraphicsContext(400,300,EAGLE_WINDOWED);

   int window_count = 2;

   EagleEventHandler* queue = a5sys->GetSystemQueue();

   std::vector<EagleThread*> thread_list;

   a5sys->GetSystemTimer()->Start();

   while (1) {
      EagleEvent ee = queue->WaitForEvent(0);

      if (ee.type != EAGLE_EVENT_TIMER && ee.type != EAGLE_EVENT_MOUSE_AXES) {
         EagleInfo() << "Handling event " << EagleEventName(ee.type) << std::endl;
      }

      if (ee.type == EAGLE_EVENT_DISPLAY_CLOSE) {
         if (ee.window == win1) {
            EagleInfo() << "Close display receieved by main display." << std::endl;
            break;
         }
         a5sys->GetWindowManager()->DestroyWindow(ee.window->GetEagleId());
         window_count--;
         if (window_count == 0) {
            break;
         }
      }
///      if (ee.type == EAGLE_EVENT_DISPLAY_HALT_DRAWING) {
///         a5sys->GetWindowManager()->AcknowledgeDrawingHalt(ee.window);
///      }
      if (ee.type == EAGLE_EVENT_KEY_DOWN && ee.keyboard.keycode == EAGLE_KEY_ESCAPE) {
         break;
      }
      if (ee.type == EAGLE_EVENT_KEY_DOWN && ee.keyboard.keycode == EAGLE_KEY_N) {
         EagleThread* ethread = GetAllegro5System()->CreateThread();
         ethread->SetName(StringPrintF("New Window Thread #%3d" , ethread->GetEagleId()));
         ethread->Create(bad_thread , ethread);
         ethread->Start();
         thread_list.push_back(ethread);
         window_count++;
      }
   }

   for (int i = 0 ; i < (int)thread_list.size() ; ++i) {
      EagleThread* t = thread_list[i];
      t->Join();
   }

   Eagle::EagleLibrary::ShutdownEagle();

   return 0;
}




/*
string default_branch = "TextTestMain";

int main(int argc , char** argv) {


   SendOutputToFile("Libtest.txt" , "" , false);

   sys = GetAllegro5System();

///   atexit(Eagle::EagleLibrary::ShutdownEagle);

   int state = 0;
   if ((state = sys->Initialize(EAGLE_FULL_SETUP)) != EAGLE_FULL_SETUP) {
      EagleLog() << PrintFailedEagleInitStates(EAGLE_FULL_SETUP , state) << std::endl;
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

   Eagle::EagleLibrary::ShutdownEagle();

   return 0;
}

*/

