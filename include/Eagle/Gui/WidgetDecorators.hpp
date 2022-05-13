



#ifndef WidgetDecorators_HPP
#define WidgetDecorators_HPP



class EagleGraphicsContext;
class WidgetBase;
class DecoratorBase;

#include "Eagle/SharedMemory.hpp"

typedef SHAREDOBJECT<DecoratorBase> SHAREDDECORATOR;



class DecoratorBase {
protected :
   SHAREDDECORATOR next;///< For linking decorations together. Basic linked list. next == null = EOF
   
   void DecorateNext(void* object , EagleGraphicsContext* win , int xpos , int ypos);

public :
   DecoratorBase(SHAREDDECORATOR d = HeapObject((DecoratorBase*)0));
   virtual ~DecoratorBase() {};

   virtual void Decorate(void* object , EagleGraphicsContext* win , int xpos , int ypos)=0;
};


class WidgetDecorator : public DecoratorBase {
   
public :
   
   WidgetDecorator(SHAREDDECORATOR d = HeapObject((DecoratorBase*)0));
   virtual ~WidgetDecorator() {}
   
   virtual void Decorate(void* object , EagleGraphicsContext* win , int xpos , int ypos);

   virtual void DecorateWidget(WidgetBase* widget , EagleGraphicsContext* win , int xpos , int ypos)=0;
   
//   virtual Rectangle GetOurArea();
};


class DropShadowDecorator : public WidgetDecorator {
   
public :
   
   DropShadowDecorator(SHAREDDECORATOR d = HeapObject((DecoratorBase*)0));
   virtual ~DropShadowDecorator() {}
   
   virtual void DecorateWidget(WidgetBase* widget , EagleGraphicsContext* win , int xpos , int ypos);

};



#endif // WidgetDecorators_HPP
