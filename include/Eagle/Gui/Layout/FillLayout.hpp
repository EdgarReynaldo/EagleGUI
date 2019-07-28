
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
 *    Copyright 2009-2019+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file FillLayout.hpp
 * @brief Interface for the FillLayout class
 *
 */

#ifndef FillLayout_HPP
#define FillLayout_HPP

#include "Eagle/Gui/Layout/Layout.hpp"



/**! @class FillLayout
 *   @brief A simple layout class that expands its contents to fit its specified size
 *          this means when it is resized, the widget it holds will be resized as well
 */

class FillLayout : public LayoutBase {
   
   virtual void ReserveSlots(int nslots);

public :
   
   FillLayout(std::string objname = "Nemo");
   
   virtual Rectangle RequestWidgetArea(int widget_slot , int newx , int newy , int newwidth , int newheight) const;///< Always returns its own area
   
   virtual void PlaceWidget(SHAREDWIDGET widget , int slot);///< Only valid slot is zero
   virtual int AddWidget(SHAREDWIDGET widget);///< Will only add a widget to an empty layout

   virtual std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const;///< Describe this object to a stream
};




#endif // FillLayout_HPP



