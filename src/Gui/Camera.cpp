
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
 *    Copyright 2009-2013+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */



#include <cmath>

#include "Eagle/Gui/Camera.hpp"

#include "Eagle/Image.hpp"
#include "Eagle/InputHandler.hpp"



using std::string;


const unsigned int TOPIC_CAMERA = NextFreeTopicId();

REGISTERED_WIDGET_MESSAGE(TOPIC_CAMERA , CAMERA_VIEW_MOVED);
REGISTERED_WIDGET_MESSAGE(TOPIC_CAMERA , CAMERA_VIEW_DEST_REACHED);



const unsigned int TOPIC_ZOOM_CAMERA = NextFreeTopicId();

REGISTERED_WIDGET_MESSAGE(TOPIC_ZOOM_CAMERA , CAMERA_ZOOM_CHANGED);
REGISTERED_WIDGET_MESSAGE(TOPIC_ZOOM_CAMERA , CAMERA_ZOOM_DEST_REACHED);




void Camera::SetDestination(int xpos , int ypos) {
   EAGLE_ASSERT(view_bmp);
   
   // Image boundary checks
   if (xpos < 0) {xpos = 0;}
   if (ypos < 0) {ypos = 0;}
   if (xpos + view_area.W() > view_bmp->W()) {
      xpos = view_bmp->W() - view_area.W();
   }
   if (ypos + view_area.H() > view_bmp->H()) {
      ypos = view_bmp->H() - view_area.H();
   }
   dest_x = xpos;
   dest_y = ypos;
}



void Camera::SetViewPos(int xpos , int ypos) {
   view_area.SetPos(xpos,ypos);
   RaiseEvent(WidgetMsg(this , TOPIC_CAMERA , CAMERA_VIEW_MOVED));
   SetRedrawFlag();
}



void Camera::PerformMove() {
   if (move_time > 0.0) {
      if (time_since_prev_pos >= move_time) {
         SetViewPos(dest_x , dest_y);
         RaiseEvent(WidgetMsg(this , TOPIC_CAMERA , CAMERA_VIEW_DEST_REACHED));
         move_time = 0.0;
      } else {
         const double t = time_since_prev_pos;
         double curr_x = prev_x + xvel*t + (xacc/2.0)*(t*t);
         double curr_y = prev_y + yvel*t + (yacc/2.0)*(t*t);
         SetViewPos((int)curr_x , (int)curr_y);
//         RaiseEvent(WidgetMsg(this , TOPIC_CAMERA , CAMERA_VIEW_MOVED));// SetViewPos sends CAMERA_VIEW_MOVED already
      }
   }
}



Camera::Camera(string name) :
      WidgetBase(name),
      view_bmp(0),
      view_area(),
      prev_x(0.0),
      prev_y(0.0),
      dest_x(0),
      dest_y(0),
      xvel(0.0),
      yvel(0.0),
      xacc(0.0),
      yacc(0.0),
      time_since_prev_pos(0.0),
      move_time(0.0),
      startmx(0),
      startmy(0),
      startxpos(0),
      startypos(0),
      drag(false),
      mmb_drag_enabled(true),
      take_focus(true)
{
   
}




void Camera::PrivateDisplay(EagleGraphicsContext* win , int x , int y) {
   EAGLE_ASSERT(win);
   
   if (view_bmp && (flags & VISIBLE)) {
      Rectangle r = area.InnerArea();
      r.MoveBy(x,y);
      Clipper(win->GetDrawingTarget() , r);
   
      // center the viewed area on the display area
      int ox = (r.W() - view_area.W())/2;
      int oy = (r.H() - view_area.H())/2;
      
//      EagleLog() << "View area = " << view_area << std::endl;
      
      win->DrawRegion(view_bmp , view_area , r.X() + ox , r.Y() + oy);
   }
   ClearRedrawFlag();
}



