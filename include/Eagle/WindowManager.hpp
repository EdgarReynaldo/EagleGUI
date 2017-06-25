





#ifndef EagleWindowManager_HPP
#define EagleWindowManager_HPP



#include <map>



class EagleSystem;
class EagleGraphicsContext;
class EagleThread;
class EagleMutex;



class EagleWindowManager {

protected :
   EagleSystem*  parent_system;

   EagleThread*  manager_thread;
   EagleMutex*   manager_mutex;

   typedef std::map<int , EagleGraphicsContext*> WINMAP;
   typedef WINMAP::iterator WMIT;

   WINMAP window_map;

   int window_count;

   EagleGraphicsContext* active_window;




   void SwitchIn(EagleGraphicsContext* window);
   void SwitchOut(EagleGraphicsContext* window);





protected :
   virtual EagleGraphicsContext* PrivateCreateWindow(int width , int height , int flags)=0;

   virtual int PrivateGiveWindowFocus(int window_eid)=0;

public :
   EagleWindowManager(EagleSystem* sys);

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

