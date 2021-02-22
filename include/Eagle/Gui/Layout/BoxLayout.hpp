
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


#include "Eagle/Gui/Layout.hpp"


/**! @enum BOX_SIZE_RULES
 *   @brief How empty space is used in a box layout
 */

enum BOX_SIZE_RULES {
   BOX_ALIGN_ONLY    = 0,///< Left over space is unused
   BOX_EXPAND        = 1,///< Left over space is given completely to widgets
   BOX_SPACE_BETWEEN = 2,///< Left over space is split up between each widget, pushing them out from the middle
   BOX_SPACE_EVEN    = 3 ///< Left over space is split up evenly between each widget on its outer edges
};



/**! @enum FLOW_ANCHOR_POINT
 *   @brief Where to anchor the flow from. The opposite corner is where it flows to
 */
enum FLOW_ANCHOR_POINT {
	FLOW_ANCHOR_NW = 0,///< Anchor the flow in the NW corner
	FLOW_ANCHOR_NE = 1,///< Anchor the flow in the NE corner
	FLOW_ANCHOR_SE = 2,///< Anchor the flow in the SE corner
	FLOW_ANCHOR_SW = 3 ///< Anchor the flow in the SW corner
};



/**! @enum FLOW_FAVORED_DIRECTION
 *   @brief Which direction to favor when adding widgets to this layout
 */
enum FLOW_FAVORED_DIRECTION {
	FLOW_FAVOR_VERTICAL   = 0,///< Favor vertical flow
	FLOW_FAVOR_HORIZONTAL = 1 ///< Favor horizontal flow
};



class BoxLayout : public LayoutBase {
   
protected :
   bool overflow;/// When the children are too big for the container this becomes true
   BOX_SIZE_RULES box_rules;
   std::vector<Rectangle> areas;
   
   virtual void RecalcFlow()=0;
   
   
public :
   BoxLayout(std::string classname = "BoxLayout" , std::string objname = "Nemo");
   BoxLayout(std::string classname , std::string objname) :
         LayoutBase(classname , objname),
         overflow(false),
         box_rules(BOX_ALIGN),
         areas()
   {
      LayoutBase::SetAlignment(HALIGN_CENTER , VALIGN_CENTER);
   }
   
   virtual Rectangle RequestWidgetArea(int widget_slot , int newx , int newy , int newwidth , int newheight) override;
Rectangle RequestWidgetArea(int widget_slot , int newx , int newy , int newwidth , int newheight) {
   Rectangle r = areas[widget_slot];
}

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




 
 
 
