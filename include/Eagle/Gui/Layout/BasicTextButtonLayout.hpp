
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
 * @file BasicTextButtonLayout.hpp
 * @brief This file holds the BasicTextButton class. A versatile widget button with text.
 * 
 * 
 * 
 */


#ifndef BasicTextButtonLayout_HPP
#define BasicTextButtonLayout_HPP


#include "Eagle/Gui/Layout/Layout.hpp"
#include "Eagle/Gui/Button/BasicButton.hpp"
#include "Eagle/Gui/Text/BasicText.hpp"

/**! @class BasicTextButton
 * @brief This is the BasicTextButton class. A versatile widget button with text.
 *
 *
 */

class BasicTextButton : public LayoutBase {
protected :
   BasicButton basic_button;
   BasicText basic_text;
   
   BasicButton* our_button;/// slot 0
   BasicText*   our_text;/// slot 1

   
public :   
   
   BasicTextButton();
   
   /// WidgetBase
   
   void SetZOrder(int zpriority) override;///< Set the z priority. Larger values are closer to the screen and appear above

   /// LayoutBase
   virtual Rectangle RequestWidgetArea(int widget_slot , int newx , int newy , int newwidth , int newheight);

   virtual void Resize(unsigned int nsize);///< No need to call. Size is always 2.




   /// Members
   void SetOurButton(BasicButton* button);
   void SetOurText(BasicText* text);


   BasicButton* GetOurButton();
   BasicText* GetOurText();
};



#endif // BasicTextButtonLayout_HPP



