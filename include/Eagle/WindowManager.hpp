
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
 *    Copyright 2009-2019+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file WindowManager.hpp
 * @brief The interface for managing windows in Eagle
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



/**! @class EagleWindowManager
 *   @brief The base class for handling windows in Eagle
 */

class EagleWindowManager : public EagleObject , public EagleEventSource {

protected :
   EagleSystem*  parent_system;///< Our system pointer

   EagleThread*  manager_thread;///< For running the window manager process
   EagleMutex*   manager_mutex;///< For synchronizing the window manager process

   typedef std::map<int , EagleGraphicsContext*> WINMAP;
   typedef WINMAP::iterator WMIT;

   WINMAP window_map;///< Map of our windows

   int window_count;///< Number of live windows

   EagleGraphicsContext* active_window;///< The active window

   EagleThread* our_thread;///< Our thread??? 


   void SwitchIn(EagleGraphicsContext* window);///< Switch a window in
   void SwitchOut(EagleGraphicsContext* window);///< Switch a window out

protected :
   /**! @fn PrivateCreateWindow <std::string , int , int , int>
    *   @brief Pure virtual function to create a new window
    */
   virtual EagleGraphicsContext* PrivateCreateWindow(std::string objname , int width , int height , int flags)=0;

   /**! @fn PrivateGiveWindowFocus <int>
    *   @brief Pure virtual function to give a window focus
    */
   virtual int PrivateGiveWindowFocus(int window_eid)=0;

public :
   /**! @fn EagleWindowManager <EagleSystem* , std::string , std::string>
    *   @brief Constructs a new window manager using the specified system, class, and object names
    */
   EagleWindowManager(EagleSystem* sys , std::string objclass = "EagleWindowManager" , std::string objname = "Nemo");

   virtual ~EagleWindowManager();///< Virtual destructor


   void CloseWindows();///< Close all windows

   /**! @fn CreateWindow <std::string , int , int , int>
    *   @brief Creates a new window with the specified attributes
    *   @param flags See @ref EAGLE_DISPLAY_FLAGS for possible values - combine using bitwise OR
    *   @retval The new window
    */
   EagleGraphicsContext* CreateWindow(std::string objname , int width , int height , int flags);

   /**! @fn DestroyWindow <int>
    *   @brief Destroys a window given by it's @ref EAGLE_ID - see @ref EagleObject::GetEagleId
    */
   void DestroyWindow(int window_eid);

   EagleGraphicsContext* GetActiveWindow();///< Get a pointer to the active window - may return NULL

   int GiveWindowFocus(int window_eid);///< Returns the new window with focus, or -1 on failure

   int WindowCount() {return window_count;}///< Get the window count for this manager
};




#endif // EagleWindowManager_HPP



