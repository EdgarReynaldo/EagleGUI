
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



#ifndef EagleWindowManager_HPP
#define EagleWindowManager_HPP


#include "Eagle/Object.hpp"
#include "Eagle/Events.hpp"


#include <map>



class EagleSystem;
class EagleGraphicsContext;
class EagleThread;
class EagleMutex;



class EagleWindowManager : public EagleObject , public EagleEventSource {

protected :
   EagleSystem*  parent_system;

   EagleThread*  manager_thread;
   EagleMutex*   manager_mutex;

   typedef std::map<int , EagleGraphicsContext*> WINMAP;
   typedef WINMAP::iterator WMIT;

   WINMAP window_map;

   int window_count;

   EagleGraphicsContext* active_window;

   EagleThread* our_thread;


   void SwitchIn(EagleGraphicsContext* window);
   void SwitchOut(EagleGraphicsContext* window);





protected :
   virtual EagleGraphicsContext* PrivateCreateWindow(std::string objname , int width , int height , int flags)=0;

   virtual int PrivateGiveWindowFocus(int window_eid)=0;

public :
   EagleWindowManager(EagleSystem* sys , std::string objclass = "EagleWindowManager" , std::string objname = "Nemo");

   virtual ~EagleWindowManager();



   void CloseWindows();

   EagleGraphicsContext* CreateWindow(std::string objname , int width , int height , int flags);
   void                  DestroyWindow(int window_eid);

   EagleGraphicsContext* GetActiveWindow();

   int GiveWindowFocus(int window_eid);/// returns window with focus, or -1 on failure

   int WindowCount() {return window_count;}


///   virtual void AcknowledgeDrawingHalt(EagleGraphicsContext* window)=0;




};



#endif // EagleWindowManager_HPP

