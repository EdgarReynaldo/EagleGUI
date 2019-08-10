
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



/**! @class PopupWindow
 *   @brief Makes it easier to create and handle popup windows
 
A popup needs a window, a widget handler, and widgets
 
 */

class PopupWindow : public WidgetHandler {

protected :
   EagleSystem* our_system;
   EagleGraphicsContext* our_window;

public :
   
//   PopupWindow(int x , int y , int width , int height , int flags , std::string system = "Any");
   PopupWindow(int x , int y , int width , int height , int flags , std::string system = "Any") :
         WidgetHandler(0 , "PopupHandler"),
         our_system(0),
         our_window(0)
   {
      our_system = Eagle::EagleLibrary::System(system);
      if (!our_system) {
         throw EagleException(StringPrintF("Failed to get system '%s' from Eagle.\n" , system.c_str()));
      }
      our_window = our_system->CreatePopupWindow("Popup" , width , height , flags);
      if (!our_window) {
         std::string flagstr = PrintDisplayFlags(flags);
         throw EagleException(StringPrintF("Failed to create %d x %d popup window with flags %s\n",
                                           width , height , flagstr.c_str()));
      }
      
      our_window->SetWindowPosition(x , y);
      SetupBuffer(width , height , our_window);
      SetWidgetArea(Rectangle(0 , 0 , width , height) , false);
   }
   
   void Hide();
   void Show();
   
   
   EagleGraphicsContext* OurWindow() {return our_window;}
};


SHAREDOBJECT<PopupWindow> CreatePopupTextWindow(int x , int y , int flags , std::string message , EagleFont* font);


