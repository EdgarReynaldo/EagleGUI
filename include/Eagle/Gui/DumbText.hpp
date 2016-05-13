



#ifndef DumbText_HPP
#define DumbText_HPP

#include "Text.hpp"

class DumbText : public WidgetBase {

protected :
   HALIGNMENT halign;
   VALIGNMENT valign;
   EagleFont* text_font;
   string text;
   std::vector<std::string> lines;
   std::vector<Rectangle> lineareas;
   int nlines;
   int hpadding;
   int vpadding;
   int linespacing;
   int maxwidth;
   int totalheight;
   int textx;
   int texty;
   std::vector<int> widths_vector;

   
   
   virtual void RefreshTextPosition(int lineheight);

   virtual int PrivateHandleEvent(EagleEvent e);
   virtual int PrivateCheckInputs();
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);
   virtual int PrivateUpdate(double tsec);

   
public :
   DumbText();
   DumbText(string name);
   virtual ~DumbText(){}

   void DrawText(EagleGraphicsContext* win , int xpos , int ypos , EagleColor c);

   void SetWidgetArea(int x , int y , int w , int h , bool notify_layout = true);

   /// Shrink wrap : Changes area so it exactly fits the text
   void ShrinkWrap();

   
	/// Changes position and outer area!!!
	virtual void SetMarginsExpandFromInner(int left , int right , int top , int bottom);

	/// Make room in outer area for inner area first!!!
	virtual void SetMarginsContractFromOuter(int left , int right , int top , int bottom);



   virtual void SetupText(HALIGNMENT hal , VALIGNMENT val , int hpad , int vpad , int vspacing ,
                          std::string textstr , EagleFont* font);
   
   virtual void SetText(std::string textstr , EagleFont* font);

   virtual void SetFont(EagleFont* font);
   
   virtual void Realign(HALIGNMENT hal , VALIGNMENT val);
   
   virtual void Refresh();

   
   string GetText() {return text;}
   
   Rectangle TextArea() {return Rectangle(textx , texty , maxwidth , totalheight);}
   
   virtual std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const ;

};




#endif // DumbText_HPP

