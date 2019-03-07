


#include <string>
#include <cstdarg>
#include <iostream>

unsigned int STRINGPRINTFEX_BUFFER_SIZE = 1024;



std::string StringPrintFex(const char* format_str , ...)  __attribute__ ((format (printf, 1, 2)));
std::string StringPrintFex(const char* format_str , ...) {
   char buffer[STRINGPRINTFEX_BUFFER_SIZE];
   va_list args;
   va_start(args , format_str);
///int vsnprintf (char * s, size_t n, const char * format, va_list arg );
   vsnprintf(buffer , STRINGPRINTFEX_BUFFER_SIZE , format_str , args);
   va_end(args);
   return std::string(buffer);
}

std::string GetRandomString() {
   static int rand = 0;
   if (!rand) {
      rand = 1;
      return "Random";
   }
   else {
      rand = 0;
      return "String";
   }
   
}

int main3(int argc , char** argv) {
   
   (void)argc;
   (void)argv;
   
///   std::cout << StringPrintFex("Bad string : %s\nBad arg : %s" , GetRandomString().c_str() , GetRandomString()) << std::endl;
   
   return 0;
}

#define ALLEGRO_UNSTABLE



#include <cstdlib>


#include "Eagle.hpp"
#include "TestRegistry.hpp"
#include "TestMenu.hpp"
#include "Eagle/backends/Allegro5Backend.hpp"


#include <cstdlib>


int main(int argc , char** argv) {
   
   (void)argc;
   (void)argv;
   
   Allegro5System* a5sys = GetAllegro5System();
   
   a5sys->Initialize(EAGLE_FULL_SETUP);
   
   EagleGraphicsContext* main_win = GetAllegro5WindowManager()->CreateWindow("TINS:main_win" , 1024 , 768 , EAGLE_OPENGL | EAGLE_WINDOWED);
   
   EAGLE_ASSERT(main_win && main_win->Valid());
   
   main_win->DrawToBackBuffer();
   main_win->Clear(EagleColor(0,0,0));
   main_win->FlipDisplay();
   
   
   
   return 0;
}







using namespace std;

void* bad_thread(EagleThread* t , void* data) {

   (void)data;
   void* ret = (void*)-1;

   EagleGraphicsContext* new_win = GetAllegro5System()->CreateGraphicsContext("bad_thread::new_win" , 240 , 180 , EAGLE_WINDOWED);
   EAGLE_ASSERT(new_win && new_win->Valid());

   new_win->SetOurThread(t);

   EagleEventHandler* q = GetAllegro5System()->CreateEventHandler("bad_thread::q" , false);

   Allegro5Thread* a5thread = dynamic_cast<Allegro5Thread*>(t);

   EagleFont* f = new_win->DefaultFont();
   EAGLE_ASSERT(f && f->Valid());

   q->ListenTo(GetAllegro5System()->GetSystemQueue());
///   q->ListenTo(GetAllegro5System()->GetInputHandler());
///   q->ListenTo(new_win , t);
///   q->ListenTo(GetAllegro5System()->GetSystemTimer() , t);

   double pt = 0.0;
   double st = GetAllegro5System()->GetProgramTime();
   double hrs = 0.0;
   double mins = 0.0;
   double secs = 0.0;
   bool redraw = true;
   bool quit = false;

   while (!quit) {

      if (a5thread->ShouldStop()) {
         EagleInfo() << StringPrintF("Eagle thread %d signalled to stop." , t->GetEagleId()) << std::endl;
         break;
      }


      if (redraw && new_win->StartDrawing(t)) {

         new_win->DrawToBackBuffer();
         EagleColor cc = EagleColor(0,0,0);
         if (GetAllegro5System()->GetActiveWindow() == new_win) {
            cc = EagleColor(0,255,0);
         }
         new_win->Clear(cc);
         new_win->DrawTextString(f , StringPrintF("ID : %d" , new_win->GetEagleId()) , 10 , 10 , EagleColor(255,255,255));
         new_win->DrawTextString(f , StringPrintF("%4.5lf" , pt) , 10 , new_win->Height() - 40 , EagleColor(255,255,255));
         new_win->DrawTextString(f , StringPrintF("%d:%d:%2.5lf" , (int)hrs , (int)mins , secs) , 10 , new_win->Height() - 20 , EagleColor(255,255,255));
         new_win->FlipDisplay();

         redraw = false;
         new_win->CompleteDrawing(t);
      }

      do {

         EagleEvent ee = q->WaitForEvent(1.5 , t);
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
            pt = GetAllegro5System()->GetProgramTime() - st;
///            pt = al_get_time();
            hrs = pt/3600;
            mins = hrs - (int)hrs;
            mins *= 60.0;
            secs = mins - (int)mins;
            mins = (int)mins;
            secs*= 60.0;
            redraw = true;
         }
      } while (q->HasEvent(t));
   }

   GetAllegro5System()->FreeEventHandler(q);
   GetAllegro5System()->FreeGraphicsContext(new_win);

   EagleInfo() << "EXITING BAD THREAD" << std::endl;

   return ret;
}



void shutdown_main() {
   EagleInfo() << "EAGLE INFO : Shutting down main." << std::endl;
   return;
}


