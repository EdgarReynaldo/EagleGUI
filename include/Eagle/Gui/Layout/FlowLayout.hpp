
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
#include <string>




/**! @enum FLOW_FAVORED_DIRECTION
 *   @brief Which direction to favor when adding widgets to this layout
 */
enum FLOW_FAVORED_DIRECTION {
	FLOW_FAVOR_HORIZONTAL = 0,///< Favor horizontal flow
	FLOW_FAVOR_VERTICAL   = 1 ///< Favor vertical flow
};

std::string PrintFlowFavoredDirection(FLOW_FAVORED_DIRECTION d);


class FlowLayout : public LayoutBase {

protected :
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
   
   virtual void RepositionAllChildren() override;
   virtual void RepositionChild(int slot) override;

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
   
   void SetBoxSpacing(BOX_SPACE_RULES r);
   
   virtual std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const;
   
};





#endif // FlowLayout_HPP




