

#include "Eagle/backends/Allegro5Backend.hpp"
#include "Eagle.hpp"



#include <cmath>



struct Mover {
   Pos2D pos;
   Pos2D vel;
   double dir;
   double speed;
   
   Mover() : pos(),vel(),dir(-1.0),speed(0.0) {}
   Mover(double px , double py , double vx , double vy) : pos(px,py) , vel(vx,vy) , dir(atan2(vy,vx)) , speed(sqrt(vy*vy + vx*vx)) {}
//   double AimAheadT(const Mover& target , double velocity , double tsecs_ahead);
   double AimAheadT(const Mover& target , double velocity , double tsecs_ahead) {
      const double dx = target.pos.X() - pos.X();
      const double dy = target.pos.Y() - pos.Y();
      const double pvx = target.vel.X();
      const double pvy = target.vel.Y();
      const double bvz = velocity;
      const double t = tsecs_ahead;
      if ((bvz <= 0.0) || (t == 0.0)) {
         return dir = -1.0;
      }
      double angle = atan2(dy/(bvz*t) + pvy/bvz , dx/(bvz*t) + pvx/bvz);
      if (angle < 0.0) {
         return dir = angle + 2.0*M_PI;
      }
      return dir = angle;
   }
   double AimAheadX(const Mover& target , double velocity) {
      const double px = target.pos.X();
      const double py = target.pos.Y();
      const double bx = pos.X();
      const double by = pos.Y();
      const double dx = px - bx;
      const double dy = py - by;
      const double dzsq = dx*dx + dy*dy; 
      const double dz = sqrt(dzsq);
      const double pvx = target.vel.X();
      const double pvy = target.vel.Y();
      const double pvzsq = pvx*pvx + pvy*pvy;
      const double pvz = sqrt(pvzsq);
      const double bvzsq = velocity*velocity;
      const double bvz = sqrt(bvzsq);

//  t^2(pvx^2 + pvy^2 - bv^2)
// +t^1(2pxpvx + 2pypvy - 2bxpvx - 2bypvy)
// +t^0(px^2 + bx^2 + py^2 + by^2 - 2bxpx - 2bypy)

   double A = pvz*pvz - bvz*bvz;
   double B = 2*px*pvx + 2*py*pvy - 2*bx*pvx - 2*by*pvy;
   double C = px*px + bx*bx + py*py + by*by - 2*bx*px - 2*by*py;
   double D = B*B - 4*A*C;
   if (A == 0.0 || D < 0.0) {
      return -1.0;
   }
   
   double t1 = (-B - sqrt(D))/(2.0*A);
   double t2 = (-B + sqrt(D))/(2.0*A);

   double t = t2;
   if (t2 < 0.0 || t1 > 0.0 && t1 < t2) {
      t = t1;
   }
   
   EagleInfo() << "T is " << t << std::endl;
   if (t < 0.0) {
      return -1.0;
   }
   
///      const double t = tsecs_ahead;
/** DanielH's method, doesn't work because we don't know bvx and bvy
      /// t = sqrt(((bx - px)^2 + (by - py)^2) / (pv^2 + bv^2 - 2*pv*bv*cos(C)))
      ///cos(angle) = (v1 . v2)/(||v1||||v2||)
      double B = acos((pvx*bvx + pvy*bvy)/(pvz*bvz));
      /// A = asin(pvz*sin(B)/bvz)
      double A = asin(pvz*sin(B)/bvz);
      double C = M_PI - A - B;
      const double cosC = cos(C);
      const double bvx = bvz*cosC;
      const double bvy = bvz*sin(C);
      double t = sqrt(dz*dz/((pvzsq + bvzsq - 2.0*(pvx*pvx + pvy*pvy)*(bvz*bvz)*cos(C))));
      EagleInfo() << "T is " << t << std::endl;
*/



/**
This doesn't work
      const double A = pvzsq - bvzsq;
      const double B = 2*(pvy*dy + pvx*dx);
      const double C = dzsq;
*/
/**
      const double A = pvzsq - bvzsq;
      const double B = 2*px*pvx + 2*py*pvy - 2*bx*pvx - 2*by*pvy;
      const double C = px*px + bx*bx + py*py + by*by - 2*bx*px - 2*by*py;
      
      if (A == 0.0) {
         /// They're traveling the same speed, this equation won't work by itself
         return dir = -1.0;
      }
      
      const double discrim = B*B - 4*A*C;
      if (discrim < 0.0) {
         return dir = -1.0;
      }
      const double sqrtd = sqrt(discrim);
      
      const double t1 = (-B - sqrtd)/(2.0*A);
      const double t2 = (-B + sqrtd)/(2.0*A);

      const double* T = (t1 < t2)?&t1:&t2;
      if (t1 <= 0.0) {
         T = &t2;
         if (t2 <= 0.0) {
            return dir = -1.0;
         }
      }
      /// T now points to the smallest positive time if there is one
      EagleInfo() << "T is " << *T << std::endl;
      
      double angle = atan2(dy/(bvz*(*T)) + pvy/bvz , dx/(bvz*(*T)) + pvx/bvz);
*/
      double angle = atan2(dy + pvy*t , dx + pvx*t);
      if (angle < 0.0) {
         return dir = angle + 2.0*M_PI;
      }
      return dir = angle;
   }
//   void Update(double dt);
   void Update(double dt) {
      pos.tx += vel.X()*dt;
      pos.ty += vel.Y()*dt;
   }
//   void SetDirectionAndSpeed(double theta , double velocity);
   void SetDirectionAndSpeed(double theta , double velocity) {
      if (theta < 0.0) {velocity = 0.0;}
      dir = theta;
      vel.tx = velocity*cos(dir);
      vel.ty = velocity*sin(dir);
      speed = velocity;
   }
   double GetDirection() {return dir;}
   double GetSpeed() {return speed;}
};


