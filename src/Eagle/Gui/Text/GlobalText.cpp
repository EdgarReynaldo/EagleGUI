
/**
 *
 *     _______       ___       ____      __       _______
 *    /\  ____\    /|   \     /  __\    /\ \     /\  ____\
 *    \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_
 *     \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\
 *      \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_
 *       \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *        \/______/|/_/  \/_/  \_\_____/    \/_____/ \/______/
 *
 *
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2019+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */

#include "Eagle/Platform.hpp"

#include "Eagle/Gui/Text/GlobalText.hpp"

#include "Eagle/Gui/WidgetBase.hpp"

#include "Eagle/Font.hpp"

#include "Eagle/StringWork.hpp"



const unsigned int TOPIC_TEXT_WIDGET = NextFreeTopicId();



unsigned int GetNextFreeTextMessageId() {
   static unsigned int i = 0;
   return i++;
}



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

