



#ifndef TestRegistry_HPP
#define TestRegistry_HPP

#include "Eagle/System.hpp"

#include <map>
#include <vector>
#include <string>


typedef int (*MAINFUNC)(int , char**);

#define DECLARE_TEST(mainfunc) \
   extern Test test_##mainfunc;
   
#define DEFINE_TEST(mainfunc , description) \
   Test test_##mainfunc(#mainfunc , mainfunc , description);


extern EagleSystem* sys;



class Test {

protected:
   string name;
   MAINFUNC mainfunc;   
   string description;
public :
   Test(string name_str , MAINFUNC pmain , string describe_str);
   
   string Name() const {return name;}
   MAINFUNC MainFunc() const {return mainfunc;}
   string Describe() const {return description;}
};



class TestRegistry {
   
protected :

   typedef std::map<std::string , Test*> TESTMAP;
   typedef TESTMAP::iterator TMIT;
   
   TESTMAP test_map;

public :

   TestRegistry();
   void RegisterTest(Test* t);

   std::vector<const Test*> GetRegisteredTests();

///   int Run(const char* test_branch , int argc , char** argv);
   int Run(const char* test_branch , int argc , char** argv) {
      TMIT it = test_map.find(test_branch);
      if (it == test_map.end()) {
         return -1;
      }
      MAINFUNC mainfunc = test_map[test_branch]->MainFunc();
      return (*mainfunc)(argc , argv);
   }
   
};

extern TestRegistry test_registry;


#endif // TestRegistry_HPP

