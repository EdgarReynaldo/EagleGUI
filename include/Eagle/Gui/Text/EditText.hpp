


#ifndef EditText_HPP
#define EditText_HPP



#include "Eagle/Gui/Text/SelectText.hpp"



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




#endif // EditText_HPP

