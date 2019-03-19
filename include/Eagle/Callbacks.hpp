
/**
 *
 *     _______       ___       ____      __       _______
 *    /\  ____\    /|   \     /  __\    /\ \     /\  ____\
 *    \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_
 *     \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\
 *      \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_
 *       \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *        \/______/|/_/  \/_/  \_\_____/    \/_____/ \/______/
 *
 *
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2019+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */



#ifndef Callbacks_HPP
#define Callbacks_HPP

/**

class CallerBase {
public :
   virtual int operator()=0;
};



template <class TYPE>
class MethodCaller : public CallerBase {
   TYPE* pcallobj;
   
   typedef int(TYPE::*MCALLBACK)();
   
   MCALLBACK mcallback;
   
public :
   
   MethodCaller(TYPE* t , MCALLBACK imethod);
   MethodCaller(TYPE* t , MCALLBACK imethod) :
         pcallobj(t),
         mcallback(imethod)
   {}
   
   virtual int operator();
int operator() {
   return (pcallobj->*mcallback)();
}
   
   
};
//*/

#endif // Callbacks_HPP

