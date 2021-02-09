
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
 * @file 
 * @brief
 * 
 * 
 * 
 */

#ifndef EditText_HPP
#define EditText_HPP

#include "Eagle/Gui/Text/SelectText.hpp"



/**! @class EditText
 *   @brief A simple multi line text editor widget
 *   TODO : DOCS
 */

class EditText : public SelectText {
   
protected :
   
   bool insert_mode;
   unsigned int pixscroll;
   unsigned int NTABSPACES;
   
   
   
   /// WIDGETBASE
   virtual int PrivateHandleEvent(EagleEvent e);
   virtual int PrivateCheckInputs();
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);
   virtual int PrivateUpdate(double tsec);

   virtual void OnAreaChanged();
   virtual void OnFlagChanged(WIDGET_FLAGS f , bool on);
   
   /// SelectText
   virtual void DrawCaret(EagleGraphicsContext* win , int xpos , int ypos);

   /// EditText
   void DrawOverwriteCaret(EagleGraphicsContext* win , int xpos , int ypos);
   
   void Insert(std::string instr , int caretLine , int caretPos);

   void DeleteSelection(int selLineStart , int selLineStop , int selLeft , int selRight);
   void Overwrite(std::string instr , int selLineStart , int selLineStop , int selLeft , int selRight);
   
public :
   
   EditText(std::string classname = "EditText" , std::string objname = "Nemo");
   
   bool Insert();
   bool Overwrite();
   
   std::string GetText() {return text;}
   void SetTabSpacing(unsigned int ntabspaces);
};



class IntEditText : public EditText {
   
//   virtual int PrivateHandleEvent(EagleEvent e);
   virtual int PrivateHandleEvent(EagleEvent e) {
      if (e.type == EAGLE_EVENT_KEY_CHAR && !isdigit(e.keyboard.unicode)) {return DIALOG_OKAY;}
      return EditText::PrivateHandleEvent(e);
   }
   
public :
   IntEditText(std::string classname = "IntEditText" , std::string objname = "Nemo") :
         EditText(classname , objname)
   {}
};



#endif // EditText_HPP

