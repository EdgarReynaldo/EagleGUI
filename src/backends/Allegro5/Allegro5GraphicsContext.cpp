

#include "Eagle/Exception.hpp"
#include "Eagle/StringWork.hpp"

#include "Eagle/backends/Allegro5/Allegro5Color.hpp"
#include "Eagle/backends/Allegro5/Allegro5EventHandler.hpp"
#include "Eagle/backends/Allegro5/Allegro5Font.hpp"
#include "Eagle/backends/Allegro5/Allegro5GraphicsContext.hpp"
#include "Eagle/backends/Allegro5/Allegro5Image.hpp"
#include "Eagle/backends/Allegro5/Allegro5MousePointer.hpp"

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







std::map<ALLEGRO_DISPLAY* , EagleGraphicsContext*> display_context_map;




EagleGraphicsContext* GetAssociatedContext(ALLEGRO_DISPLAY* display) {
   std::map<ALLEGRO_DISPLAY* , EagleGraphicsContext*>::iterator it = display_context_map.find(display);
   if (it != display_context_map.end()) {
      return it->second;
   }
   return (EagleGraphicsContext*)0;
}



EagleEvent GetEagleDisplayEvent(ALLEGRO_EVENT ev) {
   EagleEvent ee;
   
   Allegro5GraphicsContext* a5win = 0;

   if (ev.type == EAGLE_EVENT_DISPLAY_CREATE || ev.type == EAGLE_EVENT_DISPLAY_DESTROY) {
      a5win = (Allegro5GraphicsContext*)ev.user.data1;
   
      ee.display.x = a5win->XPos();
      ee.display.y = a5win->YPos();
      ee.display.width = a5win->Width();
      ee.display.height = a5win->Height();
      ee.display.orientation = EAGLE_DISPLAY_ORIENTATION_0_DEGREES;
   }
   else {
      a5win = dynamic_cast<Allegro5GraphicsContext*>(GetAssociatedContext(ev.display.source));

      ee.display.x = ev.display.x;
      ee.display.y = ev.display.y;
      ee.display.width = ev.display.width;
      ee.display.height = ev.display.height;
      ee.display.orientation = ev.display.orientation;
   }

   EAGLE_ASSERT(a5win);
      
   ee.source = a5win;
   ee.window = a5win;


   switch(ev.type) {
   case EAGLE_EVENT_DISPLAY_CREATE :
      ee.type = EAGLE_EVENT_DISPLAY_CREATE;
      break;
   case EAGLE_EVENT_DISPLAY_DESTROY :
      ee.type = EAGLE_EVENT_DISPLAY_DESTROY;
      break;
   case ALLEGRO_EVENT_DISPLAY_EXPOSE :
      ee.type = EAGLE_EVENT_DISPLAY_EXPOSE;
      break;
   case ALLEGRO_EVENT_DISPLAY_RESIZE :
      ee.type = EAGLE_EVENT_DISPLAY_RESIZE;
      break;
   case ALLEGRO_EVENT_DISPLAY_CLOSE :
      ee.type = EAGLE_EVENT_DISPLAY_CLOSE;
      break;
   case ALLEGRO_EVENT_DISPLAY_LOST :
      ee.type = EAGLE_EVENT_DISPLAY_LOST;
      break;
   case ALLEGRO_EVENT_DISPLAY_FOUND :
      ee.type = EAGLE_EVENT_DISPLAY_FOUND;
      break;
   case ALLEGRO_EVENT_DISPLAY_SWITCH_OUT :
      ee.type = EAGLE_EVENT_DISPLAY_SWITCH_OUT;
      break;
   case ALLEGRO_EVENT_DISPLAY_SWITCH_IN :
      ee.type = EAGLE_EVENT_DISPLAY_SWITCH_IN;
      break;
   case ALLEGRO_EVENT_DISPLAY_ORIENTATION :
      ee.type = EAGLE_EVENT_DISPLAY_ORIENTATION;
      break;
   case ALLEGRO_EVENT_DISPLAY_HALT_DRAWING :
      ee.type = EAGLE_EVENT_DISPLAY_HALT_DRAWING;
      break;
   case ALLEGRO_EVENT_DISPLAY_RESUME_DRAWING :
      ee.type = EAGLE_EVENT_DISPLAY_RESUME_DRAWING;
      break;
      
   default :
      throw EagleException(StringPrintF("A5WindowProcess : We don't monitor this kind of event (%d)." , ev.type));
   }
   return ee;
}



