





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
   virtual EagleGraphicsContext* PrivateCreateWindow(int width , int height , int flags)=0;

   virtual int PrivateGiveWindowFocus(int window_eid)=0;

public :
   EagleWindowManager(EagleSystem* sys , std::string objclass = "EagleWindowManager" , std::string objname = "Nemo");

   virtual ~EagleWindowManager();



   void CloseWindows();

   EagleGraphicsContext* CreateWindow(int width , int height , int flags);
   void                  DestroyWindow(int window_eid);

   EagleGraphicsContext* GetActiveWindow();

   int GiveWindowFocus(int window_eid);/// returns window with focus, or -1 on failure

   int WindowCount() {return window_count;}


///   virtual void AcknowledgeDrawingHalt(EagleGraphicsContext* window)=0;




};



#endif // EagleWindowManager_HPP

