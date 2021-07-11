

#include "Eagle/Lib.hpp"
#include "Eagle/Events.hpp"
#include "Eagle/Exception.hpp"
#include "Eagle/CXX11Mutexes.hpp"
#include "Eagle/StringWork.hpp"


#include "Eagle/backends/Allegro5/Allegro5Color.hpp"
#include "Eagle/backends/Allegro5/Allegro5EventHandler.hpp"
#include "Eagle/backends/Allegro5/Allegro5Font.hpp"
#include "Eagle/backends/Allegro5/Allegro5GraphicsContext.hpp"
#include "Eagle/backends/Allegro5/Allegro5Image.hpp"
#include "Eagle/backends/Allegro5/Allegro5MousePointer.hpp"
#include "Eagle/backends/Allegro5/Allegro5WindowManager.hpp"

#include "allegro5/allegro.h"
#include "allegro5/allegro_primitives.h"




ALLEGRO_VERTEX MakeAllegro5Vertex(float x , float y , float z , float u , float v , ALLEGRO_COLOR ac) {
	ALLEGRO_VERTEX vtx;
	vtx.x = x;
	vtx.y = y;
	vtx.z = z;
	vtx.u = u;
	vtx.v = v;
	vtx.color = ac;
	return vtx;
}






/// -------------------------------      Allegro5GraphicsContext      -----------------------------------



void Allegro5GraphicsContext::ResetBackBuffer() {
   realbackbuffer.ReferenceBitmap(al_get_backbuffer(display));
   backbuffer = &realbackbuffer;
   SetDrawingTarget(backbuffer);
}



void Allegro5GraphicsContext::PrivateFlipDisplay() {
   al_flip_display();
   ResetBackBuffer();
}



Allegro5GraphicsContext::Allegro5GraphicsContext(std::string objname , int width , int height , int flags) :
      EagleGraphicsContext("Allegro5GraphicsContext" , objname),
      display(0),
      realbackbuffer("A5GC::realbackbuffer"),
      blender_op(ALLEGRO_ADD),
      blender_src(ALLEGRO_ONE),
      blender_dest(ALLEGRO_INVERSE_ALPHA),
      blender_stack(),
      allegro5transformer()
{
   our_transformer = &allegro5transformer;
   if (width < 0) {width = 0;}
   if (height < 0) {height = 0;}
   if (width && height) {
      Create(width , height , flags);
   }
}



Allegro5GraphicsContext::~Allegro5GraphicsContext() {
   Destroy();
}



EagleSystem* Allegro5GraphicsContext::GetSystem() {
   return Eagle::EagleLibrary::System("Allegro5");
}



bool Allegro5GraphicsContext::Create(int width , int height , int flags) {
   Destroy();

   /// TODO : Convert EAGLE_FLAGS into ALLEGRO_FLAGS
   al_set_new_display_flags(flags);
   display = al_create_display(width,height);

   if (!display) {
      EagleCritical() << "Failed to create an allegro display for the Allegro 5 Graphics Context at " << this << std::endl;
      return false;
   }

   scrw = al_get_display_width(display);
   scrh = al_get_display_height(display);

   GetAllegro5WindowManager()->AddDisplay(this , display);

   LoadDefaultFont();

   ResetBackBuffer();

   window_mutex = new CXX11Mutex("A5GC::window_mutex");
   
   if (!window_mutex->Create(false , false)) {
      EagleCritical() << "Failed to Create new CXX11Mutex for the Allegro 5 Graphics Context at " << this << std::endl;
      return false;
   }

   mp_manager = new Allegro5MousePointerManager(this);

   EagleEvent ee;
   ee.type = EAGLE_EVENT_DISPLAY_CREATE;
   ee.window = this;
   ee.display = DISPLAY_EVENT_DATA();

   EmitEvent(ee , 0);

   return true;
}



bool Allegro5GraphicsContext::Valid() {
   return (display);
}



