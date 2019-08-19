
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
 * @file PopupWindow.hpp
 * @brief A templated widget class for making popup windows
 */

#include "Eagle/SharedMemory.hpp"
#include "Eagle/Gui/WidgetHandler.hpp"
#include "Eagle/Lib.hpp"

#include "Eagle/StringWork.hpp"
#include "Eagle/Gui/TextWidgets.hpp"


/**! @class PopupWindow
 *   @brief Makes it easier to create and handle popup windows
 
A popup needs a window, a widget handler, and widgets
 
 */

class PopupWindow : public WidgetHandler {

protected :
   EagleSystem* our_system;
   EagleGraphicsContext* our_window;

public :
   
   PopupWindow(std::string system = "Any" , std::string window_name = "Nemo");
   
   virtual ~PopupWindow();
   
   virtual void FreePopupWindow();
   
   virtual void CreatePopupWindow(int sx , int sy , int width , int height , int flags);
      
   void Display();
   void Hide();
   void Show();
   
   
   EagleGraphicsContext* OurWindow() {return our_window;}
};

class PopupText : public PopupWindow {
   BasicText text;
   
public :
   PopupText(int sx , int sy , int flags , std::string message , EagleFont* font);
};


