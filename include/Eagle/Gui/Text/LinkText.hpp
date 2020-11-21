
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
 *    Copyright 2009-2021+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file LinkText.hpp
 * @brief The interface for the link text class
 * 
 */

#ifndef LinkText_HPP
#define LinkText_HPP

#include "Eagle/Gui/Text/SelectText.hpp"



extern const int LINK_LAUNCHED;


///< Registers a widget message under TOPIC_TEXT_WIDGET with the value LINK_LAUNCHED
REGISTER_WIDGET_MESSAGE(TOPIC_TEXT_WIDGET , LINK_LAUNCHED);



/**! @class LinkText
 *   @brief A widget class that holds clickable text, to launch hyperlinks with
 *   @sa SelectText
 */

class LinkText : public SelectText {

protected :

   float link_height;///< Specifies the 
   
   /// WidgetBase overrides
   
   virtual int PrivateHandleEvent(EagleEvent e);
   virtual int PrivateCheckInputs();
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);
   virtual int PrivateUpdate(double tsec);

   virtual void OnFlagChanged(WIDGET_FLAGS f , bool on);

public :
   LinkText(std::string classname = "LinkText" , std::string name = "Nemo");
   
   virtual void SetFont(EagleFont* font);///< Override to set the font

   virtual void SetLineSpacing(int vspacing);///< Override to set the line spacing

   void LaunchLink();///< Launches the link, using a system call, be careful what you launch...
};



#endif // LinkText_HPP



