
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
 * @brief The interface for the flow box layout
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


/**! @fn PrintFlowFavoredDirection
 *   @brief Turns a @ref FLOW_FAVORED_DIRECTION into a text string
 */

std::string PrintFlowFavoredDirection(FLOW_FAVORED_DIRECTION d);



/**! @class FlowBoxLayout
 *   @brief For laying out widgets horizontally or vertically, wrapping to the next row as necessary
 */

class FlowLayout : public BoxLayout {

protected :
	FLOW_FAVORED_DIRECTION favored_direction;
	BOX_SPACE_RULES minorspacing;/// TODO : Implement

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
   void AdjustSpacing();
   void MirrorFlow();
   

public :
   FlowLayout(std::string classname = "FlowLayout" , std::string objname = "Nemo");
   virtual ~FlowLayout();

   virtual Rectangle RequestWidgetArea(int widget_slot , int newx , int newy , int newwidth , int newheight) override;
   
   void SetDefaultWidth(unsigned int w);///< Set the default width for widgets added to this layout. Only used if w->PreferredWidth() is 0
   void SetDefaultHeight(unsigned int h);///< Set the default height for widgets added to this layout. Only used if w->PreferredHeight() is 0
   void SetDefaultSize(unsigned int w , unsigned int h);/// Set the default size for widgets added to this layout. See @ref SetDefaultWidth and @ref SetDefaultHeight

   
   virtual void SetAnchorPosition(BOX_ANCHOR_POINT p) override;///< Set the anchor position for this widget. May be any corner of the @ref BOX_ANCHOR_POINT FBox values

   void SetFlowDirection(FLOW_FAVORED_DIRECTION d);///< Whether to flow horizontally or vertically. See @ref FLOW_FAVORED_DIRECTION for values.
   
   virtual int WidthLeft() override;///< Return space left on this row horizontally
   virtual int HeightLeft() override;///< Return space left including the last row vertically
   
   virtual bool WidgetWouldOverflowLayout(WidgetBase* w) override;///< Returns true if the layout would overflow by adding w
   
   virtual std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const override;
   
};





#endif // FlowLayout_HPP