void Allegro5GraphicsContext::Destroy() {

   FreeAllImages();
   FreeAllFonts();
   
   if (display) {

       EagleEvent ee;
       ee.type = EAGLE_EVENT_DISPLAY_DESTROY;
       ee.window = this;
       ee.display = DISPLAY_EVENT_DATA();

       EmitEvent(ee , 0);


      /// Block until after drawing is done and releases the lock,
      /// or acquire the lock before drawing and prevent it
      ThreadLockMutex(our_thread , window_mutex);

      /// Make the display current before destroying it
      al_set_target_backbuffer(display);
      
      GetAllegro5WindowManager()->SignalClose(this);

      al_destroy_display(display);

      GetAllegro5WindowManager()->RemoveDisplay(display);

      display = 0;

      ThreadUnLockMutex(our_thread , window_mutex);
   }

   if (window_mutex) {
      delete window_mutex;
      window_mutex = 0;
   }
   
   if (mp_manager) {
      delete mp_manager;
      mp_manager = 0;
   }

}



void Allegro5GraphicsContext::AcknowledgeResize() {
   EAGLE_ASSERT(display);
   al_acknowledge_resize(display);
   scrw = al_get_display_width(display);
   scrh = al_get_display_height(display);
}



int Allegro5GraphicsContext::XPos() {
   if (display) {
      int x = 0 , y = 0;
      al_get_window_position(display , &x , &y);
      return x;
   }
   return -1;
}



int Allegro5GraphicsContext::YPos() {
   if (display) {
      int x = 0 , y = 0;
      al_get_window_position(display , &x , &y);
      return y;
   }
   return -1;
}



Pos2I Allegro5GraphicsContext::GetMaxTextureSize() {
   int maxwidth = al_get_display_option(display , ALLEGRO_MAX_BITMAP_SIZE);
   return Pos2I(maxwidth , maxwidth);
}



void Allegro5GraphicsContext::LoadDefaultFont() {
   if (default_font) {
      FreeFont(default_font);
      default_font = 0;
   }
   default_font_path = DefaultFontPath();
   default_font_size = DefaultFontSize();
   default_font_flags = DefaultFontFlags();

   default_font = LoadFont(default_font_path.c_str() , default_font_size , default_font_flags , VIDEO_IMAGE);

   if (!default_font || !default_font->Valid()) {
      EagleWarn() << StringPrintF("Failed to load default font %s at size %d with flags %d" ,
                                   default_font_path.c_str() , default_font_size , default_font_flags) << std::endl;
      FreeFont(default_font);
      default_font = new Allegro5Font(al_create_builtin_font());
      if (!default_font->Valid()) {
         EagleError() << "Failed to create built in font for fallback. No Default Font available." << std::endl;
         delete default_font;
         default_font = 0;
      }
      else {
         EagleWarn() << "Using allegro's built in font for default font" << std::endl;
      }
   }
   if (default_font) {
      fontset.insert(default_font);
   }
   
   EagleInfo() << StringPrintF("Allegro5GraphicsContext::LoadDefaultFont : Default font is %s , size %d\n" ,
                              (default_font && default_font->Valid())?"Valid":"Invalid" , default_font->Height()) << std::endl;
}



// clears target bitmap
void Allegro5GraphicsContext::Clear(EagleColor c) {
   ALLEGRO_COLOR ac = GetAllegroColor(c);
   al_clear_to_color(ac);
}



/// Blender operations



void Allegro5GraphicsContext::SetCopyBlender() {
   StoreBlender();
   blender_op = ALLEGRO_ADD;
   blender_src = ALLEGRO_ONE;
   blender_dest = ALLEGRO_ZERO;
   al_set_blender(blender_op , blender_src , blender_dest);
}



void Allegro5GraphicsContext::SetFullCopyBlender() {
   StoreBlender();
   
///void al_set_separate_blender(int op, int src, int dst, int alpha_op, int alpha_src, int alpha_dst)
   blender_op = ALLEGRO_ADD;
   blender_src = ALLEGRO_ONE;
   blender_dest = ALLEGRO_ZERO;
   al_set_separate_blender(blender_op , blender_src , blender_dest , blender_op , blender_src , blender_dest);
}