int Camera::CheckInputs() {
   int msx = mouse_x - AbsParentX();
   int msy = mouse_y - AbsParentY();
   if (flags & ENABLED) {
      if (area.OuterArea().Contains(msx,msy)) {
         if (input_mouse_press(LMB)) {
            if (take_focus) {// workaround for WidgetHandler so it can take the focus instead
               return DIALOG_TAKE_FOCUS | DIALOG_INPUT_USED;
            }
         }
         if (mmb_drag_enabled) {
            if (input_mouse_press(MMB)) {
               drag = true;
               startmx = mouse_x;
               startmy = mouse_y;
               startxpos = view_area.X();
               startypos = view_area.Y();
               return DIALOG_TAKE_FOCUS | DIALOG_INPUT_USED;
            }
         }
      }
      if (drag && input_mouse_held(MMB)) {
         int dx = mouse_x - startmx;
         int dy = mouse_y - startmy;
         SetDestination(startxpos - dx , startypos - dy);
         SetViewPos(dest_x , dest_y);
         return DIALOG_INPUT_USED;
      }
      if (input_mouse_release(MMB)) {
         drag = false;
      }
      
   }
   return DIALOG_OKAY;
}



int Camera::Update (double tsec) {
   if (tsec < 0.0) {tsec = 0.0;}
   time_since_prev_pos += tsec;
   PerformMove();
   return DIALOG_OKAY;
}



/// Don't remove these two function - they serve to prevent redraw on color change
void Camera::SetColorset(const WidgetColorset& colors , bool set_descendants_colors) {
   wcols = colors;
   (void)set_descendants_colors;
   // purposely don't set redraw flag
   return;
}



/// Don't remove these two function - they serve to prevent redraw on color change
void Camera::SetPrivateColorset(const WidgetColorset& colors) {
   privwcols = colors;
   // purposely don't set redraw flag
   return;
}



void Camera::SetView(EagleImage* bmp , Rectangle area_to_view) {
   SetView(bmp , area_to_view.X() , area_to_view.Y() , area_to_view.W() , area_to_view.H());
}



void Camera::SetViewArea(Rectangle area_to_view) {
   SetViewArea(area_to_view.X() , area_to_view.Y() , area_to_view.W() , area_to_view.H());
}



void Camera::SetView(EagleImage* bmp , int x , int y , int w , int h) {
   EAGLE_ASSERT(bmp);
   view_bmp = bmp;
   SetViewArea(x , y , w , h);
}



void Camera::SetViewArea(int x , int y , int w , int h) {
   EAGLE_ASSERT(view_bmp);
   if (w < 1) {w = 1;}
   if (h < 1) {h = 1;}
   if (w > view_bmp->W()) {w = view_bmp->W();}
   if (h > view_bmp->H()) {h = view_bmp->H();}
   view_area.SetDimensions(w,h);
   SetDestination(x,y);
   SetViewPos(dest_x , dest_y);
   SetBgRedrawFlag();
}



void Camera::AccMoveViewTlxTo(int xpos , int ypos , double time) {
   EAGLE_ASSERT(view_bmp);
   SetDestination(xpos,ypos);
   prev_x = (double)view_area.X();
   prev_y = (double)view_area.Y();
   if (time <= 0.0) {
      SetViewPos(dest_x,dest_y);
      move_time = 0.0;
      RaiseEvent(WidgetMsg(this , TOPIC_CAMERA , CAMERA_VIEW_DEST_REACHED));
      return;
   }
   move_time = time;
   double deltax = (double)dest_x - prev_x;
   double deltay = (double)dest_y - prev_y;
   xvel = 2.0*deltax/move_time;
   yvel = 2.0*deltay/move_time;
   xacc = -2.0*deltax/(move_time*move_time);
   yacc = -2.0*deltay/(move_time*move_time);
   time_since_prev_pos = 0.0;
}



void Camera::AccMoveViewCenterTo(int xpos , int ypos , double time) {
   AccMoveViewTlxTo(xpos - view_area.W()/2 , ypos - view_area.H()/2 , time);
}



void Camera::AccMoveViewBy(int dx , int dy , double time) {
   AccMoveViewTlxTo(view_area.X() + dx , view_area.Y() + dy , time);
}



void Camera::MoveViewTlxTo(int xpos , int ypos , double time) {
   EAGLE_ASSERT(view_bmp);
   SetDestination(xpos,ypos);
   prev_x = (double)view_area.X();
   prev_y = (double)view_area.Y();
   if (time <= 0.0) {
      SetViewPos(dest_x,dest_y);
      move_time = 0.0;
      RaiseEvent(WidgetMsg(this , TOPIC_CAMERA , CAMERA_VIEW_DEST_REACHED));
      return;
   }
   move_time = time;
   xvel = ((double)dest_x - prev_x)/move_time;
   yvel = ((double)dest_y - prev_y)/move_time;
   xacc = 0.0;
   yacc = 0.0;
   time_since_prev_pos = 0.0;
}