void* A5WindowProcess(EagleThread* thread , void* context) {
   
   /// This thread monitors display events only
   
   EagleGraphicsContext* window = (EagleGraphicsContext*)context;
   Allegro5GraphicsContext* a5win = dynamic_cast<Allegro5GraphicsContext*>(window);
   
   if (!a5win) {
      throw EagleException("A5WindowProcess : context is invalid!");
   }
   
///   ALLEGRO_EVENT_SOURCE* window_event_source = &(a5win->window_event_source);
   ALLEGRO_EVENT_QUEUE* window_queue = (a5win->window_queue);
   
   while(!thread->ShouldStop()) {
      ALLEGRO_EVENT ev;
      al_wait_for_event(window_queue , &ev);
      
      EagleEvent ee = GetEagleDisplayEvent(ev);
      
      if (ev.type == EAGLE_EVENT_DISPLAY_CREATE) {
         a5win->SetActiveWindow(a5win);
      }
      else if (ev.type == ALLEGRO_EVENT_DISPLAY_SWITCH_OUT) {
         EAGLE_ASSERT(a5win->GetActiveWindow() == a5win);
         a5win->SetActiveWindow(0);
      }
      else if (ev.type == ALLEGRO_EVENT_DISPLAY_SWITCH_IN) {
         a5win->SetActiveWindow(a5win);
      }
      
      a5win->EmitEvent(ee);
      
      if (ev.type == EAGLE_EVENT_DISPLAY_DESTROY) {
         break;
      }
   }
   return context;
}



/// -------------------------------      Allegro5GraphicsContext      -----------------------------------



Allegro5Mutex Allegro5GraphicsContext::window_mutex;



void Allegro5GraphicsContext::Init() {
   window_queue = al_create_event_queue();
   EAGLE_ASSERT(window_queue);
   al_init_user_event_source(&window_event_source);
   al_register_event_source(window_queue , &window_event_source);
   
   if (!window_mutex.Create(false)) {
      throw EagleException("Allegro5GraphicsContext::Init : Failed to create window mutex.");
   }
   pmutex = &window_mutex;

   if (!window_thread.Create(A5WindowProcess , this)) {
      throw EagleException("Allegro5GraphicsContext::Init : Failed to create window process thread.");
   }
}



void Allegro5GraphicsContext::ResetBackBuffer() {
   realbackbuffer.ReferenceBitmap(al_get_backbuffer(display));
   backbuffer = &realbackbuffer;
   SetDrawingTarget(backbuffer);
}



void Allegro5GraphicsContext::PrivateFlipDisplay() {
   al_flip_display();
   ResetBackBuffer();
}



Allegro5GraphicsContext::Allegro5GraphicsContext() :
      EagleGraphicsContext(StringPrintF("Allegro5GraphicsContext at %p" , this)),
      display(0),
      realbackbuffer(),
      blender_op(ALLEGRO_ADD),
      blender_src(ALLEGRO_ONE),
      blender_dest(ALLEGRO_INVERSE_ALPHA),
      blender_stack(),
      allegro5transformer(),
      window_thread(),
      window_event_source(),
      window_queue(0)
{
   Init();
}



Allegro5GraphicsContext::Allegro5GraphicsContext(int width , int height , int flags) :
      EagleGraphicsContext(StringPrintF("Allegro5GraphicsContext at %p" , this)),
      display(0),
      realbackbuffer(),
      blender_op(ALLEGRO_ADD),
      blender_src(ALLEGRO_ONE),
      blender_dest(ALLEGRO_INVERSE_ALPHA),
      blender_stack(),
      allegro5transformer(),
      window_thread(),
      window_event_source(),
      window_queue(0)
{
   Init();
   Create(width , height , flags);
}



