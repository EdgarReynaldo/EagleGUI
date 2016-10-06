



#include "TestRegistry.hpp"




Test::Test(string name_str , MAINFUNC pmain , string describe_str) :
      name(name_str),
      mainfunc(pmain),
      description(describe_str)
{
   test_registry.RegisterTest(this);
}






TestRegistry::TestRegistry()
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
