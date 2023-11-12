
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
#include "Eagle/Resources.hpp"



class EagleSoundSample : public ResourceBase {

   virtual bool LoadFromArgs(std::vector<std::string> args)=0;/*(void)args*/
public :
   EagleSoundSample();

   virtual ~EagleSoundSample();

   virtual void Free()=0;
};



class EagleSoundInstance {
public :
   EagleSoundSample* parent;

   EagleSoundInstance(EagleSoundSample* parent_sample);
   virtual ~EagleSoundInstance();
   
   EagleSoundSample* Parent() {return parent;}
};



class EagleSoundStream : public ResourceBase {

   virtual bool LoadFromArgs(std::vector<std::string> args)=0;/*size_t nbuffers = 3 , size_t buf_sample_count = 32768*/
public :
   EagleSoundStream();
   virtual ~EagleSoundStream();

   virtual void Free()=0;
};






#endif // Sound_HPP






