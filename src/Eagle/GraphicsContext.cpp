
/**
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
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2019+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */



#include "Eagle/GraphicsContext.hpp"
#include "Eagle/Area.hpp"
#include "Eagle/StringWork.hpp"
#include "Eagle/Gui/Text/GlobalText.hpp"
#include "Eagle/System.hpp"

#include <vector>
using std::vector;




unsigned int GUI_TEXT_LINE_SPACING = 3;



const char* eagle_default_font_path = "Verdana.ttf";

int eagle_default_font_size = -16;

int eagle_default_font_flags = LOAD_FONT_MONOCHROME;


/**
REGION_INFO::REGION_INFO() : sx(0.0f) , sy(0.0f) , sw(0.0f) , sh(0.0f) {}



REGION_INFO::REGION_INFO(float srcx , float srcy , float srcw , float srch) :
      sx(srcx) , sy(srcy) , sw(srcw) , sh(srch)
{}



SCALE_INFO::SCALE_INFO() : x(1.0f) , y(1.0f) {}



SCALE_INFO::SCALE_INFO(float xscale , float yscale) : x(xscale) , y(yscale) {}



RESIZE_INFO::RESIZE_INFO() :
      dx(0.0f),
      dy(0.0f),
      dw(0.0f),
      dh(0.0f)
{}



RESIZE_INFO::RESIZE_INFO(float destx , float desty , float destw , float desth) :
      dx(destx),
      dy(desty),
      dw(destw),
      dh(desth)
{}



ROTATE_INFO::ROTATE_INFO() : cx(0.0f) , cy(0.0f) , dx(0.0f) , dy(0.0f) , angle(0.0f) {}



ROTATE_INFO::ROTATE_INFO(float pivotx , float pivoty , float destx , float desty , float theta) :
      cx(pivotx),
      cy(pivoty),
      dx(destx),
      dy(desty),
      angle(theta)
{}


void EagleDrawingInfo::CheckUse() {
   use_any = use_region || use_scale || use_resize || use_rotate || use_tint;
}



EagleDrawingInfo::EagleDrawingInfo() :
      use_any(false),
      use_region(false),
      use_scale(false),
      use_resize(false),
      use_rotate(false),
      use_tint(false),
      dx(0.0f),
      dy(0.0f),
      region(),
      scale(),
      rotate(),
      tint(255,255,255,255),
      flags(0)
{}



EagleDrawingInfo::EagleDrawingInfo(float destx , float desty) :
      use_any(false),
      use_region(false),
      use_scale(false),
      use_resize(false),
      use_rotate(false),
      use_tint(false),
      dx(destx),
      dy(desty),
      region(),
      scale(),
      rotate(),
      tint(255,255,255,255),
      flags(0)
{}



void EagleDrawingInfo::SetDest(float x , float y) {
   dx = x;
   dy = y;
   use_scale = false;
   use_resize = false;
   use_rotate = false;
   CheckUse();
}



void EagleDrawingInfo::SetRegion(REGION_INFO r)   {
   use_region = true;
   use_any = true;
   region = r;
}



void EagleDrawingInfo::SetScale(SCALE_INFO s)     {
   use_scale = true;
   use_resize = false;
   use_any = true;
   scale = s;
}



void EagleDrawingInfo::SetResize(RESIZE_INFO r)   {
   use_resize = true;
   use_scale = false;
   use_rotate = false;
   use_any = true;
   resize = r;
}



void EagleDrawingInfo::SetRotate(ROTATE_INFO r)   {
   use_rotate = true;
   use_resize = false;
   use_any = true;
   rotate = r;
}



void EagleDrawingInfo::SetTintColor(EagleColor c) {
     use_tint = true;
     use_any = true;
     tint   = c;
}



void EagleDrawingInfo::SetFlags(int f) {
   flags = f;
}



void EagleDrawingInfo::ClearSettings() {
   use_any = false;
   use_region = false;
   use_scale = false;
   use_resize = false;
   use_rotate = false;
   use_tint = false;
}
//*/



/// ------------------------------     EagleGraphicsContext     -------------------------------------------



EagleGraphicsContext::EagleGraphicsContext(std::string objclass , std::string objname) :
      EagleObject(objclass , objname),
      EagleEventSource(),
      scrw(0),
      scrh(0),
      backbuffer(0),
      drawing_target(0),
      imageset(),
      fontset(),
      mp_manager(0),
      maxframes(60),
      numframes(0.0f),
      total_frame_time(0.0f),
      frame_times(),
      previoustime(0.0f),
      currenttime(0.0f),
      default_font(),
      default_font_path(eagle_default_font_path),
      default_font_size(eagle_default_font_size),
      default_font_flags(eagle_default_font_flags),
      our_thread(0),
      window_mutex(0)
{
   /// NOTE: derived class needs to instantiate mp_manager
   /// NOTE : derived class needs to create default font inside DerivedGraphicsContext::Create
   /// NOTE : derived class needs to instantiate window_mutex
}



