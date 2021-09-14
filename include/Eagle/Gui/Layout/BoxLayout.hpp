
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
   HBOX_ANCHOR_W = 1 << 1,/// Anchor the flow on the west side, for hbox 1
   HBOX_ANCHOR_E = 1 << 2,/// Anchor the flow on the east side, for hbox 2
   VBOX_ANCHOR_N = 1 << 3,/// Anchor the flow on the north side, for vbox 4
   VBOX_ANCHOR_S = 1 << 4,/// Anchor the flow on the south side, for vbox 8
	FBOX_ANCHOR_NW = HBOX_ANCHOR_W | VBOX_ANCHOR_N,///< Anchor the flow in the NW corner, for flow 5
	FBOX_ANCHOR_NE = HBOX_ANCHOR_E | VBOX_ANCHOR_N,///< Anchor the flow in the NE corner, for flow 6
	FBOX_ANCHOR_SE = HBOX_ANCHOR_E | VBOX_ANCHOR_S,///< Anchor the flow in the SE corner, for flow 10
	FBOX_ANCHOR_SW = HBOX_ANCHOR_W | VBOX_ANCHOR_S ///< Anchor the flow in the SW corner, for flow 9
};

std::string PrintBoxAnchorPoint(BOX_ANCHOR_POINT p);


/**! @enum BOX_SPACE_RULES
 *   @brief How empty space is used in a box layout
 */

enum BOX_SPACE_RULES {
   BOX_ALIGN_ONLY    = 0,///< Left over space is unused
   BOX_EXPAND        = 1,///< Left over space is given completely to widgets
   BOX_SPACE_BETWEEN = 2,///< Left over space is split up between each widget, pushing them out from the middle
   BOX_SPACE_EVEN    = 3 ///< Left over space is split up evenly between each widget on its outer edges
};


std::string PrintBoxSpaceRule(BOX_SPACE_RULES b);



class BoxLayout : public LayoutBase {
   
protected :
   
   virtual void RecalcFlow()=0;
   
   
public :
   BoxLayout(std::string classname = "BoxLayout" , std::string objname = "Nemo");
   
   virtual Rectangle RequestWidgetArea(int widget_slot , int newx , int newy , int newwidth , int newheight) override;

   virtual void Resize(unsigned int nsize) override;

   virtual void PlaceWidget(WidgetBase* w , int slot);
   virtual int AddWidget(WidgetBase* w);
   virtual void InsertWidget(WidgetBase* w , int slot);
   
   
   
   virtual void SetAnchorPosition(BOX_ANCHOR_POINT p)=0;
   
   virtual int WidthLeft()=0;
   virtual int HeightLeft()=0;
   virtual bool OverflowWarning()=0;
   bool WidgetWouldOverflowLayout(WidgetBase* w)=0;
};



class HBoxLayout : public BoxLayout {
   
protected :
   BOX_ANCHOR_POINT anchor;
   BOX_SPACE_RULES rules;
   int totalprefw;
   int maxprefh;
   int colcount;
   std::vector<int> colsizes;
   int colwidthleft;
   int rowheightleft;
   int defwidth;
   int defheight;
   bool overflow;
   
   
   virtual void RecalcFlow();
   
public :
   HBoxLayout(std::string classname = "HBoxLayout" , std::string objname = "Nemo");
   
   
   
   virtual void SetAnchorPosition(BOX_ANCHOR_POINT p);
   
   virtual int WidthLeft();

   virtual int HeightLeft();


   virtual bool OverflowWarning();
   
   virtual bool WidgetWouldOverflowLayout(WidgetBase* w);
   
   
};













#endif // BoxLayout_HPP




 
 
 
