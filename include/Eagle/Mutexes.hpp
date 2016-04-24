
/*
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
 *    EAGLE
 *    Edgar's Agile Gui Library and Extensions
 *
 *    Copyright 2009-2014+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */

#ifndef EagleMutex_HPP
#define EagleMutex_HPP



/// Abstract Base class

class EagleMutex {

protected :
   bool recursive;
   
public :
   
   EagleMutex() : recursive(false) {}
   virtual ~EagleMutex() {}
   
   virtual bool Create(bool multi_lockable)=0;
   virtual void Destroy()=0;
   virtual bool Valid()=0;
   
   virtual void Lock()=0;
   virtual void Unlock()=0;
   
};





#endif // EagleMutex_HPP





