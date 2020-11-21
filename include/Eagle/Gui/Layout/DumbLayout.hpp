
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
 * @file DumbLayout.hpp
 * @brief Stupid layout
 *
 */



#ifndef DumbLayout_HPP
#define DumbLayout_HPP


#include "Eagle/Gui/Layout/Layout.hpp"

/**! @class DumbLayout
 *   @brief A stupid layout that keeps everything right where you left it
 */

class DumbLayout : public LayoutBase {

public :
	DumbLayout(std::string name = "Nemo");
	~DumbLayout();

};




#endif // DumbLayout_HPP