void Allegro5GraphicsContext::SetPMAlphaBlender() {
   StoreBlender();
   blender_op = ALLEGRO_ADD;
   blender_src = ALLEGRO_ONE;
   blender_dest = ALLEGRO_INVERSE_ALPHA;
   al_set_blender(blender_op , blender_src , blender_dest);

}



void Allegro5GraphicsContext::SetNoPMAlphaBlender() {
   StoreBlender();
   blender_op = ALLEGRO_ADD;
   blender_src = ALLEGRO_ALPHA;
   blender_dest = ALLEGRO_INVERSE_ALPHA;
   al_set_blender(blender_op , blender_src , blender_dest);
}



void Allegro5GraphicsContext::RestoreLastBlendingState() {
   RestoreBlender();
}



void Allegro5GraphicsContext::StoreBlender() {
   BLENDER b;
   b.blender_op = blender_op;
   b.blender_src = blender_src;
   b.blender_dest = blender_dest;
   blender_stack.push_back(b);
}



void Allegro5GraphicsContext::RestoreBlender() {
   if (blender_stack.size()) {
      BLENDER b = blender_stack.back();
      blender_stack.pop_back();
      al_set_blender(blender_op = b.blender_op , blender_src = b.blender_src , blender_dest = b.blender_dest);
   }
}



// basic drawing operations
void Allegro5GraphicsContext::PutPixel(int x , int y , EagleColor c) {
   ALLEGRO_COLOR ac = GetAllegroColor(c);
   al_put_pixel(x,y,ac);
}



void Allegro5GraphicsContext::DrawLine(int x1 , int y1 , int x2 , int y2 , EagleColor c) {
	ALLEGRO_COLOR ac = GetAllegroColor(c);
	al_draw_line(x1 + 0.5f , y1 + 0.5f , x2 + 0.5f , y2 + 0.5f , ac , 1.0);
}



void Allegro5GraphicsContext::DrawRectangle(int x , int y , int w , int h , int thickness , EagleColor c) {
   float fx = x;
   float fy = y;
   float fw = w;
   float fh = h;
   float fthickness = thickness;
   ALLEGRO_COLOR ac = GetAllegroColor(c);

   const float onehalf = 1.0f/2.0f;
   al_draw_rectangle( fx + onehalf , fy + onehalf , fx + fw - 1.0 , fy + fh - 1.0 , ac , fthickness);
}



void Allegro5GraphicsContext::DrawFilledRectangle(int x , int y , int w , int h , EagleColor c) {
   ALLEGRO_COLOR ac = GetAllegroColor(c);
   al_draw_filled_rectangle(x , y , x + w , y + h , ac);
}



void Allegro5GraphicsContext::DrawRoundedRectangle(int x , int y , int w , int h , int rx , int ry , EagleColor c) {
	ALLEGRO_COLOR ac = GetAllegroColor(c);
	al_draw_rounded_rectangle(x , y , x + w , y + h , rx , ry , ac , 1.0);
}



void Allegro5GraphicsContext::DrawFilledRoundedRectangle(int x , int y , int w , int h , int rx , int ry , EagleColor c) {
	ALLEGRO_COLOR ac = GetAllegroColor(c);
	al_draw_filled_rounded_rectangle(x , y , x + w , y + h , rx , ry , ac);
}



void Allegro5GraphicsContext::DrawCircle(int cx , int cy , int radius , int thickness , EagleColor c) {
   ALLEGRO_COLOR ac = GetAllegroColor(c);
   if (radius >= 0) {
      al_draw_circle(cx + 0.5f , cy + 0.5f , radius , ac , thickness);
   }
}



void Allegro5GraphicsContext::DrawFilledCircle(int cx , int cy , int radius , EagleColor c) {
   ALLEGRO_COLOR ac = GetAllegroColor(c);
   if (radius >= 0) {
      al_draw_filled_circle(cx + 0.5f , cy + 0.5f , radius , ac);
   }
}