bool EagleGraphicsContext::StartDrawing(EagleThread* draw_thread) {
   EAGLE_ASSERT(window_mutex && window_mutex->Valid());
   return ThreadTryLockMutex(draw_thread , window_mutex);
}



void EagleGraphicsContext::CompleteDrawing(EagleThread* draw_thread) {
   ThreadUnLockMutex(draw_thread , window_mutex);
}



float EagleGraphicsContext::GetFPS() {
   if (total_frame_time <= 0.0f) {
      return 0.0f;
   }
   return numframes/total_frame_time;
}



void EagleGraphicsContext::DrawRectangle(Rectangle r , int thickness , EagleColor c) {
   DrawRectangle(r.X() , r.Y() , r.W() , r.H() , thickness , c);
}



void EagleGraphicsContext::DrawFilledRectangle(Rectangle r , EagleColor c) {
   DrawFilledRectangle(r.X() , r.Y() , r.W() , r.H() , c);
}



void EagleGraphicsContext::DrawRoundedRectangle(Rectangle r , int rx , int ry , EagleColor c) {
   DrawRoundedRectangle(r.X() , r.Y() , r.W() , r.H() , rx , ry , c);
}



void EagleGraphicsContext::DrawFilledRoundedRectangle(Rectangle r , int rx , int ry , EagleColor c) {
   DrawFilledRoundedRectangle(r.X() , r.Y() , r.W() , r.H() , rx , ry , c);
}



void EagleGraphicsContext::DrawFilledQuarterEllipse(Rectangle r , QUADRANT_DIR dir , EagleColor c) {
   EagleImage* target = GetDrawingTarget();
   target->PushClippingRectangle(r);
   Pos2I corners[4] = {
      Pos2I(r.X()   , r.BRY()), 
      Pos2I(r.BRX() , r.BRY()), 
      Pos2I(r.BRX() , r.Y()  ),
      Pos2I(r.X()   , r.Y()  ) 
   };
   DrawFilledQuarterEllipse(Rectangle(corners[dir].X() , corners[dir].Y() , r.Width() , r.Height()) , dir , c);
   target->PopClippingRectangle();
}



void EagleGraphicsContext::Draw(EagleImage* img , float x , float y , HALIGNMENT halign , VALIGNMENT valign , int flags) {
   if (halign == HALIGN_CENTER) {
      x -= img->W()/2.0f;
   }
   else if (halign == HALIGN_RIGHT) {
      x -= img->W();
   }
   if (valign == VALIGN_CENTER) {
      y -= img->H()/2.0f;
   }
   else if (valign == VALIGN_BOTTOM) {
      y -= img->H();
   }
   Draw(img , x , y , flags);
}



void EagleGraphicsContext::DrawStretchedRegion(EagleImage* img , Rectangle src , Rectangle dest , int flags) {
   DrawStretchedRegion(img , src.X() , src.Y() , src.W() , src.H() , dest.X() , dest.Y() , dest.W() , dest.H() , flags);
}



void EagleGraphicsContext::DrawMultiLineTextString(EagleFont* font , std::string str , float x , float y , EagleColor c , float line_spacing ,
                            HALIGNMENT halign , VALIGNMENT valign) {
   EAGLE_ASSERT(font);
   EAGLE_ASSERT(font->Valid());

   int lineheight = font->Height();
   int nlines = 0;
   std::vector<std::string> lines;
   std::vector<int> linewidths;
   int maxwidth = 0;
   int totalheight = 0;
   GetTextAttributes(str , font , line_spacing , &nlines , &lines , &linewidths , &maxwidth , &totalheight);


   if (valign == VALIGN_CENTER) {
      y -= totalheight/2;
   }
   else if (valign == VALIGN_BOTTOM) {
      y -= totalheight;
   }
   int ly = y;
   for (int i = 0 ; i < nlines ; ++i) {
      int lx = x;
      if (halign == HALIGN_CENTER) {
         lx -= linewidths[i]/2;
      }
      else if (halign == HALIGN_RIGHT) {
         lx -= linewidths[i];
      }
      DrawTextString(font , lines[i].c_str() , lx , ly , c , HALIGN_LEFT , VALIGN_TOP);
      ly += lineheight + line_spacing;
   }
}



void EagleGraphicsContext::DrawGuiTextString(EagleFont* font , std::string str , float x , float y , EagleColor c ,
                       HALIGNMENT halign ,VALIGNMENT valign) {

  EAGLE_ASSERT(font);
  EAGLE_ASSERT(font->Valid());

  std::string text = GetGuiText(str);
  std::string underlinetext = GetGuiUnderlineText(str);

  int height = font->Height();
  int vspace = height/10;///GUI_TEXT_LINE_SPACING + 1;

  DrawTextString(font , text , x , y , c , halign , valign);
  DrawTextString(font , underlinetext , x , y + vspace, c , halign , valign);
}



