


#ifndef GuiTestMain_HPP
#define GuiTestMain_HPP

#include "TestRegistry.hpp"

int GuiTestMain(int argc, char** argv , TestRunner* test_runner);
int GuiTestMain2(int argc, char** argv , TestRunner* test_runner);
int GuiTestMain3(int argc, char** argv , TestRunner* test_runner);

DECLARE_TEST(GuiTestMain);
DECLARE_TEST(GuiTestMain2);
DECLARE_TEST(GuiTestMain3);


#endif // GuiTestMain_HPP