void Allegro5GraphicsContext::DrawEllipse(int cx , int cy , int rx , int ry , int thickness , EagleColor c) {
	ALLEGRO_COLOR ac = GetAllegroColor(c);
	if (rx >= 0 && ry >= 0) {
      al_draw_ellipse(cx + 0.5f , cy + 0.5f , rx , ry , ac , thickness);
	}
}



void Allegro5GraphicsContext::DrawFilledEllipse(int cx , int cy , int rx , int ry , EagleColor c) {
	ALLEGRO_COLOR ac = GetAllegroColor(c);
	if (rx >= 0 && ry >= 0) {
      al_draw_filled_ellipse(cx + 0.5f , cy + 0.5f , rx , ry , ac);
	}
}



void Allegro5GraphicsContext::DrawTriangle(int x1 , int y1 , int x2 , int y2 , int x3 , int y3 , int thickness , EagleColor c) {
   ALLEGRO_COLOR ac = GetAllegroColor(c);
   al_draw_triangle(x1 + 0.5f , y1 + 0.5f , x2 + 0.5f , y2 + 0.5f , x3 + 0.5f , y3 + 0.5f , ac , thickness);
}



void Allegro5GraphicsContext::DrawFilledTriangle(int x1 , int y1 , int x2 , int y2 , int x3 , int y3 , EagleColor c) {
   ALLEGRO_COLOR ac = GetAllegroColor(c);
   al_draw_filled_triangle(x1 + 0.5f , y1 + 0.5f , x2 + 0.5f , y2 + 0.5f , x3 + 0.5f , y3 + 0.5f , ac);
}



// precise drawing operations
void Allegro5GraphicsContext::PutPixel(float x , float y , EagleColor c) {
   ALLEGRO_COLOR ac = GetAllegroColor(c);
   al_draw_pixel(x,y,ac);
}



void Allegro5GraphicsContext::DrawLine(float x1 , float y1 , float x2 , float y2 , float thickness , EagleColor c) {
	ALLEGRO_COLOR ac = GetAllegroColor(c);
	al_draw_line(x1 , y1 , x2 , y2 , ac , thickness);
}



void Allegro5GraphicsContext::DrawRectangle(float x , float y , float w , float h , float thickness , EagleColor c) {
   ALLEGRO_COLOR ac = GetAllegroColor(c);
   al_draw_rectangle(x , y , x + w , y + h , ac , thickness);
}



void Allegro5GraphicsContext::DrawFilledRectangle(float x , float y , float w , float h , EagleColor c) {
   ALLEGRO_COLOR ac = GetAllegroColor(c);
   al_draw_filled_rectangle(x , y , x + w , y + h , ac);
}



void Allegro5GraphicsContext::DrawRoundedRectangle(float x , float y , float w , float h , float rx , float ry , float thickness , EagleColor c) {
	ALLEGRO_COLOR ac = GetAllegroColor(c);
	al_draw_rounded_rectangle(x , y , x + w , y + h , rx , ry , ac , thickness);
}



void Allegro5GraphicsContext::DrawFilledRoundedRectangle(float x , float y , float w , float h , float rx , float ry , EagleColor c) {
	ALLEGRO_COLOR ac = GetAllegroColor(c);
	al_draw_filled_rounded_rectangle(x , y , x + w , y + h , rx , ry , ac);
}



void Allegro5GraphicsContext::DrawCircle(float cx , float cy , float radius , float thickness , EagleColor c) {
   ALLEGRO_COLOR ac = GetAllegroColor(c);
   al_draw_circle(cx , cy , radius , ac , thickness);
}



void Allegro5GraphicsContext::DrawFilledCircle(float cx , float cy , float radius , EagleColor c) {
   ALLEGRO_COLOR ac = GetAllegroColor(c);
   al_draw_filled_circle(cx , cy , radius , ac);
}



