
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


/**! @enum BOX_ANCHOR_POINT
 *   @brief Where to anchor the flow from. The opposite corner is where it flows to
 */
enum BOX_ANCHOR_POINT {
   HBOX_ANCHOR_W = 0, ///< Anchor west side for hbox
   HBOX_ANCHOR_E = 1, ///< Anchor east side for hbox
   VBOX_ANCHOR_N = 2, ///< Anchor north side for vbox
   VBOX_ANCHOR_S = 3, ///< Anchor south side for vbox
	FBOX_ANCHOR_NW = 4,///< Anchor northwest corner for fbox
	FBOX_ANCHOR_NE = 5,///< Anchor northeast corner for fbox
	FBOX_ANCHOR_SE = 6,///< Anchor southeast corner for fbox
	FBOX_ANCHOR_SW = 7 ///< Anchor southwest corner for fbox
};

std::string PrintBoxAnchorPoint(BOX_ANCHOR_POINT p);


/**! @enum BOX_SPACE_RULES
 *   @brief How empty space is used in a box layout
 */

enum BOX_SPACE_RULES {
   BOX_ALIGN_ONLY    = 0,///< Left over space is unused
   BOX_EXPAND        = 1,///< Left over space is given completely to widgets, scaling them to fit the full area of the box
   BOX_SPACE_BETWEEN = 2,///< Left over space is split up between each widget, pushing them out from the middle
   BOX_SPACE_EVEN    = 3 ///< Left over space is split up evenly between each widget on its outer edges
};


/**! @fn PrintBoxSpaceRules
 *   @brief Turns an @ref BOX_SPACE_RULES enum into a text string
 */

std::string PrintBoxSpaceRule(BOX_SPACE_RULES b);


/**! @class BoxLayout
 *   @brief Base class for all box layouts
 */
class BoxLayout : public LayoutBase {
   
protected :
   BOX_SPACE_RULES spacing;
   BOX_ANCHOR_POINT anchor;

   std::vector<Rectangle> rcsizes;

   bool overflow;

   virtual void RecalcFlow()=0;
   
   
public :
   BoxLayout(std::string classname = "BoxLayout" , std::string objname = "Nemo");
   
   virtual Rectangle RequestWidgetArea(int widget_slot , int newx , int newy , int newwidth , int newheight) override;

   virtual void Resize(unsigned int nsize) override;

   virtual void PlaceWidget(WidgetBase* w , int slot);
   virtual int AddWidget(WidgetBase* w);
   virtual void InsertWidget(WidgetBase* w , int slot);
   
   void SetBoxSpacing(BOX_SPACE_RULES r);///< Set the box spacing for this box layout
   
   virtual void SetAnchorPosition(BOX_ANCHOR_POINT p)=0;///< Set the anchor point for this box layout
   virtual void SetAlignment(HALIGNMENT h_align , VALIGNMENT v_align);///< Set the horizontal and vertical alignment for this layout
   virtual int WidthLeft()=0;///< How much width is left in this layout
   virtual int HeightLeft()=0;///< How much height is left in this layout
   bool OverflowWarning();///< Whether or not this layout has overflowed
   virtual bool WidgetWouldOverflowLayout(WidgetBase* w)=0;///< Whether or not adding w to the layout would make it overflow

   std::ostream& DescribeTo(std::ostream& os , Indenter indent) const override;
};


/**! @class HBoxLayout
 *   @brief A horizontal box layout
 */

class HBoxLayout : public BoxLayout {
   
protected :
   int totalprefw;
   int maxprefh;
   int colcount;
   std::vector<int> colsizes;
   int colwidthleft;
   int rowheightleft;
   int defwidth;
   int defheight;
   
   
   virtual void RecalcFlow();
   
public :
   HBoxLayout(std::string classname = "HBoxLayout" , std::string objname = "Nemo");
   
   virtual void SetAnchorPosition(BOX_ANCHOR_POINT p);///< Sets the @ref BOX_ANCHOR_POINT of this layout - may be @ref HBOX_ANCHOR_E or @ref HBOX_ANCHOR_W
   
   virtual int WidthLeft();
   virtual int HeightLeft();

   virtual bool WidgetWouldOverflowLayout(WidgetBase* w);
};



/**! @class VBoxLayout
 *   @brief A vertical box layout
 */

class VBoxLayout : public BoxLayout {
protected :
   int maxprefw;
   int totalprefh;
   int rowcount;
   std::vector<int> rowsizes;
   int colwidthleft;
   int rowheightleft;
   int defwidth;
   int defheight;
   
   virtual void RecalcFlow();
   
public :
   VBoxLayout(std::string classname = "VBoxLayout" , std::string objname = "Nemo");
   
   virtual void SetAnchorPosition(BOX_ANCHOR_POINT p) override;///< Sets the anchor position for this layout, @ref VBOX_ANCHOR_N or @ref VBOX_ANCHOR_S
   
   virtual int WidthLeft() override;
   virtual int HeightLeft() override;
   
   virtual bool WidgetWouldOverflowLayout(WidgetBase* w) override;
};



#endif // BoxLayout_HPP




 
 
 
