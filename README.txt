



/**
#defines used in building Eagle5
----------------------------

DEBUG :
 - turns on debugging (enables EAGLE_ASSERT() and EAGLE_DEBUG() statements)
LOG :
 - turns on debugging log (enables EagleLog())
EAGLE_ASSERT_EXCEPTION :
 - turns on assertion failure on exceptions, which allows you to get a backtrace because
 - the debugger will catch an intentional divide by zero and let you examine the stack.
 - Make sure to make good use of EAGLE_ASSERT

EAGLE_STATIC_LINK
 - links statically

Macros
--------------------------------
EAGLE_ASSERT(exp);
 - like assert, but with debugging info attached

EAGLE_DEBUG(exp)
 - exp will only execute when DEBUG is defined. Useful for keeping release code clean.
ASSERT_EXCEPTION();
 - Meant for causing a divide by zero crash. Useful for getting a backtrace.
 - Divides by zero when EAGLE_ASSERT_EXCEPTION is defined, or does nothing if not


Keywords used for source searches
---------------------------------
- CREDIT
- CREDITS
- TODO
- DESIGN

*/

