



#ifndef NamedParameters_HPP
#define NamedParameters_HPP


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
