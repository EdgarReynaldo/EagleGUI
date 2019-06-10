



#ifndef NinePatchPainters_HPP
#define NinePatchPainters_HPP

/// Forward declarations

class EagleGraphicsContext;

#include "Eagle/Gui/WidgetArea.hpp"
#include "Eagle/Color.hpp"



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
void PaintNPOutsideContrast(EagleGraphicsContext* win , NPAREA np , EagleColor pcol , EagleColor scol , void* unused);

///< Paint the outside area of the specified nine patch with a contrasting border as a gradient. Primary color is the outer color.
///< The corners will be beveled on the diagonals like an octagon
void PaintNPOutsideGradient(EagleGraphicsContext* win , NPAREA np , EagleColor pcol , EagleColor scol , void* unused);





#endif // NinePatchPainters_HPP