void Allegro5GraphicsContext::DrawEllipse(float cx , float cy , float rx , float ry , float thickness , EagleColor c) {
	ALLEGRO_COLOR ac = GetAllegroColor(c);
	al_draw_ellipse(cx , cy , rx , ry , ac , thickness);
}



void Allegro5GraphicsContext::DrawFilledEllipse(float cx , float cy , float rx , float ry , EagleColor c) {
	ALLEGRO_COLOR ac = GetAllegroColor(c);
	al_draw_filled_ellipse(cx , cy , rx , ry , ac);
}



void Allegro5GraphicsContext::DrawTriangle(float x1 , float y1 ,
                                           float x2 , float y2 ,
                                           float x3 , float y3 ,
                                           float thickness , EagleColor c) {
   ALLEGRO_COLOR ac = GetAllegroColor(c);
   al_draw_triangle(x1 , y1 , x2 , y2 , x3 , y3 , ac , thickness);
}



void Allegro5GraphicsContext::DrawFilledTriangle(float x1 , float y1 , float x2 , float y2 , float x3 , float y3 , EagleColor c) {
   ALLEGRO_COLOR ac = GetAllegroColor(c);
   al_draw_filled_triangle(x1 , y1 , x2 , y2 , x3 , y3 , ac);
}



void Allegro5GraphicsContext::DrawShadedRectangle(const Rectangle* r , EagleColor tl , EagleColor tr , EagleColor br , EagleColor bl) {
	EAGLE_ASSERT(r);
	float lx = r->X();
	float rx = r->X() + r->W();
	float ty = r->Y();
	float by = r->Y() + r->H();
	DrawShadedQuad(lx,ty,tl , rx,ty,tr , rx,by,br , lx,by,bl);
}



void Allegro5GraphicsContext::DrawShadedQuad(float x1 , float y1 , EagleColor c1 ,
															float x2 , float y2 , EagleColor c2 ,
															float x3 , float y3 , EagleColor c3 ,
															float x4 , float y4 , EagleColor c4) {
	ALLEGRO_VERTEX vtx[4] = {
		MakeAllegro5Vertex(x1 , y1 , 0.0 , 0.0 , 0.0 , GetAllegroColor(c1)),
		MakeAllegro5Vertex(x2 , y2 , 0.0 , 0.0 , 0.0 , GetAllegroColor(c2)),
		MakeAllegro5Vertex(x3 , y3 , 0.0 , 0.0 , 0.0 , GetAllegroColor(c3)),
		MakeAllegro5Vertex(x4 , y4 , 0.0 , 0.0 , 0.0 , GetAllegroColor(c4))
	};

	al_draw_prim(vtx , 0 , 0 , 0 , 4 , ALLEGRO_PRIM_TRIANGLE_FAN);
}



void Allegro5GraphicsContext::DrawShadedTexturedQuad(float x1 , float y1 , float u1 , float v1 , EagleColor c1 ,
                                                     float x2 , float y2 , float u2 , float v2 , EagleColor c2 ,
                                                     float x3 , float y3 , float u3 , float v3 , EagleColor c3 ,
                                                     float x4 , float y4 , float u4 , float v4 , EagleColor c4 ,
                                                     EagleImage* texture) {
	ALLEGRO_VERTEX vtx[4] = {
		MakeAllegro5Vertex(x1 , y1 , 0.0 , u1 , v1 , GetAllegroColor(c1)),
		MakeAllegro5Vertex(x2 , y2 , 0.0 , u2 , v2 , GetAllegroColor(c2)),
		MakeAllegro5Vertex(x3 , y3 , 0.0 , u3 , v3 , GetAllegroColor(c3)),
		MakeAllegro5Vertex(x4 , y4 , 0.0 , u4 , v4 , GetAllegroColor(c4))
	};
   ALLEGRO_BITMAP* bmp = GetAllegroBitmap(texture);
	al_draw_prim(vtx , 0 , bmp , 0 , 4 , ALLEGRO_PRIM_TRIANGLE_FAN);
}



