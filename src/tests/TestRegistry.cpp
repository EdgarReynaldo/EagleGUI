



#include "TestRegistry.hpp"




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