Allegro5GraphicsContext::~Allegro5GraphicsContext() {
   Destroy();
   
   
   
}


/*
EagleGraphicsContext* EagleGraphicsContext::GetActiveWindow() {
   EagleGraphicsContext* win = 0;
   pmutex->Lock();
   win = active_window;
   pmutex->Unlock();
   return win;
}
//*/


// creation/destruction
bool Allegro5GraphicsContext::Create(int width , int height , int flags) {
   Destroy();
   
   /// TODO : Convert EAGLE_FLAGS into ALLEGRO_FLAGS
   al_set_new_display_flags(flags);
   display = al_create_display(width,height);

   if (!display) {return false;}

   scrw = width;
   scrh = height;
   
   display_context_map[display] = this;
   
   ResetBackBuffer();

   mp_manager = new Allegro5MousePointerManager(this);
   
   LoadDefaultFont();
   
   ALLEGRO_EVENT ev;
   ev.type = EAGLE_EVENT_DISPLAY_CREATE;
   ev.user.data1 = (intptr_t)(this);
   
   
   window_thread.Start();
   
   al_emit_user_event(&window_event_source , &ev , 0);

   al_register_event_source(window_queue , al_get_display_event_source(display));
   
   return true;
}



bool Allegro5GraphicsContext::Valid() {
   return (display);
}



void Allegro5GraphicsContext::Destroy() {
   images.FreeAll();
   if (mp_manager) {
      delete mp_manager;
      mp_manager = 0;
   }
   if (display) {
      std::map<ALLEGRO_DISPLAY* , EagleGraphicsContext*>::iterator it = display_context_map.find(display);
      EAGLE_ASSERT(it != display_context_map.end());
      if (it != display_context_map.end()) {display_context_map.erase(it);}
      if (GetCurrentDisplay() == this) {
         SetCurrentDisplay(0);
      }
         
      ALLEGRO_EVENT ev;
      ev.type = EAGLE_EVENT_DISPLAY_DESTROY;
      ev.user.data1 = (intptr_t)this;      
      
      al_emit_user_event(&window_event_source , &ev , 0);
      
      window_thread.Join();
      
      al_unregister_event_source(window_queue , al_get_display_event_source(display));
      
      al_destroy_display(display);
      display = 0;
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
         delete default_font;
         default_font = 0;
      }
      else {
         EagleWarn() << "Using allegro's built in font for default font" << std::endl;
         fonts.Add(default_font);
      }
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



// image drawing operations
void Allegro5GraphicsContext::Draw(EagleImage* img , float x , float y , int flags) {
   ALLEGRO_BITMAP* bmp = GetAllegroBitmap(img);
   EAGLE_ASSERT(bmp);
   al_draw_bitmap(bmp , x , y , flags);
}



void Allegro5GraphicsContext::DrawRegion(EagleImage* img , Rectangle src , float x , float y , int flags) {
   ALLEGRO_BITMAP* bmp = GetAllegroBitmap(img);
   EAGLE_ASSERT(bmp);
//void al_draw_bitmap_region(ALLEGRO_BITMAP *bitmap, float sx, float sy, float sw, float sh, float dx, float dy, int flags);
   al_draw_bitmap_region(bmp , src.X() , src.Y() , src.W() , src.H() , x , y , flags);
}



void Allegro5GraphicsContext::DrawStretchedRegion(EagleImage* img ,
                                                  float sx , float sy , float sw , float sh ,
                                                  float dx , float dy , float dw , float dh ,
                                                  int flags) {
   ALLEGRO_BITMAP* bmp = GetAllegroBitmap(img);
   EAGLE_ASSERT(bmp);
   al_draw_scaled_bitmap(bmp , sx , sy , sw , sh , dx , dy , dw , dh , flags);
}



void Allegro5GraphicsContext::DrawTinted(EagleImage* img , int x , int y , EagleColor col) {
   ALLEGRO_BITMAP* bmp = GetAllegroBitmap(img);
   EAGLE_ASSERT(bmp);
   ALLEGRO_COLOR c = GetAllegroColor(col);
///   void al_draw_tinted_bitmap(ALLEGRO_BITMAP *bitmap, ALLEGRO_COLOR tint,
///   float dx, float dy, int flags)
   al_draw_tinted_bitmap(bmp , c , x , y , 0);
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
//void al_draw_text(const ALLEGRO_FONT *font,
//   ALLEGRO_COLOR color, float x, float y, int flags,
//   char const *text) 

   /// Need to set the premultiplied alpha blender here , and maybe the non-pm alpha too sometimes
   SetNoPMAlphaBlender();
///   SetPMAlphaBlender();
   al_draw_text(f , GetAllegroColor(c) , x , y , 0 , str.c_str());
   RestoreLastBlendingState();
}



// getters
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
   ALLEGRO_DISPLAY* current_a5_display = al_get_current_display();
   SetCurrentDisplay(display_context_map[current_a5_display]);
   drawing_target = dest;
}