void Camera::MoveViewCenterTo(int xpos , int ypos , double time) {
   MoveViewTlxTo(xpos - view_area.W()/2 , ypos - view_area.H()/2 , time);
}



void Camera::MoveViewBy(int dx , int dy , double time) {
   MoveViewTlxTo(view_area.X() + dx , view_area.Y() + dy , time);
}



void Camera::AllowMiddleMouseButtonDrag(bool allow) {
   mmb_drag_enabled = allow;
}



void Camera::TakesFocus(bool click_takes_focus) {
   take_focus = click_takes_focus;
}



/// ------------------------------------- SBCamera class --------------------------------------------


/**
void SBCamera::SetViewPos(int xpos , int ypos) {
   Camera::SetViewPos(xpos,ypos);
   Free();
   sbcam = create_sub_bitmap(view_bmp , view_area.X() , view_area.Y() , view_area.W() , view_area.H());
   if (draw_type == SOLID) {
      SetRedrawFlag();
   } else {
      SetBgRedrawFlag();
   }
}



SBCamera::SBCamera(string name , BMP_DRAW_TYPE bmp_draw_type) :
      Camera(name),
      sbcam(0),
      draw_type(bmp_draw_type)
{}



SBCamera::SBCamera(string name , BITMAP* view , BMP_DRAW_TYPE bmp_draw_type , Rectangle area_to_view ,
                     Rectangle display_area , UINT wflags) :
      Camera(name , bmp_draw_type , view , area_to_view , display_area , wflags),
      sbcam(0),
      draw_type(bmp_draw_type)
{
   SetDestination(area_to_view.X() , area_to_view.Y());
   SetViewPos(dest_x , dest_y);
}



void SBCamera::Free() {
   if (sbcam) {
      destroy_bitmap(sbcam);
   }
   sbcam = 0;
}



void SBCamera::DisplayOn(BITMAP* bmp , int x , int y) {
   ASSERT(bmp);
   if (flags & VISIBLE) {
      Rectangle r = area;
      r.MoveBy(x,y);
      Rectangle old_clip = GetClippingRectangle(bmp);
      r.SetClipRect(bmp);
      // center the viewed area on the display area
      int ox = (area.W() - view_area.W())/2;
      int oy = (area.H() - view_area.H())/2;
      switch (draw_type) {
         case SOLID :
   //         blit(view_bmp , bmp , view_area.X() , view_area.Y() , r.X() + ox , r.Y() + oy , view_area.W() , view_area.H());
            blit(sbcam , bmp , 0 , 0 , r.X() + ox , r.Y() + oy , sbcam->w , sbcam->h);
            break;
         case TRANSPARENT :
            draw_sprite(bmp , sbcam , r.X() + ox , r.Y() + oy);
            break;
         case TRANSLUCENT :
            set_alpha_blender();
            draw_trans_sprite(bmp , sbcam , r.X() + ox , r.Y() + oy);
            break;
      }
      old_clip.SetClipRect(bmp);
   }
   ClearRedrawFlag();
}



void SBCamera::SetDrawType(BMP_DRAW_TYPE bmp_draw_type) {
   draw_type = bmp_draw_type;
}



/// -------------------------------------- ZoomCamera class --------------------------------------------



void ZoomCamera::SetDestinationDimensions(int width , int height) {
   if (width < min_zoom_w) {width = min_zoom_w;}
   if (height < min_zoom_h) {height = min_zoom_h;}
   if (width > view_bmp->w) {width = view_bmp->w;}
   if (height > view_bmp->h) {height = view_bmp->h;}
   dest_w = width;
   dest_h = height;
}



void ZoomCamera::SetViewPos(int xpos , int ypos) {
   if (xpos < 0) {xpos = 0;}
   if (ypos < 0) {ypos = 0;}
   if (xpos > (view_bmp->w - view_area.W())) {
      xpos = view_bmp->w - view_area.W();
   }
   if (ypos > (view_bmp->h - view_area.H())) {
      ypos = view_bmp->h - view_area.H();
   }
   Camera::SetViewPos(xpos,ypos);
   vcx = view_area.CX();
   vcy = view_area.CY();
   SetRedrawFlag();
   drawn = false;
}



void ZoomCamera::SetViewDimensions(int width , int height) {
   view_area.SetDimensions(width,height);

//   double zoom_scale_w = (double)view_bmp->w/(double)view_area.W();
//   double zoom_scale_h = (double)view_bmp->h/(double)view_area.H();
//   zoom_scale = (zoom_scale_w < zoom_scale_h)?zoom_scale_w:zoom_scale_h;
//   zoom_value_100ths = (int)(100.0*(log10(zoom_scale)/log10(2.0)) + 0.5);

   SetRedrawFlag();
   drawn = false;
}



void ZoomCamera::PerformZoom() {
   if (zoom_time > 0.0) {
      if (time_since_prev_zoom >= zoom_time) {
         SetViewDimensions(dest_w,dest_h);
         SetDestination(dvcx - dest_w/2 , dvcy - dest_h/2);
         SetViewPos(dest_x,dest_y);
         zoom_time = 0.0;
         RaiseEvent(WidgetMsg(this , TOPIC_ZOOM_CAMERA , CAMERA_ZOOM_DEST_REACHED));
      } else {
         const double t = time_since_prev_zoom;
         double curr_w = prev_w + wvel*t + (wacc/2.0)*(t*t);
         double curr_h = prev_h + hvel*t + (hacc/2.0)*(t*t);
         int cw = (int)curr_w;
         int ch = (int)curr_h;
         SetViewDimensions(cw,ch);
         SetDestination(dvcx - cw/2 , dvcy - ch/2);
         SetViewPos(dest_x,dest_y);
         RaiseEvent(WidgetMsg(this , TOPIC_ZOOM_CAMERA , CAMERA_ZOOM_CHANGED));
      }
      ResetDisplayArea();
   }
}



void ZoomCamera::ResetBuffer() {
   if (!buffer || (buffer.W() != area.W()) || (buffer.H() != area.H())) {
      buffer.Resize(area.W() , area.H() , MEMORY , SOLID);
   }
   drawn = false;
}



void ZoomCamera::ResetDisplayArea() {
   if (zoom_scale >= 1.0) {
      display_area.SetDimensions(area.W() , area.H());
      display_area.SetPos(0,0);
   } else if (zoom_scale > 0.0) {
      // zoom_scale*area = displayed
      int width = (int)((double)area.W()*zoom_scale + 0.5);
      int height = (int)((double)area.H()*zoom_scale + 0.5);
      if (width < 1) {width = 1;}
      if (height < 1) {height = 1;}
      display_area.SetDimensions(width , height);
      display_area.SetPos((area.W() - width)/2 , (area.H() - height)/2);
      
      SetBgRedrawFlag();
   } else {
      // zoom scale should never be less than or equal to zero...
      ASSERT(0);
   }
   drawn = false;
}




ZoomCamera::ZoomCamera(string name , STRETCH_TYPE cam_stretch_type , BITMAP* view , Rectangle area_to_view ,
                        Rectangle position , UINT wflags) :
///      Camera(name),
      Camera(name , SOLID , view , area_to_view , position , wflags),
      vcx(0),vcy(0),
      dvcx(0),dvcy(0),
      display_area(),
      prev_w(0.0),
      prev_h(0.0),
      dest_w(0),
      dest_h(0),
      wvel(0.0),
      hvel(0.0),
      wacc(0.0),
      hacc(0.0),
      time_since_prev_zoom(0.0),
      zoom_time(0.0),
      stretch_type(cam_stretch_type),
      min_zoom_w(1),
      min_zoom_h(1),
      zoom_value_100ths(0),
      zoom_rate_100ths(20),
      zoom_scale(1.0),
      min_zoom_value_100ths(-500),
      max_zoom_value_100ths(500),
      min_zoom_scale(1.0/32.0),
      max_zoom_scale(32.0),
      buffer(),
      drawn(false)
{
   SetView(view , area_to_view);

//   vcx = view_area.CX();
//   vcy = view_area.CY();
//   dvcx = vcx;
//   dvcy = vcy;
//   double zoom_scale_w = (double)view_bmp->w/(double)view_area.W();
//   double zoom_scale_h = (double)view_bmp->h/(double)view_area.H();
//   zoom_scale = (zoom_scale_w < zoom_scale_h)?zoom_scale_w:zoom_scale_h;
//   zoom_value_100ths = (int)(100.0*(log10(zoom_scale)/log10(2.0)) + 0.5);

   ResetBuffer();
   ResetDisplayArea();
}



WidgetMsg ZoomCamera::Update (double tsec) {
   if (tsec < 0.0) {tsec = 0.0;}
   time_since_prev_zoom += tsec;
   PerformZoom();
   Camera::Update(tsec);
   return WidgetMsg(this , TOPIC_DIALOG , DIALOG_OKAY);
}



WidgetMsg ZoomCamera::CheckInputs(int msx , int msy) {
   int ret = DIALOG_OKAY;
   if (flags & ENABLED) {
      if (area.Contains(msx,msy)) {
         if (input_mouse_press(MMB)) {
            drag = true;
            startmx = mx;
            startmy = my;
            startxpos = view_area.X();
            startypos = view_area.Y();
            return WidgetMsg(this , TOPIC_DIALOG , DIALOG_TAKE_FOCUS | DIALOG_INPUT_USED);
         }
         if (ms_zdiff) {
            ChangeZoomValueBy(ms_zdiff*zoom_rate_100ths);
            ret |= DIALOG_INPUT_USED;
         }
         int cx = msx - area.X();
         int cy = msy - area.Y();
         if (display_area.Contains(cx,cy)) {
            int dx = cx - display_area.X();
            int dy = cy - display_area.Y();
            int vx = (int)((double)dx*((double)view_area.W()/(double)display_area.W()) + 0.5);
            int vy = (int)((double)dy*((double)view_area.H()/(double)display_area.H()) + 0.5);
            vx += view_area.X();
            vy += view_area.Y();
            if (input_mouse_press(LMB) || mb_held_for(LMB , 0.5)) {
               ChangeZoomValueBy(zoom_rate_100ths);
               SetZoomCenter(vx,vy);
               ret |= DIALOG_TAKE_FOCUS | DIALOG_INPUT_USED | DIALOG_REDRAW_ME;
            }
            if (input_mouse_press(RMB) || mb_held_for(RMB , 0.5)) {
               ChangeZoomValueBy(-zoom_rate_100ths);
               SetZoomCenter(vx,vy);
               ret |= DIALOG_TAKE_FOCUS | DIALOG_INPUT_USED | DIALOG_REDRAW_ME;
            }
         }
      }
      if (drag && input_mouse_held(MMB)) {
         int dx = mx - startmx;
         int dy = my - startmy;
         SetDestination(startxpos - dx , startypos - dy);
         SetViewPos(dest_x , dest_y);
         dvcx = vcx;
         dvcy = vcy;
         return WidgetMsg(this , TOPIC_DIALOG , DIALOG_INPUT_USED);
      }
      if (input_mouse_release(MMB)) {
         drag = false;
      }
   }
   return WidgetMsg(this , TOPIC_DIALOG , ret);
}



void ZoomCamera::DisplayOn(BITMAP* bmp , int x , int y) {
   ASSERT(is_memory_bitmap(view_bmp));
///   ASSERT(is_memory_bitmap(view_bmp) ||
///            (is_system_bitmap(view_bmp) && (gfx_capabilities & GFX_HW_SYS_STRETCH_BLIT) && is_video_bitmap(buffer)) ||
///            (is_video_bitmap(view_bmp) && (gfx_capabilities & GFX_HW_VRAM_STRETCH_BLIT) && is_video_bitmap(buffer)));
//   ASSERT(is_memory_bitmap(view_bmp) ||
//            (is_system_bitmap(view_bmp) && (gfx_capabilities & GFX_HW_SYS_STRETCH_BLIT) && is_video_bitmap(bmp)) ||
//            (is_video_bitmap(view_bmp) && (gfx_capabilities & GFX_HW_VRAM_STRETCH_BLIT) && is_video_bitmap(bmp)));
   ASSERT(bmp);
   if (flags & VISIBLE) {
      if (!drawn) {
         if (view_bmp && buffer) {
            switch (stretch_type) {
               case USE_ALLEGRO_STRETCH :
                  stretch_blit(view_bmp , buffer , view_area.X() , view_area.Y() , view_area.W() , view_area.H() , 0 , 0 , display_area.W() , display_area.H());
                  break;
               case USE_SMOOTH_STRETCH :
                  stretch_blit_smooth(view_bmp , buffer , view_area.X() , view_area.Y() , view_area.W() , view_area.H() , 0 , 0 , display_area.W() , display_area.H());
                  break;
            }
         }
         drawn = true;
      }
      if (buffer) {
         Rectangle r = area;
         r.MoveBy(display_area.X() , display_area.Y());
         blit(buffer , bmp , 0 , 0 , r.X() , r.Y() , display_area.W() , display_area.H());
      }

//      Rectangle r = display_area;
//      r.MoveBy(area.X() + x , area.Y() + y);
//      if ((view_area.W() == display_area.W()) && (view_area.H() == display_area.H())) {
//         blit(view_bmp , bmp , view_area.X() , view_area.Y() , r.X() , r.Y() , r.W() , r.H());
//      } else {
//         switch (stretch_type) {
//            case USE_ALLEGRO_STRETCH :
//               stretch_blit(view_bmp , bmp , view_area.X() , view_area.Y() , view_area.W() , view_area.H() , r.X() , r.Y() , r.W() , r.H());
//               break;
//            case USE_SMOOTH_STRETCH :
//               stretch_blit_smooth(view_bmp , bmp , view_area.X() , view_area.Y() , view_area.W() , view_area.H() , r.X() , r.Y() , r.W() , r.H());
//               break;
//         }
//      }

   }
   ClearRedrawFlag();
}



void ZoomCamera::SetDrawPos(int xpos , int ypos) {
   Camera::SetDrawPos(xpos,ypos);
}



void ZoomCamera::SetDrawDimensions(int width , int height) {
   Camera::SetDrawDimensions(width,height);
   ResetBuffer();
   ResetDisplayArea();
}



void ZoomCamera::SetArea(int xpos , int ypos , int width , int height) {
   Camera::SetArea(xpos,ypos,width,height);
   ResetBuffer();
   ResetDisplayArea();
}



void ZoomCamera::SetArea(const Rectangle& r) {
   Camera::SetArea(r);
   ResetBuffer();
   ResetDisplayArea();
}



void ZoomCamera::SetView(BITMAP* bmp , Rectangle area_to_view) {
   Camera::SetView(bmp , area_to_view);
   if ((!buffer) || (bitmap_color_depth(bmp) != bitmap_color_depth(buffer)) ||
       (buffer.W() != area.W()) || (buffer.H() != area.H())) {
      buffer.Resize(bitmap_color_depth(bmp) , area.W() , area.H() , MEMORY , SOLID);
   }
   drawn = false;
}



void ZoomCamera::SetViewArea(Rectangle area_to_view) {
   Camera::SetViewArea(area_to_view);

   vcx = view_area.CX();
   vcy = view_area.CY();
   dvcx = vcx;
   dvcy = vcy;
   double zoom_scale_w = (double)view_bmp->w/(double)view_area.W();
   double zoom_scale_h = (double)view_bmp->h/(double)view_area.H();
   zoom_scale = (zoom_scale_w < zoom_scale_h)?zoom_scale_w:zoom_scale_h;
   zoom_value_100ths = (int)(100.0*(log10(zoom_scale)/log10(2.0)) + 0.5);
   
   SetRedrawFlag();
   drawn = false;
}



void ZoomCamera::AccZoomTo(int width , int height , double time , int center_x , int center_y) {
   ASSERT(view_bmp);
   SetDestinationDimensions(width,height);
   prev_w = (double)view_area.W();
   prev_h = (double)view_area.H();
   if (center_x == -1) {
      dvcx = vcx;
   } else {
      dvcy = center_x;
   }
   if (center_y == -1) {
      dvcy = vcy;
   } else {
      dvcy = center_y;
   }
   if (time <= 0.0) {
      SetViewDimensions(dest_w,dest_h);
      SetDestination(dvcx - dest_w/2 , dvcy - dest_h/2);
      SetViewPos(dest_x,dest_y);
      ResetDisplayArea();
      zoom_time = 0.0;
      RaiseEvent(WidgetMsg(this , TOPIC_ZOOM_CAMERA , CAMERA_ZOOM_DEST_REACHED));
      return;
   }
   zoom_time = time;
   double deltaw = (double)dest_w - prev_w;
   double deltah = (double)dest_h - prev_h;
   wvel = 2.0*deltaw/zoom_time;
   hvel = 2.0*deltah/zoom_time;
   wacc = -2.0*deltaw/(zoom_time*zoom_time);
   hacc = -2.0*deltah/(zoom_time*zoom_time);
   time_since_prev_zoom = 0.0;
}



void ZoomCamera::AccZoomBy(int dw , int dh , double time , int center_x , int center_y) {
   AccZoomTo(view_area.W() + dw , view_area.H() + dh , time , center_x , center_y);
}



void ZoomCamera::MoveZoomTo(int width , int height , double time , int center_x , int center_y) {
   ASSERT(view_bmp);
   SetDestinationDimensions(width,height);
   prev_w = (double)view_area.W();
   prev_h = (double)view_area.H();
   if (center_x == -1) {
      dvcx = vcx;
   } else {
      dvcy = center_x;
   }
   if (center_y == -1) {
      dvcy = vcy;
   } else {
      dvcy = center_y;
   }
   if (time <= 0.0) {
      SetViewDimensions(dest_w,dest_h);
      SetDestination(dvcx - dest_w/2 , dvcy - dest_h/2);
      SetViewPos(dest_x,dest_y);
      ResetDisplayArea();
      zoom_time = 0.0;
      RaiseEvent(WidgetMsg(this , TOPIC_ZOOM_CAMERA , CAMERA_ZOOM_DEST_REACHED));
      return;
   }
   zoom_time = time;
   double deltaw = (double)dest_w - prev_w;
   double deltah = (double)dest_h - prev_h;
   wvel = deltaw/zoom_time;
   hvel = deltah/zoom_time;
   wacc = 0.0;
   hacc = 0.0;
   time_since_prev_zoom = 0.0;
}



void ZoomCamera::MoveZoomBy(int dw , int dh , double time , int center_x , int center_y) {
   MoveZoomTo(view_area.W() + dw , view_area.H() + dh , time , center_x , center_y);
}



void ZoomCamera::SetZoomCenter(int center_x , int center_y) {
   if (center_x == -1) {
      dvcx = vcx;
   } else {
      dvcx = center_x;
   }
   if (center_y == -1) {
      dvcy = vcy;
   } else {
      dvcy = center_y;
   }
   SetDestination(dvcx - view_area.W()/2 , dvcy - view_area.H()/2);
   SetViewPos(dest_x,dest_y);
   zoom_time = 0.0;
   RaiseEvent(WidgetMsg(this , TOPIC_ZOOM_CAMERA , CAMERA_ZOOM_DEST_REACHED));
}



void ZoomCamera::SetZoomValue(int pow2_100ths) {
//   if (pow2_100ths < 0) {pow2_100ths = 0;}
   zoom_value_100ths = pow2_100ths;
   zoom_scale = pow(2.0 , ((double)zoom_value_100ths/100.0));
   double zoom_scale_wmax = (double)view_bmp->w;
   double zoom_scale_hmax = (double)view_bmp->h;
   double zoom_scale_max = (zoom_scale_wmax > zoom_scale_hmax)?zoom_scale_wmax:zoom_scale_hmax;
   if (zoom_scale > zoom_scale_max) {
      zoom_scale = zoom_scale_max;
      zoom_value_100ths = (int)(100.0*(log10(zoom_scale)/log10(2.0)) + 0.5);
   }
   if (zoom_scale > max_zoom_scale) {
      zoom_scale = max_zoom_scale;
      zoom_value_100ths = (int)(100.0*(log10(zoom_scale)/log10(2.0)) + 0.5);
   }
   if (zoom_scale < min_zoom_scale) {
      zoom_scale = min_zoom_scale;
      zoom_value_100ths = (int)(100.0*(log10(zoom_scale)/log10(2.0)) + 0.5);
   }
   int zoom_w = (int)((double)view_bmp->w/zoom_scale + 0.5);
   int zoom_h = (int)((double)view_bmp->h/zoom_scale + 0.5);
   
   SetDestinationDimensions(zoom_w , zoom_h);
   SetViewDimensions(dest_w , dest_h);

   SetDestination(dvcx - view_area.W()/2 , dvcy - view_area.H()/2);
   SetViewPos(dest_x , dest_y);
   ResetDisplayArea();
   SetRedrawFlag();
   zoom_time = 0.0;
}



void ZoomCamera::ChangeZoomValueBy(int delta_pow2_100ths) {
   SetZoomValue(zoom_value_100ths + delta_pow2_100ths);
}



void ZoomCamera::SetZoomScale(double scale) {
///   if (scale < 1.0) {scale = 1.0;}
   if (scale <= 0.0) {scale = 1.0;}
   double zoom_scale_wmax = (double)view_bmp->w;
   double zoom_scale_hmax = (double)view_bmp->h;
   double zoom_scale_max = (zoom_scale_wmax > zoom_scale_hmax)?zoom_scale_wmax:zoom_scale_hmax;
   if (scale > zoom_scale_max) {
      scale = zoom_scale_max;
   }
   if (scale > max_zoom_scale) {
      scale = max_zoom_scale;
   }
   if (scale < min_zoom_scale) {
      scale = min_zoom_scale;
   }
   zoom_scale = scale;
   zoom_value_100ths = (int)(100.0*(log10(zoom_scale)/log10(2.0)) + 0.5);
   int zoom_w = (int)((double)view_bmp->w/zoom_scale + 0.5);
   int zoom_h = (int)((double)view_bmp->h/zoom_scale + 0.5);
   SetDestinationDimensions(zoom_w , zoom_h);
   SetViewDimensions(dest_w , dest_h);
   SetDestination(dvcx - view_area.W()/2 , dvcy - view_area.H()/2);
   SetViewPos(dest_x , dest_y);
   ResetDisplayArea();
   SetRedrawFlag();
   zoom_time = 0.0;
}



void ZoomCamera::ChangeZoomScaleBy(double delta_scale) {
   SetZoomScale(zoom_scale + delta_scale);
}



void ZoomCamera::SetZoomRate(int rate_100ths) {
   if (rate_100ths < 0) {rate_100ths = -rate_100ths;}
   if (rate_100ths < 1) {rate_100ths = 1;}
   zoom_rate_100ths = rate_100ths;
}



void ZoomCamera::SetMinZoomDimensions(int min_width , int min_height) {
   if (min_width > view_bmp->w) {min_width = view_bmp->w;}
   if (min_height > view_bmp->h) {min_height = view_bmp->h;}
   if (min_width < 1) {min_width = 1;}
   if (min_height < 1) {min_height = 1;}
   min_zoom_w = min_width;
   min_zoom_h = min_height;
   SetDestinationDimensions(view_area.W() , view_area.H());
   SetViewDimensions(dest_w , dest_h);
}



void ZoomCamera::SetStretchType(STRETCH_TYPE cam_stretch_type) {
   stretch_type = cam_stretch_type;
   SetRedrawFlag();
}

*/


