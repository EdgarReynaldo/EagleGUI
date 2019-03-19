
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