/**
///#include "AllegroMultiWin.hpp"

///#include "EagleMultiWin.hpp"

///int main8(int argc , char** argv) {

///   PROG* prog = new PROG();
///   PROGGIE* prog = new PROGGIE();

///   delete prog;

///   return 0;
///}
*/



int main4(int argc , char** argv) {

   (void)argc;
   (void)argv;

   SendOutputToFile("Libtest.txt" , "" , false);

   Allegro5System* a5sys = GetAllegro5System();//Eagle::EagleLibrary::System("Allegro5");

   a5sys->Initialize(EAGLE_FULL_SETUP);

   EagleGraphicsContext* win1 = a5sys->CreateGraphicsContext("main::win1" , 400,300,EAGLE_WINDOWED);
   EAGLE_ASSERT(win1 && win1->Valid());

   EagleEventHandler* queue = a5sys->GetSystemQueue();

   std::vector<EagleThread*> thread_list;

   a5sys->GetSystemTimer()->Start();

   bool redraw = true;
   bool quit = false;
   
   while (!quit) {
      
      EagleGraphicsContext* active_win = a5sys->GetActiveWindow();
      EAGLE_ID active_win_id = EAGLE_ID_UNASSIGNED;
      if (active_win) {
         active_win_id = active_win->GetEagleId();
      }
      if (redraw) {
         win1->DrawToBackBuffer();
         win1->Clear(EagleColor(255,255,255));
         win1->DrawTextString(win1->DefaultFont() , StringPrintF("Main Window EID %d" , win1->GetEagleId()) , 10 , 10 , EagleColor(0,0,0));
         win1->DrawTextString(win1->DefaultFont() , StringPrintF("Active Window EID %d" , active_win_id) , 10 , 30 , EagleColor(0,0,0));
         win1->FlipDisplay();
         redraw = false;
      }
         
      EagleEvent ee = queue->WaitForEvent(0);

      if (ee.type == EAGLE_EVENT_TIMER) {
         redraw = true;
      }
      
      if (ee.type != EAGLE_EVENT_TIMER && ee.type != EAGLE_EVENT_MOUSE_AXES) {
         EagleInfo() << "Handling event " << EagleEventName(ee.type) << std::endl;
      }

      if (ee.type == EAGLE_EVENT_DISPLAY_CLOSE) {
///         a5sys->GetWindowManager()->DestroyWindow(ee.window->GetEagleId());
         if (ee.window == win1) {
            quit = true;
            break;
         }
      }
///      if (ee.type == EAGLE_EVENT_DISPLAY_HALT_DRAWING) {
///         a5sys->GetWindowManager()->AcknowledgeDrawingHalt(ee.window);
///      }
      if (ee.type == EAGLE_EVENT_KEY_DOWN && ee.keyboard.keycode == EAGLE_KEY_ESCAPE) {
         quit = true;
         break;
      }
      if (ee.type == EAGLE_EVENT_KEY_DOWN && ee.keyboard.keycode == EAGLE_KEY_N) {
         EagleThread* ethread = GetAllegro5System()->CreateThread("ethread");
         ethread->Create(bad_thread , ethread);
         ethread->Start();
         thread_list.push_back(ethread);
      }
   }

   for (int i = 0 ; i < (int)thread_list.size() ; ++i) {
      EagleThread* t = thread_list[i];
      t->Join();
   }

   Eagle::EagleLibrary::ShutdownEagle();

   return 0;
}



void* test_thread(EagleThread* t , void* data) {

   (void)data;
   EagleInfo() << StringPrintF("Started test_thread on EagleThread %s" , t->FullName()) << std::endl;

   int j = 0;
   while (j < 5) {
///      GetAllegro5System()->GetSystemQueue()->WaitForEvent(EAGLE_EVENT_TIMER);
      al_rest(1.0);
      ++j;
      if (t->ShouldStop()) {break;}
   }

   EagleInfo() << StringPrintF("Stopping test_thread on EagleThread %s" , t->FullName()) << std::endl;

   return (void*)j;
}


int main2(int argc , char** argv) {

   (void)argc;
   (void)argv;

   SendOutputToFile("Libtest.txt" , "" , false);

   Allegro5System* a5sys = GetAllegro5System();
   a5sys->Initialize(EAGLE_FULL_SETUP);
   a5sys->GetSystemTimer()->SetSecondsPerTick(1);
   a5sys->GetSystemTimer()->Start();

   int NTHREADS = 250;
   EagleThread* threads[NTHREADS];
   void* rvals[NTHREADS];
   for (int i = 0 ; i < NTHREADS ; ++i) {
      threads[i] = a5sys->CreateThread("test_thread" , test_thread , 0);
      threads[i]->Start();
      EAGLE_ASSERT(threads[i]->Running());
   }
   a5sys->Rest(3);
   for (int i = 0 ; i < NTHREADS ; ++i) {
      threads[i]->SignalToStop();
   }
   for (int i = 0 ; i < NTHREADS ; ++i) {
      rvals[i] = threads[i]->Join();
   }
   for (int i = 0 ; i < NTHREADS ; ++i) {
      EagleInfo() << StringPrintF("Thread %d returned %ld" , threads[i]->GetEagleId() , (long)rvals[i]) << std::endl;
   }

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

