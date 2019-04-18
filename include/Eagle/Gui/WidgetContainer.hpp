
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
 * @file WidgetContainer.hpp
 * @brief Simple container for widgets, using shared pointers
 * 
 */



#ifndef WidgetContainer_HPP
#define WidgetContainer_HPP



#include "Eagle/Object.hpp"
#include "Eagle/SharedMemory.hpp"

#include <map>



class WidgetBase;


typedef SHAREDOBJECT<WidgetBase> SHAREDWIDGET;


/*! \brief Simple class to store widget references */

class WIDGETCONTAINER {
public :
   typedef std::map<EAGLE_ID , SHAREDWIDGET > COMPONENT_MAP;
   typedef COMPONENT_MAP::iterator CMIT;
protected :
   
   COMPONENT_MAP cmap;
   
public :   
   
   WIDGETCONTAINER();///< Create an empty container
   ~WIDGETCONTAINER();
   
   EAGLE_ID Register(SHAREDWIDGET sw);///< Register this shared widget with the container
   void     Remove(EAGLE_ID id);      ///< Remove the reference to this widget
   void     Clear();                  ///< Empty the container

   SHAREDWIDGET FindById(EAGLE_ID id);///< Get a shared pointer to the specified object
   
   SHAREDWIDGET operator[] (EAGLE_ID id) {return FindById(id);}///< Shortcut to FindById
};


typedef WIDGETCONTAINER WidgetContainer;



#endif // WidgetContainer_HPP



