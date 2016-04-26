


#include "Eagle/Gui/Text.hpp"



void GetTextAttributes(std::string text , EagleFont* font , int line_spacing , int* pnlines ,
                       std::vector<std::string>* plines , std::vector<int>* pwidths_vector ,
                       int* pmaxwidth , int* ptotalheight) {

   EAGLE_ASSERT(font);
   EAGLE_ASSERT(font->Valid());
///   EAGLE_ASSERT(pnlines);
///   EAGLE_ASSERT(plines);
///   EAGLE_ASSERT(pwidths_vector);
   EAGLE_ASSERT(pmaxwidth);
   EAGLE_ASSERT(ptotalheight);

   /// calculate size of entire text
   if (plines) {*plines = SplitByNewLines(text);}
   if (pnlines) {
      if (plines) {
         *pnlines = (int)plines->size();
      }
   }
   if (*pnlines < 1) {
      return;
   }
   int lineheight = font->Height();
   if (pwidths_vector) {pwidths_vector->clear();}
   *pmaxwidth = 0;
   for (int i = 0 ; i < *pnlines ; ++i) {
      int w = font->Width((*plines)[i].c_str());
      if (pwidths_vector) {pwidths_vector->push_back(w);}
      if (w > *pmaxwidth) {*pmaxwidth = w;}
   }
   *ptotalheight = lineheight*(*pnlines) + line_spacing*(*pnlines-1);

}



void DumbText::DrawText(EagleGraphicsContext* win , int xpos , int ypos , EagleColor c) {
   EAGLE_ASSERT(win);
   EAGLE_ASSERT(win->Valid());
   
   if (text.size()) {
      EAGLE_ASSERT(text_font && text_font->Valid());

      int x = InnerArea().X() + xpos + hpadding;
      int y = InnerArea().Y() + ypos + vpadding;
      if (halign == HALIGN_CENTER) {
         x = InnerArea().CX() + xpos;
      }
      if (halign == HALIGN_RIGHT) {
         x = InnerArea().BRX() + xpos - hpadding;
      }
      if (valign == VALIGN_CENTER) {
         y = InnerArea().CY() + ypos;
      }
      if (valign == VALIGN_BOTTOM) {
         y = InnerArea().BRY() + ypos - vpadding;
      }
      win->DrawMultiLineTextString(text_font , text , x , y , c , linespacing , halign , valign);
   }
}



int DumbText::PrivateHandleEvent(EagleEvent e) {
   (void)e;
   return DIALOG_OKAY;
}



int DumbText::PrivateCheckInputs() {
   return DIALOG_OKAY;
}



void DumbText::PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos) {
   DrawText(win , xpos , ypos , WCols()[TXTCOL]);
}



int DumbText::PrivateUpdate(double tsec) {
   (void)tsec;
   return DIALOG_OKAY;
}



void DumbText::SetTextParameters(HALIGNMENT hal , VALIGNMENT val , int hpad , int vpad , int vspacing) {
   halign = hal;
   valign = val;
   hpadding = hpad;
   vpadding = vpad;
   linespacing = vspacing;
   SetBgRedrawFlag();
}



void DumbText::SetTextString(std::string str , EagleFont* font) {
   if (font) {
      text_font = font;
   }
   text = str;
   SetBgRedrawFlag();
}

