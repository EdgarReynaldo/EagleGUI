
/*
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
 *    EAGLE
 *    Edgar's Agile Gui Library and Extensions
 *
 *    Copyright 2009-2014+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */

#ifndef FlowLayout_HPP
#define FlowLayout_HPP


#include "Eagle/Gui/Layout/Layout.hpp"




enum FLOW_ANCHOR_POINT {
	FLOW_ANCHOR_NW = 0,
	FLOW_ANCHOR_NE = 1,
	FLOW_ANCHOR_SE = 2,
	FLOW_ANCHOR_SW = 3
};

enum FLOW_FAVORED_DIRECTION {
	FLOW_FAVOR_VERTICAL = 0,
	FLOW_FAVOR_HORIZONTAL = 1
};


class FlowLayout : public Layout {/// TODO : IMPLEMENT ME!

protected :
	
	FLOW_ANCHOR_POINT anchor_pt;
	FLOW_FAVORED_DIRECTION favored_direction;

public :

   virtual ~FlowLayout();

};





#endif // FlowLayout_HPP