void Allegro5GraphicsContext::DrawTintedStretchedRegion(EagleImage* img , float sx , float sy , float sw , float sh , float dx , float dy , float dw , float dh , EagleColor tint , int flags) {
   
   if (!flags) {
      DrawShadedTexturedQuad(
            dx      , dy      , sx      , sy      , tint ,
            dx      , dy + dh , sx      , sy + sh , tint ,
            dx + dw , dy + dh , sx + sw , sy + sh , tint ,
            dx + dw , dy      , sx + sw , sy      , tint ,
            img);
   }
   else if (flags == DRAW_HFLIP) {/// Exchange texture coordinates between corners 1 and 4 and 2 and 3
      DrawShadedTexturedQuad(
            dx      , dy      , sx + sw , sy      , tint ,
            dx      , dy + dh , sx + sw , sy + sh , tint ,
            dx + dw , dy + dh , sx      , sy + sh , tint ,
            dx + dw , dy      , sx      , sy      , tint ,
            img);
   }
   else if (flags == DRAW_VFLIP) {/// Exchange texture coordinates between corners 1 and 2 and 3 and 4
      DrawShadedTexturedQuad(
            dx      , dy      , sx      , sy + sh , tint ,
            dx      , dy + dh , sx      , sy      , tint ,
            dx + dw , dy + dh , sx + sw , sy      , tint ,
            dx + dw , dy      , sx + sw , sy + sh , tint ,
            img);
   }
   else if (flags == DRAW_HVFLIP) {/// Exchange texture coordinates between corners 1 and 3 and 2 and 4
      DrawShadedTexturedQuad(
            dx      , dy      , sx + sw , sy + sh , tint ,
            dx      , dy + dh , sx + sw , sy      , tint ,
            dx + dw , dy + dh , sx      , sy      , tint ,
            dx + dw , dy      , sx      , sy + sh , tint ,
            img);
   }
}



void Allegro5GraphicsContext::ConvertColorToAlpha(EagleImage* img , EagleColor alpha_color) {
   Allegro5Image* a5img = dynamic_cast<Allegro5Image*>(img);
   ALLEGRO_BITMAP* allegro_bmp = a5img->AllegroBitmap();
   al_convert_mask_to_alpha(allegro_bmp , GetAllegroColor(alpha_color));
}



/**
TODO TODO TODO ADD THIS FUNCTION FINISH IT BIHATCHIAN BIYATCH BITCH FACE
void Allegro5GraphicsContext::Draw(EagleImage* src , EagleDrawingInfo info) {

//   bool use_any;
//   bool use_region;
//   bool use_scale;
//   bool use_resize;// if false, destination point will be used, which is default
//   bool use_rotate;
//   bool use_tint;


   ALLEGRO_COLOR tint = GetAllegroColor(info.tint);
   Allegro5Image* a5img = dynamic_cast<Allegro5Image*>(src);
   EAGLE_ASSERT(a5img);
   ALLEGRO_BITMAP* bmp = a5img->AllegroBitmap();
   EAGLE_ASSERT(bmp);

   if (info.use_any) {

// Drawing operations
//
//    al_draw_bitmap_region
//    al_draw_tinted_bitmap_region
//    al_draw_tinted_scaled_rotated_bitmap_region
//
//    al_draw_bitmap
//    al_draw_scaled_bitmap
//    al_draw_tinted_bitmap
//    al_draw_rotated_bitmap
//
//    al_draw_tinted_rotated_bitmap
//    al_draw_scaled_rotated_bitmap
//    al_draw_tinted_scaled_rotated_bitmap
//    al_draw_tinted_scaled_bitmap
//

      if (info.use_region) {
         // Using source region
         if (use_resize) {
            // simple stretch blit
            if (tint) {
               // tint,resize,region

//void al_draw_tinted_scaled_bitmap(ALLEGRO_BITMAP *bitmap,
//   ALLEGRO_COLOR tint,
//   float sx, float sy, float sw, float sh,
//   float dx, float dy, float dw, float dh, int flags)

               al_draw_tinted_scaled_bitmap(bmp , tint , info.region.x , info.region.y , info.region.w , info.region.h,
                                            info.resize.dx , info.resize.dy , info.resize.dw , info.resize.dh , info.flags);
            }
            else {

            }
         }
         else if (use_rotate) {
            float xscale = 1.0f;
            float yscale = 1.0f;
            if (use_scale) {
               xscale = info.scale.x;
               yscale = info.scale.y;
            }
         }


      }
      else {

      }
   }

}
*/