EagleImage* Allegro5GraphicsContext::EmptyImage() {
   EagleImage* img = new Allegro5Image();
   images.Add(img);
   return img;
}



EagleImage* Allegro5GraphicsContext::AdoptImage(ALLEGRO_BITMAP* img) {
   EagleImage* eagle_image = new Allegro5Image(img , true);
   images.Add(eagle_image);
   return eagle_image;
}



EagleImage* Allegro5GraphicsContext::ReferenceImage(ALLEGRO_BITMAP* img) {
   EagleImage* eagle_image = new Allegro5Image(img , false);
   images.Add(eagle_image);
   return eagle_image;
}



EagleImage* Allegro5GraphicsContext::CloneImage(EagleImage* img) {
   Allegro5Image* newimg = new Allegro5Image();
   newimg->Allocate(img->W() , img->H() , img->ImageType());
   /// TODO Set blender here
   PushDrawingTarget(newimg);
   /// TODO Reset blending here
   Draw(img , 0.0f , 0.0f);
   
   PopDrawingTarget();
   
   images.Add(newimg);
   return newimg;
}



// image creation / loading / sub division
EagleImage* Allegro5GraphicsContext::CreateImage(int width , int height , IMAGE_TYPE type) {
   EagleImage* eagle_image = new Allegro5Image(width , height , type);
   images.Add(eagle_image);
   return eagle_image;
}



EagleImage* Allegro5GraphicsContext::LoadImageFromFile(std::string file , IMAGE_TYPE type) {
   EagleImage* eagle_image = new Allegro5Image(file , type);
   images.Add(eagle_image);
   return eagle_image;
}



EagleImage* Allegro5GraphicsContext::CreateSubImage(EagleImage* parent , int x , int y , int width , int height) {
   EagleImage* eagle_image = new Allegro5Image(parent , x , y , width , height);
   images.Add(eagle_image);
   return eagle_image;
}



EagleFont* Allegro5GraphicsContext::LoadFont(std::string file , int height , int flags , IMAGE_TYPE type) {
   EagleFont* eagle_font = new Allegro5Font(file , height , flags , type);
   fonts.Add(eagle_font);
   
   return eagle_font;
}


/**
void Allegro5GraphicsContext::RegisterDisplayInput(EagleEventHandler* eagle_handler) {
   EAGLE_ASSERT(eagle_handler);
   Allegro5EventHandler* allegro_handler = dynamic_cast<Allegro5EventHandler*>(eagle_handler);
   EAGLE_ASSERT(allegro_handler);
   ALLEGRO_EVENT_QUEUE* allegro_queue = allegro_handler->AllegroQueue();
   if (display) {
      al_register_event_source(allegro_queue , al_get_display_event_source(display));
   }
}
//*/


Transformer* Allegro5GraphicsContext::GetTransformer() {
   return &allegro5transformer;
}



void Allegro5GraphicsContext::MakeDisplayCurrent() {
   ResetBackBuffer();
}



