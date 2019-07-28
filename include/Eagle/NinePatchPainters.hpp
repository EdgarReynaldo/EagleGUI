
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
 * @file NinePatchPainters.hpp
 * @brief Some simple nine patch painting functions
 */

#ifndef NinePatchPainters_HPP
#define NinePatchPainters_HPP



#include "Eagle/Gui/WidgetArea.hpp"
#include "Eagle/Color.hpp"

/// Forward declarations

class EagleGraphicsContext;



/**! @typedef NPPAINTER
 *   @brief The NPPAINTER typedef serves to make a template for nine patch painter functions
 *
 *   Parameters are an Eagle graphics context pointer, a nine patch area, and a primary and secondary color
 *   The last parameter is optional and can be used to customize the function with specific data
 *   Make sure you're passing the right kind of objects in the void* data field.
 */

typedef void (*NPPAINTER) (EagleGraphicsContext* , NPAREA , EagleColor , EagleColor , void*);


///< Paint the outside area of the specified nine patch with a fill. This will produce a rectangular outline, like a border.
void PaintNPOutsideFill   (EagleGraphicsContext* win , NPAREA np , EagleColor pcol , EagleColor scol , void* unused);

///< Paint the outside area of the specified nine patch with a rounded rectangle. This will produce a rectangular outline, like a border.
void PaintNPOutsideRounded(EagleGraphicsContext* win , NPAREA np , EagleColor pcol , EagleColor scol , void* unused);

///< Paint the outside area of the specified nine patch with a contrasting border. Primary color is the outer color.
///< PaintNPOutsideRContrast reverses the colors, so pcol will be the inner color
void PaintNPOutsideContrast(EagleGraphicsContext* win , NPAREA np , EagleColor pcol , EagleColor scol , void* unused);
void PaintNPOutsideRContrast(EagleGraphicsContext* win , NPAREA np , EagleColor pcol , EagleColor scol , void* unused);

///< Paint the outside area of the specified nine patch with a contrasting border as a gradient. Primary color is the outer color.
///< The corners will be beveled on the diagonals like an octagon
void PaintNPOutsideGradient(EagleGraphicsContext* win , NPAREA np , EagleColor pcol , EagleColor scol , void* unused);





#endif // NinePatchPainters_HPP