void Allegro5GraphicsContext::DrawTextString(EagleFont* font , std::string str , float x , float y , EagleColor c ,
                                             HALIGNMENT halign , VALIGNMENT valign) {
   EAGLE_ASSERT(font);
   EAGLE_ASSERT(font->Valid());
   Allegro5Font* a5font = dynamic_cast<Allegro5Font*>(font);
   EAGLE_ASSERT(a5font);
   ALLEGRO_FONT* f = a5font->AllegroFont();
   EAGLE_ASSERT(f);
   if (halign != HALIGN_LEFT || valign != VALIGN_TOP) {
      int textwidth = al_get_text_width(f , str.c_str());
      int textheight = al_get_font_line_height(f);
      if (halign == HALIGN_CENTER) {x -= textwidth/2;}
      if (halign == HALIGN_RIGHT) {x -= textwidth;}
      if (valign == VALIGN_CENTER) {y -= textheight/2;}
      if (valign == VALIGN_BOTTOM) {y -= textheight;}
   }
   al_draw_text(f , GetAllegroColor(c) , x , y , 0 , str.c_str());
}



void Allegro5GraphicsContext::DrawVTextString(EagleFont* font , std::string str , float x , float y , EagleColor c ,
                                              HALIGNMENT halign , VALIGNMENT valign , int letter_spacing) {
   if (!str.size()) {return;}
   EAGLE_ASSERT(font);
   EAGLE_ASSERT(font->Valid());
   Allegro5Font* a5font = dynamic_cast<Allegro5Font*>(font);
   EAGLE_ASSERT(a5font);
   ALLEGRO_FONT* f = a5font->AllegroFont();
   EAGLE_ASSERT(f);
   
   if (letter_spacing < 0) {
      letter_spacing = al_get_font_line_height(f)/8;
   }
   
   int vheight = font->VHeight(str , -1);
   int vwidth = font->VWidth(str , 0);
   
   switch (valign) {
   case VALIGN_CENTER :
      y -= vheight/2;
      break;
   case VALIGN_BOTTOM :
      y -= vheight;
      break;
   default : break;
   };
   al_hold_bitmap_drawing(true);
   for (unsigned int i = 0 ; i < str.size() ; ++i) {
      int bbx = 0 , bby = 0 , bbw = 0 , bbh = 0;
      ALLEGRO_GLYPH g;
      memset(&g , 0 , sizeof(g));
      if (al_get_glyph(f , 0 , str[i] , &g)) {
         int ox = 0;
         switch (halign) {
         case HALIGN_LEFT :
            break;
         case HALIGN_CENTER :
            ox += (vwidth - g.w)/2;
            break;
         case HALIGN_RIGHT :
            ox += vwidth - g.w;
            break;
         default : break;
         };
         al_draw_bitmap_region(g.bitmap , g.x , g.y , g.w , g.h , x + ox , y , 0);
         y += g.h + letter_spacing;
      }
   }
   al_hold_bitmap_drawing(false);
}



/// Getters



EagleImage* Allegro5GraphicsContext::GetBackBuffer() {
   EAGLE_ASSERT(backbuffer);
   return backbuffer;
}



EagleImage* Allegro5GraphicsContext::GetScreen() {
   return 0;
}



EagleImage* Allegro5GraphicsContext::GetDrawingTarget() {
   return drawing_target;
}



// utilities
void Allegro5GraphicsContext::HoldDrawing() {
   al_hold_bitmap_drawing(true);
}



void Allegro5GraphicsContext::ReleaseDrawing() {
   al_hold_bitmap_drawing(false);
}



