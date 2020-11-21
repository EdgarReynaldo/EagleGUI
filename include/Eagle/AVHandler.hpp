
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
 * @file AVHandler.hpp
 * @brief WIP interface for audio classes in Eagle
 * 
 * TODO : Implement me
 */

#ifndef AVHandler_HPP
#define AVHandler_HPP



#include "Eagle/Object.hpp"
#include "Eagle/Events.hpp"


/**! @class EagleAudio
 *   @brief A simple class to work with audio objects
 */

class EagleAudio : public EagleObject {
   
   
   virtual bool Play()=0;
   virtual void Stop()=0;
};


/**! @class EagleVideo
 *   @brief A simple class to work with video objects
 */

class EagleVideo : public EagleObject {
   
   
   
};




/**! @class EagleAVHandler
 *   @brief A simple class to handle audio and video objects
 */

class EagleAVHandler : public EagleEventSource {
   
   
   

};



#endif // AVHandler_HPP