struct Explosion : public AnimationBase {
   double cx,cy;
   static EagleImage* frames[4];
   bool complete;
   
   void OnComplete() {
      complete = true;
   }
   
   Explosion(double x , double y) :
         AnimationBase(),
         cx(x),
         cy(y),
         complete(false)
   {
      Init(4 , 1 , 3.0 , ANIMATION_ONCE_FORWARDS);
   }
   static void Setup(EagleImage* imgs[4]) {
      for (unsigned int i = 0 ; i < 4 ; ++i) {
         frames[i] = imgs[i];
      }
      
   }
   
};

EagleImage* Explosion::frames[4] = {0,0,0,0};


enum CONTROL {
   PLANE,
   TANK
};


int main(int argc , char** argv) {
   
   (void)argc;
   (void)argv;
   
   EagleSystem* sys = GetAllegro5System();
   
   EAGLE_ASSERT(sys);
   
   int ret = sys->Initialize(EAGLE_FULL_SETUP);
   if (ret != EAGLE_FULL_SETUP) {
      EagleLog() << "Failed to initialized subsystem. Continuing." << std::endl;
   }
   
   int sw = 1024;
   int sh = 768;
   
   EagleGraphicsContext* win = sys->CreateGraphicsContext("WIN" , sw , sh , EAGLE_OPENGL | EAGLE_WINDOWED | EAGLE_NOFRAME);
   EAGLE_ASSERT(win && win->Valid());
 
   CONTROL control = TANK;
   
   Mover plane;
   
   Mover tank(100 , 768-32 , 0 , 0);
   
   std::list<Mover> projectiles;
   const int reload_time = 15;
   int frame_time;
   
   Rectangle screen(0,0,sw,sh);
 
   bool newplane = true;
 
   std::list<Explosion> explosions;
   
   EagleImage* explosion = win->LoadImageFromFile("Data/Images/Explosion.png");
   EAGLE_ASSERT(explosion && explosion->Valid());
 
   EagleImage* exp[4] = {
      win->CreateSubImage(explosion , 0 , 0 , 16 , 16),
      win->CreateSubImage(explosion , 16 , 0 , 16 , 16),
      win->CreateSubImage(explosion , 32 , 0 , 16 , 16),
      win->CreateSubImage(explosion , 48 , 0 , 16 , 16)
   };
    
   Explosion::Setup(exp);
   
   EagleImage* tankimg = win->LoadImageFromFile("Data/Images/TankImage.png");
   EagleImage* planeimg = win->LoadImageFromFile("Data/Images/PlaneImage.png");
   
   EAGLE_ASSERT(tankimg && tankimg->Valid() && planeimg && planeimg->Valid());
   
   MTRNG rng;
   rng.Seed(0);
   
   bool quit = false;
   bool redraw = true;
   
   sys->GetSystemTimer()->Start();
   
   while (!quit) {
         
         
      do {
         EagleEvent e = sys->WaitForSystemEventAndUpdateState();
         
         if (e.type == EAGLE_EVENT_DISPLAY_CLOSE) {
            quit = true;
         }
         if (e.type == EAGLE_EVENT_KEY_DOWN && e.keyboard.keycode == EAGLE_KEY_ESCAPE) {
            quit = true;
         }
         if (e.type == EAGLE_EVENT_KEY_DOWN) {
            if (e.keyboard.keycode == EAGLE_KEY_P) {
               control = PLANE;
            }
            else if (e.keyboard.keycode == EAGLE_KEY_T) {
               control = TANK;
            }
         }
         
         if (e.type == EAGLE_EVENT_TIMER) {
            redraw = true;
            ++frame_time;
            if (control == TANK) {
               if (input_key_press(EAGLE_KEY_LEFT) || input_key_held(EAGLE_KEY_LEFT)) {
                  tank.SetDirectionAndSpeed(M_PI , 100.0);
                  if (tank.pos.tx < 0) {tank.pos.tx = 0;}
               }
               else if (input_key_press(EAGLE_KEY_RIGHT) || input_key_held(EAGLE_KEY_RIGHT)) {
                  tank.SetDirectionAndSpeed(0.0 , 100.0);
                  if (tank.pos.tx >= sw) {tank.pos.tx = sw;}
               }
               else {
                  tank.SetDirectionAndSpeed(0.0 , 0.0);
               }
            }
            else if (control == PLANE) {
               if (input_key_press(EAGLE_KEY_DOWN) || input_key_held(EAGLE_KEY_DOWN)) {
                   /// pitch up
                   plane.SetDirectionAndSpeed(plane.GetDirection() - M_PI/60.0 , plane.GetSpeed());
               }
               else if (input_key_press(EAGLE_KEY_UP) || input_key_held(EAGLE_KEY_UP)) {
                  /// pitch down
                  plane.SetDirectionAndSpeed(plane.GetDirection() + M_PI/60.0 , plane.GetSpeed());
               }
            }
            if (newplane) {
               double planespeed = 50.0;
               plane.pos.tx = sw-32;
               plane.pos.ty = rng.DPercent()*(sh-16);
               plane.SetDirectionAndSpeed(3.0*M_PI/4.0 + rng.DPercent()*M_PI/2.0 , planespeed);
               newplane = false;
            }
            else {
               plane.Update(e.timer.eagle_timer_source->SecondsPerTick());
            }
            Rectangle pln;
            pln.SetCenterPos(plane.pos.X() , plane.pos.Y());
            pln.SetDimensions(5 , 5);
            Rectangle scrn(0 , 0 , sw , sh);
            for (std::list<Mover>::iterator it = projectiles.begin() ; it != projectiles.end() ; ) {
               it->Update(e.timer.eagle_timer_source->SecondsPerTick());
               Rectangle proj;
               proj.SetCenterPos(it->pos.X() , it->pos.Y());
               proj.SetDimensions(5 , 5);
               if (!scrn.Overlaps(proj) || pln.Overlaps(proj)) {
                  it = projectiles.erase(it);
                  explosions.push_back(Explosion(proj.CX() , proj.CY()));
                  if (pln.Overlaps(proj)) {
                     newplane = true;
                  }
               }
               else {
                  ++it;
               }
            }
            /// Update explosions
            for(std::list<Explosion>::iterator it = explosions.begin() ; it != explosions.end() ; ) {
               Explosion& xp = *it;
               xp.AdvanceAnimationTime(e.timer.eagle_timer_source->SecondsPerTick());
               if (xp.complete) {
                  it = explosions.erase(it);
               }
               else {
                  ++it;
               }
            }
            tank.Update(e.timer.eagle_timer_source->SecondsPerTick());

            if (frame_time % reload_time == 0) {
               double projspeed = 100.0;
               double aimtime = 1.0;
               double angle = tank.AimAheadX(plane , projspeed);
               if (angle != -1.0) {
                  projectiles.push_back(Mover(tank.pos.X() , tank.pos.Y() , projspeed*cos(angle) , projspeed*sin(angle)));
               }
            }
         }
      } while (!sys->UpToDate());
      if (redraw) {
         win->SetDrawingTarget(win->GetBackBuffer());
         win->Clear();
         
         
         for (std::list<Mover>::iterator it = projectiles.begin() ; it != projectiles.end() ; ) {
            win->DrawLine(it->pos.X() , it->pos.Y() , it->pos.X() - it->vel.X() , it->pos.Y() - it->vel.Y() , EagleColor(192,192,192,255));
            ++it;
         }
         
         for(std::list<Explosion>::iterator it = explosions.begin() ; it != explosions.end() ; ++it) {
            Explosion& xp = *it;
            win->Draw(xp.frames[xp.GetFrameNum()] , xp.cx - 8 , xp.cy - 8);
         }
         win->Draw(tankimg , tank.pos.X() - 32 , tank.pos.Y() - 16);
         
         if (screen.Contains(plane.pos.X() , plane.pos.Y())) {
            win->Draw(planeimg , plane.pos.X() - 32 , plane.pos.Y() - 16 , HALIGN_LEFT , VALIGN_TOP , EagleColor(255,255,255,255) , (plane.vel.X() < 0.0)?DRAW_NORMAL:DRAW_HFLIP);
         }
         else {
            newplane = true;
         }
         
         win->FlipDisplay();
         redraw = false;
      }
         
   };
   
   
   return 0;
}



