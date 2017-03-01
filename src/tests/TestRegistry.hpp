



#ifndef TestRegistry_HPP
#define TestRegistry_HPP

#include "Eagle/System.hpp"

#include <map>
#include <vector>
#include <string>


class TestRunner;

typedef int (*MAINFUNC)(int , char** , TestRunner*);



class TestRunner {

public :
   
   TestRunner(EagleGraphicsContext* window , MAINFUNC mainfunc);/* , int _argc , char** _argv */
   ~TestRunner();


   EagleGraphicsContext* main_window;
   EagleThread* our_thread;
   
   int argc;
   char** argv;
   
   int return_value;
   
   string error_message;
   
   int (*main_function) (int argc , char** argv , TestRunner* our_test_runner);
   
   bool ShouldStop();
   bool Running();
};

void* TestRunnerThread(EagleThread* thread , void* test_runner);

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

   TestRegistry();
public :

   static TestRegistry& GetRegistryInstance();

   void RegisterTest(Test* t);

   std::vector<const Test*> GetRegisteredTests();

   Test* GetTest(std::string test_name);
      
   int Size() {return test_map.size();}
   
};



#endif // TestRegistry_HPP

