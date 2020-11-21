
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
 * @file MemberFunctors.hpp
 * @brief The interface for working with functors and callbacks
 *
 *    
 *    TODO : Questionable usefulness. Revisit some day.
 *    CREDITS : These class method functors donated graciously by RickyLee from allegro.cc, who also got these
 *    CREDITS : from someone else on allegro.cc. All hail www.allegro.cc.
 *
 *    Rewritten by Edgar Reynaldo for lousy stylistic reasons.
 */

#ifndef EagleMemberFunctors_HPP
#define EagleMemberFunctors_HPP



#include "Container.hpp"



/// Abstract base class for a functor with two parameters
template <class U , class V>
class Functor2 {
public :
   virtual void Call(U,V)=0;
   
   virtual bool Equivalent(Functor2* functor2)=0;
   
};



/// Derivations of the functor base with 2 parameters
/// allowing you to specialize the function return type
/// and two parameter types to pass to it
template <class T , class U , class V >
class MemberFunctor2 : public Functor2<U,V> {
private :
   T* obj;
//   T::(*func2uv)(U , V);
   void (T::* func2uv)(U , V);

public :
   MemberFunctor2(T* object , void (T::*function2uv)(U , V)) : obj(object) , func2uv(function2uv) {}
   
   
   void Call(U u, V v) {
      (*obj.*func2uv)(u,v);
   }

   bool Equivalent(Functor2<U,V>* functor2) {
      MemberFunctor2* f = dynamic_cast<MemberFunctor2*>(functor2);
      if (f && (this->GetObj() == f->GetObj()) && (this->GetFunc() == f->GetFunc())) {
         return true;
      }
      return false;
   }
   
   T* GetObj() {return obj;}

   void (T::*GetFunc())(U,V) {return func2uv;}
   
};



/// Some one to keep track of a call list and call them all
/// upon command
template <class U , class V>
class Caller2 {

private :
   PointerManager< Functor2<U,V> > callbacks;

public :
   Caller2() : callbacks(false) {}
   ~Caller2() {
      callbacks.RemoveAll();
   }
   
   void Call(U u , V v) {
      for (unsigned int i = 0 ; i < callbacks.size() ; ++i) {
         callbacks[i]->Call(u , v);
      }
   }
   
   void Add(Functor2<U,V>* callback) {callbacks.Add(callback);}

   void Remove(Functor2<U,V>* callback) {
      for (int i = 0 ; i < callbacks.size() ; ++i) {
         if (callbacks[i]->Equivalent(callback)) {
            callbacks.Remove(callback);
            break;
         }
      }
   }
   
};

/**
Caller2<WidgetBase* w , EagleEvent e> caller2;

class MyClass {
   
   void OnEvent(WidgetBase* w , EagleEvent e);
}

MyClass obj;

MemberFunctor<MyClass , WidgetBase* w , EagleEvent e> memfunc(&obj , MyClass::OnEvent);

caller2.Add(&memfunc);

caller2.Call(&this_widget , widget_event);// Calls OnEvent of the obj object.
*/


#endif // EagleMemberFunctors_HPP



