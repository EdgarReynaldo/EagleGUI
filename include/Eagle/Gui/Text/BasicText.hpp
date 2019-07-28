
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
 * @file BasicText.hpp
 * @brief The interface for the BasicText widget
 *
 */

#ifndef BasicText_HPP
#define BasicText_HPP



#include "Eagle/Gui/Alignment.hpp"
#include "Eagle/Gui/Text/GlobalText.hpp"

#include "Eagle/Gui/WidgetBase.hpp"
#include "Eagle/Transforms.hpp"



/**! @class BasicText
 *   @brief A simple widget for drawing text and a base class for all other text widgets
 */

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
   
   
   
   virtual void RefreshTextPosition();///< Private function to refresh the text positioning and transforms


   /// WidgetBase overrides
   
   virtual int PrivateHandleEvent(EagleEvent e);
   virtual int PrivateCheckInputs();
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);
   virtual int PrivateUpdate(double tsec);

   virtual void OnAreaChanged();



   BasicText(std::string objclass , std::string objname);

public :

   BasicText(std::string objname = "Nemo");
   BasicText(EagleFont* font , std::string label ,
             HALIGNMENT hal = HALIGN_CENTER, VALIGNMENT val = VALIGN_CENTER , int hpad = 0, int vpad = 0 , int vspacing = 0);

   virtual ~BasicText(){}

   void DrawText(EagleGraphicsContext* win , int xpos , int ypos , EagleColor c);///< Draws this widget at the specified pos and color

   ///< Shrink wrap : Changes area so it exactly fits the text
   void ShrinkWrap();

   ///< Pass true to scale the text to fit the widget area, or false to center it unscaled
   void ScaleToFit(bool scale);
   
   ///< Sets up this text widget with the specified alignments, padding, vertical spacing, text, and font
   virtual void SetupText(HALIGNMENT hal , VALIGNMENT val , int hpad , int vpad , int vspacing ,
                          std::string textstr , EagleFont* font);
   
   ///< Changes the text and font in use
   virtual void SetText(std::string textstr , EagleFont* font);
   
   ///< Sets the text to display
   virtual void SetText(std::string textstr);
   
   ///< Sets the font in use
   virtual void SetFont(EagleFont* font);
   
   ///< Realigns the text using the specified alignments and optional padding
   virtual void Realign(HALIGNMENT hal , VALIGNMENT val , int hpad = 0 , int vpad = 0);
   
   ///< Sets the line spacing for widget with multiple lines of text
   virtual void SetLineSpacing(int vspacing);
   
   ///< Refreshes something or other....
   virtual void Refresh();

   ///< Retrieves the text in use
   std::string GetText() {return text;}
   
   ///< Retrieves the text area
   Rectangle TextArea() {return Rectangle(textx , texty , maxwidth , totalheight);}
   
   ///< Returns a count of lines in the text
   int NLines();

   ///< Returns the rectangular area of the specified line of the text
   Rectangle LineArea(int linenum);
   
   ///< Returns the text for the specified line
   std::string LineString(int linenum);
   
   ///< Describes this widget to a stream
   virtual std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const ;

};




#endif // BasicText_HPP



