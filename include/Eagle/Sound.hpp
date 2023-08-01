
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
 * @file Sound.hpp
 * @brief The interface for all sounds in Eagle
 * 
 * TODO : Implement and stuff
 */

#ifndef Sound_HPP
#define Sound_HPP

#include <string>




class SoundMan {

};



class Sound {
public :
   virtual ~Sound() {}
   virtual bool Load(std::string file)=0;
   virtual void Play()=0;
   virtual void Pause()=0;
   virtual void Stop()=0;
   
};


class SoundSample : public Sound {
protected:
   
public :
   
};


class SoundBuffer {
protected:
   
public :
   

};



class SoundStream : public Sound {
protected:
   
public :
   
};

#endif // Sound_HPP



