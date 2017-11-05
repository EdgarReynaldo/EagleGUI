



#ifndef Sound_HPP
#define Sound_HPP



#include <string>



class Sound {
public :
   virtual ~Sound() {}
   virtual bool Load(std::string file)=0;
   virtual void Play()=0;
   virtual void Free()=0;
};



class SoundManager {
public :
   virtual ~SoundManager() {}
   virtual void SetupDefaultSoundEnvironment()=0;
   virtual Sound* CreateSound(std::string sound_file)=0;
   virtual void FreeSound(Sound* s)=0;
};




#endif // Sound_HPP



