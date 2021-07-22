
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
 * @file FlowLayout.hpp
 * @brief The interface for the flow layout
 */



#ifndef FlowLayout_HPP
#define FlowLayout_HPP


#include "Eagle/Gui/Layout/BoxLayout.hpp"
#include <vector>


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




class FlowLayout : public LayoutBase {

protected :;
	BOX_SPACE_RULES size_rules;
	FLOW_ANCHOR_POINT anchor_pt;
	FLOW_FAVORED_DIRECTION favored_direction;
   bool overflow;
   bool shrink_on_overflow;

   std::vector<Rectangle> rcsizes;
   std::vector<double> waspects;

   int rowcount;
   std::vector<int> colcount;
   std::vector<int> rowheights;
   std::vector<int> colwidths;
   std::vector<int> rowspace;
   int colspace;

   int defwidth;
   int defheight;
   
   
   
   int GetMaxColWidth();
   int GetTotalRowHeight();
   int GetColumn(int index);/// 1 based
   int GetRow(int index);/// 0 based
   int GetWidgetIndex(int row , int col);
   
   virtual void ReserveSlots(int nslots);

   virtual void OnAreaChanged();///< Override to react to changes in this widget's area

   void RecalcFlow();
   
public :
   FlowLayout(std::string classname = "FlowLayout" , std::string objname = "Nemo");
   virtual ~FlowLayout();

   
   
   virtual Rectangle RequestWidgetArea(int widget_slot , int newx , int newy , int newwidth , int newheight);

   void PlaceWidget(WidgetBase* w , int slot) override;
   int AddWidget(WidgetBase* w) override;
   void InsertWidget(WidgetBase* w , int slot_before) override;

   bool Overflow() {return overflow;}
   void ShrinkOnOverflow(bool shrink);
   
   void SetDefaultWidth(unsigned int w);
   void SetDefaultHeight(unsigned int h);
   void SetDefaultSize(unsigned int w , unsigned int h);

   virtual void SetAlignment(HALIGNMENT h_align , VALIGNMENT v_align) override;
   
   void SetFlowAnchor(FLOW_ANCHOR_POINT p);

   void SetFlowDirection(FLOW_FAVORED_DIRECTION d);
};





#endif // FlowLayout_HPP




