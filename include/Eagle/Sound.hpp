
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
 *    Copyright 2009-2023+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file Sound.hpp
 * @brief The interface for all sounds in Eagle
 * 
 * Base classes for audio drivers to use
 */

#ifndef Sound_HPP
#define Sound_HPP

#include <string>




class SoundManager;



class Sound {///< Base class for all sounds
protected :
   SoundManager* soundman;
   
public :
   SoundManager(SoundManager* sndman) : soundman(sndman) {}
   
   virtual ~Sound() {}
   
   virtual void Play()=0;
   virtual void Pause()=0;
   virtual void Stop()=0;
   ///< Pan, gain, seek, etc
};



class SoundInstance : public Sound {
protected:
   
public :
   SoundInstance(SoundManager* sndman) : Sound(sndman) {}
};



class SoundStream : public Sound {
protected:
   
public :
   SoundStream(SoundManager* sndman) : Sound(sndman) {}
   
};






#endif // Sound_HPP






