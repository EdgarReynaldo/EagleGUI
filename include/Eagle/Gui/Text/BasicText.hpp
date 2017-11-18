



#ifndef DumbText_HPP
#define DumbText_HPP

#include "Eagle/Gui/Text/GlobalText.hpp"

#include "Eagle/Gui/WidgetBase.hpp"



class BasicText : public WidgetBase {

protected :
   HALIGNMENT halign;
   VALIGNMENT valign;
   EagleFont* text_font;
   std::string text;
   std::vector<std::string> lines;
   std::vector<Rectangle> lineareas;
   int nlines;
   int hpadding;
   int vpadding;
   int linespacing;
   int maxwidth;
   int fontheight;
   int totalheight;
   int textx;
   int texty;
   std::vector<int> widths_vector;

   bool scale_to_fit;
   Transform scaling_transform;
   Transform inverse_scaling_transform;
   
   
   
   virtual void RefreshTextPosition();

   virtual int PrivateHandleEvent(EagleEvent e);
   virtual int PrivateCheckInputs();
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);
   virtual int PrivateUpdate(double tsec);

   
protected :
   BasicText(std::string objclass , std::string objname);

public :

   BasicText(std::string objname = "Nemo");
   virtual ~BasicText(){}

   void DrawText(EagleGraphicsContext* win , int xpos , int ypos , EagleColor c);

   void SetWidgetArea(int x , int y , int w , int h , bool notify_layout = true);

   /// Shrink wrap : Changes area so it exactly fits the text
   void ShrinkWrap();

   void ScaleToFit(bool scale);
   
   
	/// Changes position and outer area!!!
	virtual void SetMarginsExpandFromInner(int left , int right , int top , int bottom);

	/// Make room in outer area for inner area first!!!
	virtual void SetMarginsContractFromOuter(int left , int right , int top , int bottom);



   virtual void SetupText(HALIGNMENT hal , VALIGNMENT val , int hpad , int vpad , int vspacing ,
                          std::string textstr , EagleFont* font);
   
   virtual void SetText(std::string textstr , EagleFont* font);

   virtual void SetText(std::string textstr);
   virtual void SetFont(EagleFont* font);
   
   virtual void Realign(HALIGNMENT hal , VALIGNMENT val , int hpad = 0 , int vpad = 0);
   
   virtual void SetLineSpacing(int vspacing);
   
   virtual void Refresh();

   
   std::string GetText() {return text;}
   
   Rectangle TextArea() {return Rectangle(textx , texty , maxwidth , totalheight);}
   
   int NLines();

   Rectangle LineArea(int linenum);
   
   std::string LineString(int linenum);
   
   virtual std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const ;

};




#endif // DumbText_HPP