void Allegro5GraphicsContext::SetDrawingTarget(EagleImage* dest) {
   EAGLE_ASSERT(dest);
   Allegro5Image* a5img = dynamic_cast<Allegro5Image*>(dest);
   EAGLE_ASSERT(a5img);
   ALLEGRO_BITMAP* a5bmp = a5img->AllegroBitmap();
   EAGLE_ASSERT(a5bmp);
   al_set_target_bitmap(a5bmp);
///   ALLEGRO_DISPLAY* current_a5_display = al_get_current_display();
///   SetCurrentDisplay(display_context_map[current_a5_display]);
   drawing_target = dest;
   
   allegro5transformer.RebaseTransformStack(this);
}



EagleImage* Allegro5GraphicsContext::EmptyImage(std::string iname) {
   Allegro5Image* nimg = new Allegro5Image(iname);
   imageset.insert(nimg);
   return nimg;
}



EagleImage* Allegro5GraphicsContext::AdoptImage(ALLEGRO_BITMAP* img , std::string iname) {
   /// TODO : Conversion to new context?
   Allegro5Image* nimg = dynamic_cast<Allegro5Image*>(EmptyImage(iname));
   nimg->AdoptBitmap(img);
   return nimg;
}



EagleImage* Allegro5GraphicsContext::ReferenceImage(ALLEGRO_BITMAP* img , std::string iname) {
   Allegro5Image* nimg = dynamic_cast<Allegro5Image*>(EmptyImage(iname));
   nimg->ReferenceBitmap(img);
   return nimg;
}



EagleImage* Allegro5GraphicsContext::CloneImage(EagleImage* img , std::string iname) {
   EagleImage* clone = dynamic_cast<Allegro5Image*>(img->Clone(this , iname));
   if (clone) {
      imageset.insert(clone);
   }
   return clone;
}



EagleImage* Allegro5GraphicsContext::CreateSubImage(EagleImage* parent , int x , int y , int width , int height , std::string iname) {
   EagleImage* img = parent->CreateSubBitmap(x , y , width , height , iname);
   /// imgset.insert(img); parent is responsible for destroying this image, not us
   return img;
}



// image creation / loading / sub division
EagleImage* Allegro5GraphicsContext::CreateImage(int width , int height , IMAGE_TYPE type , std::string iname) {
   EagleImage* img = EmptyImage(iname);
   img->Allocate(width , height , type);
   return img;
}



EagleImage* Allegro5GraphicsContext::LoadImageFromFile(std::string file , IMAGE_TYPE type) {
   EagleImage* img = EmptyImage(file);
   img->Load(file , type);
   return img;
}



EagleFont* Allegro5GraphicsContext::LoadFont(std::string file , int height , int flags , IMAGE_TYPE type) {
   Allegro5Font* font = new Allegro5Font(file , height , flags , file , type);
   if (font->Valid()) {
      fontset.insert(font);
      return font;
   }
   delete font;
   return (EagleFont*)0;
}



Transformer* Allegro5GraphicsContext::GetTransformer() {
   return our_transformer;
}



void Allegro5GraphicsContext::MakeDisplayCurrent() {
   ResetBackBuffer();
}


void Allegro5GraphicsContext::SetWindowPosition(int screenx , int screeny) {
   al_set_window_position(display , screenx , screeny);
}


#ifdef EAGLE_WIN32
   #include "allegro5/allegro_windows.h"
   #include "windows.h"
   
#endif

void Allegro5GraphicsContext::ShowWindow() {
#ifdef EAGLE_WIN32
   HWND window = al_get_win_window_handle(display);
   ::ShowWindow(window , SW_SHOW);
#else
   #warning ShowWindow is undefined on this platform. It will fail silently.
#endif
}



void Allegro5GraphicsContext::HideWindow() {
#ifdef EAGLE_WIN32
   HWND window = al_get_win_window_handle(display);
   ::ShowWindow(window , SW_HIDE);
#else
   #warning ShowWindow is undefined on this platform. It will fail silently.
#endif
}