void EagleGraphicsContext::FlipDisplay() {

   previoustime = currenttime;
   currenttime = GetSystem()->GetProgramTime();
   float deltatime = currenttime - previoustime;
   total_frame_time += deltatime;

   numframes++;
   frame_times.push_back(deltatime);
   if (numframes > maxframes) {
      numframes = maxframes;
      float oldtime = frame_times.front();
      total_frame_time -= oldtime;
      frame_times.pop_front();
   }

   PrivateFlipDisplay();

}



void EagleGraphicsContext::DrawToBackBuffer() {
   SetDrawingTarget(GetBackBuffer());
}



void EagleGraphicsContext::FreeImage(EagleImage* img) {
      
   ISIT it = imageset.find(img);
   if (it != imageset.end()) {
      delete img;
      imageset.erase(it);
      return;
   }
   
   /// If we get here, we don't own this image, what should we do?
   /// If it is a sub bitmap, when the parent is destroyed, it will be destroyed, should we bother?
   /// This image might have been freed already
   /// Best bet is to do nothing. There won't be any memory leak, just a memory waste until its used
   
/** Not necessary
   if (!img) {return;}
   EagleImage* parent = img->Parent();
   if (parent) {
      /// This is a sub bitmap, we don't own it
      parent->FreeChild(img);
      return;
   }
*/
}



void EagleGraphicsContext::FreeAllImages() {
   IMAGESET iset = imageset;
   for (ISIT it = iset.begin() ; it != iset.end() ; ++it) {
      FreeImage(*it);
   }
   EAGLE_ASSERT(imageset.empty());
}



void EagleGraphicsContext::FreeFont(EagleFont* font) {
   if (!font) {return;}
   FSIT it = fontset.find(font);
   if (it != fontset.end()) {
      delete font;
      fontset.erase(it);
   }
}



void EagleGraphicsContext::FreeAllFonts() {
   FONTSET fset = fontset;
   for (FSIT it = fset.begin() ; it != fset.end() ; ++it) {
      FreeFont(*it);
   }
   EAGLE_ASSERT(fontset.empty());
}



EagleFont* EagleGraphicsContext::DefaultFont() {
   return default_font;
}



string EagleGraphicsContext::DefaultFontPath() {
   return default_font_path;
}



int EagleGraphicsContext::DefaultFontSize() {
   return default_font_size;
}



int EagleGraphicsContext::DefaultFontFlags() {
   return default_font_flags;
}



/// Forwarding functions to access MousePointerManager mp_manager



void EagleGraphicsContext::ShowMouse() {
   EAGLE_ASSERT(mp_manager);
   mp_manager->ShowMouse();
}



void EagleGraphicsContext::HideMouse() {
   EAGLE_ASSERT(mp_manager);
   mp_manager->HideMouse();
}



bool EagleGraphicsContext::AcquireMousePointer(void* caller , MOUSE_POINTER_TYPE ptype , bool use_system_pointer) {
   EAGLE_ASSERT(mp_manager);
   return mp_manager->AcquireMousePointer(caller , ptype , use_system_pointer);
}



void EagleGraphicsContext::ReleaseMousePointer(const void* caller) {
   EAGLE_ASSERT(mp_manager);
   mp_manager->ReleaseMousePointer(caller);
}



void EagleGraphicsContext::ReAcquireMouse() {
   EAGLE_ASSERT(mp_manager);
   mp_manager->ReAcquireMouse();
}



bool EagleGraphicsContext::SetCustomPointer(MousePointerInfo info) {
   EAGLE_ASSERT(mp_manager);
   return mp_manager->SetCustomPointer(info);
}

//   void SetCustomPointerSet(MousePointerSet* pointer_set);

void EagleGraphicsContext::SetMousePosition(int mousex , int mousey) {
   EAGLE_ASSERT(mp_manager);
   mp_manager->SetMousePosition(mousex , mousey);
}


// Drawing targets


void EagleGraphicsContext::PushDrawingTarget(EagleImage* img) {
   EAGLE_ASSERT(img);

   if (img != drawing_target) {
      SetDrawingTarget(img);
   }

   draw_target_stack.push_back(img);
}



void EagleGraphicsContext::PopDrawingTarget() {

   if (!draw_target_stack.empty()) {
      draw_target_stack.pop_back();
   }

   EagleImage* back = GetBackBuffer();
   if (!draw_target_stack.empty()) {
      back = draw_target_stack.back();
   }
   if (drawing_target != back) {
      SetDrawingTarget(back);
   }
}



/// ------------------------     Drawing Targeter     ----------------------------------------



DrawingTargeter::DrawingTargeter(EagleGraphicsContext* window , EagleImage* target_image) :
      win(window)
{
   EAGLE_ASSERT(window);
   EAGLE_ASSERT(target_image);
   win->PushDrawingTarget(target_image);
}



DrawingTargeter::~DrawingTargeter() {
   win->PopDrawingTarget();
}






