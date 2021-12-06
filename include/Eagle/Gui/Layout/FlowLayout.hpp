
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


class FlowLayout : public BoxLayout {

protected :
	FLOW_FAVORED_DIRECTION favored_direction;
	BOX_SPACE_RULES minorspacing;
   bool shrink_on_overflow;

   std::vector<double> waspects;

   int rowcount;
   std::vector<int> colcount;
   std::vector<int> rowheights;
   std::vector<int> colwidths;
   std::vector<int> rowspace;
   int colspace;

   int defwidth;
   int defheight;
   
   
   
   int GetMaxColWidth() const;
   int GetTotalRowHeight() const;
   int GetColumn(int index) const;/// 0 based
   int GetRow(int index) const;/// 0 based
   int GetWidgetIndex(int row , int col) const;
   
   virtual void ReserveSlots(int nslots);

   virtual void OnAreaChanged();///< Override to react to changes in this widget's area

   virtual void RepositionAllChildren() override;
   virtual void RepositionChild(int slot) override;

   void RecalcFlow();
   void RecalcFlowOld();
   void AdjustSpacing();
   void MirrorFlow();
   

public :
   FlowLayout(std::string classname = "FlowLayout" , std::string objname = "Nemo");
   virtual ~FlowLayout();

   virtual Rectangle RequestWidgetArea(int widget_slot , int newx , int newy , int newwidth , int newheight);

   void ShrinkOnOverflow(bool shrink);
   
   void SetDefaultWidth(unsigned int w);
   void SetDefaultHeight(unsigned int h);
   void SetDefaultSize(unsigned int w , unsigned int h);

   
   void SetAnchorPosition(BOX_ANCHOR_POINT p) override;

   void SetFlowDirection(FLOW_FAVORED_DIRECTION d);
   
   virtual int WidthLeft() override;
   virtual int HeightLeft() override;
   
   virtual bool WidgetWouldOverflowLayout(WidgetBase* w) override;
   
   virtual std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const;
   
};





#endif // FlowLayout_HPP