/* TODO : OLD? Why was this kept?

void ZoomCamera::SetDisplayPos(int xpos , int ypos) {
   SetDisplayArea(xpos,ypos,display_area.W(),display_area.H());
}



void ZoomCamera::SetDisplayDimensions(int width , int height) {
   SetDisplayArea(display_area.X(),display_area.Y(),width,height);
}



void ZoomCamera::SetDisplayArea(int xpos , int ypos , int width , int height) {
   if (width < 1) {width = 1;}
   if (height < 1) {height = 1;}
   if (width > area.W()) {width = area.W();}
   if (height > area.H()) {height = area.H();}
   if (xpos < 0) {xpos = 0;}
   if (ypos < 0) {ypos = 0;}
   if (xpos + width > area.W()) {
      xpos = area.W() - width;
   }
   if (ypos + height > area.H()) {
      ypos = area.H() - height;
   }
   display_area.SetArea(xpos,ypos,width,height);
   if ((buffer.W() != width) || (buffer.H() != height)) {
      buffer.Resize(width , height , MEMORY , SOLID);
      drawn = false;
   }
   SetBgRedrawFlag();
}



void ZoomCamera::SetDisplayArea(const Rectangle& r) {
   SetDisplayArea(r.X() , r.Y() , r.W() , r.H());
}
*/








