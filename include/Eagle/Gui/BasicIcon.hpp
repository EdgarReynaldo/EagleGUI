
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
 * @file BasicIcon.hpp
 * @brief A simple class for showing icons
 * 
 * BasicIcon allows you to add icon widgets to your gui.
 * 
 */



#ifndef BasicIcon_HPP
#define BasicIcon_HPP



#include "Eagle/Gui/WidgetBase.hpp"


/*!
 * \class BasicIcon
 *
 * BasicIcon is a simple class to display an image centered or stretched to fill its inner area.
 * 
 */

class BasicIcon : public WidgetBase {
   
   
protected :   
   
   SHAREDIMAGE icon_image;///< The image to display
   
   bool stretch_icon;///< Whether to stretch the icon to fill the area
   
   
   
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);
   
public :
   
   BasicIcon(std::string objclass = "BasicIcon" , std::string objname = "Nemo");
   
   virtual ~BasicIcon() {}
   
   void SetStretch(bool stretch);///< Whether to stretch the image to fill the area
   void SetImage(SHAREDIMAGE new_image);///< Set the icon image
   
   virtual std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const ;///< Stream output
   
};





#endif // BasicIcon_HPP







