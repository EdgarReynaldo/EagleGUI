
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

#include "Eagle/File.hpp"



class SoundSample {
public :
   SoundSample();
   virtual ~SoundSample();

   virtual bool LoadFromFile(FilePath fp)=0;
   virtual void Free()=0;
};



class SoundInstance {
public :
   SoundSample* parent;

   SoundInstance(SoundSample* parent_sample);
   virtual ~SoundInstance();
   
   SoundSample* Parent() {return parent;}
};



class SoundStream {
public :
   SoundStream();
   virtual ~SoundStream();

   virtual bool LoadFromFile(FilePath fp , size_t nbuffers = 3 , size_t buf_sample_count = 32768)=0;
   virtual void Free()=0;
};






#endif // Sound_HPP






