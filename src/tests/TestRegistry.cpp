



#include "TestRegistry.hpp"
#include "Eagle/StringWork.hpp"



TestRunner::TestRunner(EagleGraphicsContext* window , MAINFUNC mainfunc) :
      main_window(window),
      our_thread(0),
      argc(0),
      argv(0),
      return_value(0),
      error_message(""),
      main_function(mainfunc)
{
   our_thread = sys->CreateThread("TestRunnerThread" , TestRunnerThread , this);
   if (!our_thread) {
      throw EagleException("TestRunner::TestRunner : Failed to create TestRunnerThread process.");
   }
   our_thread->SetName(StringPrintF("TestRunner Thread(EID = %d)" , our_thread->GetEagleId()));
   our_thread->Start();
}



TestRunner::~TestRunner() {
   our_thread->SignalToStop();
   our_thread->Join();
   sys->FreeThread(our_thread);
}



bool TestRunner::ShouldStop() {
   if (our_thread) {
      return our_thread->ShouldStop();
   }
   return true;
}



bool TestRunner::Running() {
   if (our_thread) {
      return our_thread->Running();
   }
   return false;
}



void* TestRunnerThread(EagleThread* thread , void* test_runner) {
   
   (void)thread;
   
   TestRunner* runner = (TestRunner*)test_runner;
   
   MAINFUNC main = runner->main_function;
   
   try {
      runner->return_value = main(runner->argc , runner->argv , runner);
   }
   catch (EagleException exc) {
      runner->return_value = -1;
      runner->error_message = exc.what();
   }
   return test_runner;
}



EagleSystem* sys = 0;


///TestRegistry test_registry;
TestRegistry& TestRegistry::GetRegistryInstance() {
   static TestRegistry tr;
   return tr;
}



Test::Test(string name_str , MAINFUNC pmain , string describe_str) :
      name(name_str),
      mainfunc(pmain),
      description(describe_str)
{
   TestRegistry::GetRegistryInstance().RegisterTest(this);
}






TestRegistry::TestRegistry() :
      test_map()
{}



void TestRegistry::RegisterTest(Test* t) {
   EAGLE_ASSERT(t);
   test_map[t->Name()] = t;
}



std::vector<const Test*> TestRegistry::GetRegisteredTests() {
   std::vector<const Test*> tests;
   TMIT tmit = test_map.begin();
   while (tmit != test_map.end()) {
      tests.push_back(tmit->second);
      ++tmit;
   }
   return tests;
}



Test* TestRegistry::GetTest(std::string test_name) {
   TMIT it = test_map.find(test_name);
   if (it != test_map.end()) {
      return it->second;
   }
   return 0;
}






