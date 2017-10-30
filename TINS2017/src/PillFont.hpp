


#ifndef PillFont_HPP
#define PillFont_HPP


#include "Eagle/backends/Allegro5Backend.hpp"

#include "LousyGlobals.hpp"

extern const int NCHAR;

typedef void (*PILL_DRAW_FUNC)(EagleColor);

void DrawPill(EagleColor c);
void DrawPillOne(EagleColor c);
void DrawPillTwo(EagleColor c);
void DrawPillThree(EagleColor c);
void DrawPillFour(EagleColor c);

void DrawPillGlyph(int i);

EagleImage* CreatePillFontImage(int w , int h);
EagleFont* CreatePillFont(int w , int h);


#endif // PillFont_HPP
