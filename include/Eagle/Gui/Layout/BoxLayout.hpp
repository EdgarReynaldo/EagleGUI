
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
 * @file BoxLayout.hpp
 * @brief Simple box layouts for Eagle
 */

 
 
#ifndef BoxLayout_HPP
#define BoxLayout_HPP


#include "Eagle/Gui/Layout/Layout.hpp"




/**! @enum BOX_SPACE_RULES
 *   @brief How empty space is used in a box layout
 */

enum BOX_SPACE_RULES {
   BOX_ALIGN_ONLY    = 0,///< Left over space is unused
   BOX_EXPAND        = 1,///< Left over space is given completely to widgets
   BOX_SPACE_BETWEEN = 2,///< Left over space is split up between each widget, pushing them out from the middle
   BOX_SPACE_EVEN    = 3 ///< Left over space is split up evenly between each widget on its outer edges
};



class BoxLayout : public LayoutBase {
   
protected :
   bool overflow;/// When the children are too big for the container this becomes true
   BOX_SPACE_RULES box_rules;
   std::vector<Rectangle> areas;
   
   virtual void RecalcFlow()=0;
   
   
public :
   BoxLayout(std::string classname = "BoxLayout" , std::string objname = "Nemo");
   
   virtual Rectangle RequestWidgetArea(int widget_slot , int newx , int newy , int newwidth , int newheight) override;

   virtual void Resize(unsigned int nsize) override;

   virtual void PlaceWidget(WidgetBase* w , int slot);
   virtual int AddWidget(WidgetBase* w);
   
   
   bool OverflowWarning() {return overflow;}///< Returns overflow state
};



class HBoxLayout : public BoxLayout {
   
protected :
   virtual void RecalcFlow() override;
public :
   
   
   
   
   
   
   
   
   
   
   
};













#endif // BoxLayout_HPP




 
 
 