ALLEGRO_BITMAP* CreateTexturedBitmnap(ALLEGRO_BITMAP* mask , ALLEGRO_COLOR maskcolor , ALLEGRO_BITMAP* texture , ALLEGRO_BITMAP* image);

#include "allegro5/allegro_image.h"



int main2(int argc , char** argv) {
   
   (void)argc;
   (void)argv;
   
   if (!al_init()) {return 1;}
   
   if (!al_init_image_addon()) {return 2;}
   

   return 0;
}

ALLEGRO_BITMAP* CreateTexturedBitmap(ALLEGRO_BITMAP* mask , ALLEGRO_COLOR maskcolor , ALLEGRO_BITMAP* texture , ALLEGRO_BITMAP* image) {
   
   ALLEGRO_LOCKED_REGION* mlock = 0;
   ALLEGRO_LOCKED_REGION* texlock = 0;
   ALLEGRO_LOCKED_REGION* imagelock = 0;
   ALLEGRO_LOCKED_REGION* destlock = 0;
   
   int blop = 0;
   int blsrc = 0;
   int bldest = 0;
   al_get_blender(&blop , &blsrc , &bldest);
   
   ALLEGRO_BITMAP* oldtarget = al_get_target_bitmap();
   
   
   int w = al_get_bitmap_width(image);
   int h = al_get_bitmap_height(image);
   if (al_get_bitmap_width(texture) != w || al_get_bitmap_height(texture) != h || al_get_bitmap_width(mask) != w || al_get_bitmap_height(mask) != h) {
      printf("Bitmaps not same size.\n");
      return 0;
   }
   ALLEGRO_BITMAP* dest = al_create_bitmap(w , h);
   if (!dest) {
      return 0;
   }
   al_set_target_bitmap(dest);
   al_set_blender(ALLEGRO_ADD , ALLEGRO_ONE , ALLEGRO_ZERO);
   al_draw_bitmap(image , 0 , 0 , 0);
   
   mlock = al_lock_bitmap(mask , ALLEGRO_PIXEL_FORMAT_ANY , ALLEGRO_LOCK_READONLY);
   if (!mlock) {
      al_destroy_bitmap(dest);
      dest = 0;
      goto cleanup;
   }
   
   texlock = al_lock_bitmap(texture , ALLEGRO_PIXEL_FORMAT_ANY , ALLEGRO_LOCK_READONLY);
   if (!texlock) {
      al_destroy_bitmap(dest);
      dest = 0;
      goto cleanup;
   }
   
   imagelock = al_lock_bitmap(image , ALLEGRO_PIXEL_FORMAT_ANY , ALLEGRO_LOCK_READONLY);
   if (!imagelock) {
      al_destroy_bitmap(dest);
      dest = 0;
      goto cleanup;
   }

   destlock = al_lock_bitmap(dest , ALLEGRO_PIXEL_FORMAT_ANY , ALLEGRO_LOCK_READWRITE);
   if (!destlock) {
      al_destroy_bitmap(dest);
      dest = 0;
      goto cleanup;
   }

   al_set_target_bitmap(dest);
   al_set_blender(ALLEGRO_ADD , ALLEGRO_DEST_COLOR , ALLEGRO_ZERO);/// Multiply blender
   
   for (int y = 0 ; y < h ; ++y) {
      for (int x = 0 ; x < w ; ++x) {
         ALLEGRO_COLOR mcol = al_get_pixel(mask , x , y);
         if (mcol.r == maskcolor.r && mcol.g == maskcolor.g && mcol.b == maskcolor.b) {
            // pixel on mask bitmap matches maskcolor
            al_put_blended_pixel(x , y , al_get_pixel(texture , x , y));/// draw a blended pixel from the texture if it is the mask color
         }
         else {
            al_put_pixel(x , y , al_get_pixel(image , x , y));/// draw a standard pixel from the image (non masked)
         }
      }
   }
   
   
   cleanup:
   if (mlock) {al_unlock_bitmap(mask);}
   if (texlock) {al_unlock_bitmap(texture);}
   if (imagelock) {al_unlock_bitmap(image);}
   if (destlock) {al_unlock_bitmap(dest);}
   
   al_set_blender(blop , blsrc , bldest);
   al_set_target_bitmap(oldtarget);
   
   return dest;
}



