
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
 * @file NinePatch.hpp
 * @brief A simple interface and class for working with nine patch images, like buttons
 */

#ifndef NinePatch_HPP
#define NinePatch_HPP



#include "Eagle/Gui/WidgetArea.hpp"



class EagleGraphicsContext;
class EagleImage;



/**! @class NinePatch
 *   @brief A class to store nine patch images
 */

class NinePatch {

friend NinePatch MakeNinePatch(EagleGraphicsContext* win , EagleImage* src_img , WidgetArea nparea);

   EagleImage* imgs[3][3];///< EagleImage array, row major from top to bottom, left to right
   EagleGraphicsContext* window;///< Our graphics context window

   NinePatch();///< Private constructor, accessible through @ref MakeNinePatch
   
public :
   ~NinePatch();///< Public destructor, frees any stored images
   
   void Free();///< Free early if desired, will be freed on destruction

   NinePatch(const NinePatch& np);///< Copy construct a nine patch
   NinePatch& operator=(const NinePatch& np);///< Assign one nine patch to another
   
   EagleImage** operator[](int yindex);///< Get a pointer to a sub array of images in this nine patch
   EagleImage* const* operator[](int index) const;///< Get a direct const pointer to image 0-8
   
   /**! @fn Draw <WidgetArea , int , int>
    *   @brief Draws the nine patch to the specified widget area using the specified optional offset
    */

   void Draw(NPAREA dest_area , int xoffset = 0 , int yoffset = 0);
};


/**! @fn MakeNinePatch <EagleGraphicsContext* , EagleImage* , WidgetArea>
 *   @brief Makes a NinePatch object from a source image and nine patch area on the specified window
 */

NinePatch MakeNinePatch(EagleGraphicsContext* win , EagleImage* src_img , WidgetArea nparea);




#endif // NinePatch_HPP



