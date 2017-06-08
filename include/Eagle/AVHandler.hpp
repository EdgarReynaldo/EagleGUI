



#ifndef AVHandler_HPP
#define AVHandler_HPP


#include "Eagle/Object.hpp"

#include "Eagle/Events.hpp"


class EagleAudio : public EagleObject {
   
   
   virtual bool Play()=0;
   virtual void Stop()=0;
};



class EagleVideo : public EagleObject {
   
   
   
};





class EagleAVHandler : public EagleEventSource {
   
   
   

};


#endif // AVHandler_HPP

