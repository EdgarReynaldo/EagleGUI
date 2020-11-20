
/**
 *
 *         _______       ___       ____      __       _______
 *        /\  ____\    /|   \     /  __\    /\ \     /\  ____\
 *        \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_
 *         \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\
 *          \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_
 *           \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *            \/______/|/_/  \/_/  \_\_____/    \/_____/ \/______/
 *
 *
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2021+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file NamedParameters.hpp
 * @brief Experimental, for passing named parameters to functions...
 *
 * TODO : UNUSED : Abandon all hope ye who enter here?
 */

#ifndef NamedParameters_HPP
#define NamedParameters_HPP

#error UNUSED

/**
#include <utility>

class ParamBase {
   std::string p;/// Parameter string
public :
   ParamBase(std::string params) : p(params) {}
   virtual ~ParamBase() {}
   template <class T>
   virtual operator T ();
};


template <class T>
class Param : public ParamBase {
protected :
   T val;
public :
   operator T () {return val;}
};

class Parameter {
protected :
   std::shared_ptr<ParamBase> shared_pbase;
public :
   template <class T> operator T () {return (T)(*(shared_pbase.get()));}
};

std::pair<std::string , 

*/



#endif // NamedParameters_HPP



