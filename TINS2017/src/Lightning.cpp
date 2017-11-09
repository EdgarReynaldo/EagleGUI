


#include "Lightning.hpp"
#include "RNG.hpp"

#include "Eagle/GraphicsContext.hpp"

#define M_PI 3.14159265



Runner::Runner() :
      srcpos(),
      destpos(),
      runprev(0),
      runmain(0),
      runfork(0)
{
   
}



Runner::~Runner() {
   Free();
}



void Runner::Bolt(Pos2D start , Pos2D stop , double max_leg_length) {
   double fork_chance = 0.2;
   Free();
   srcpos = start;
   Pos2D dp = stop - start;
   double theta = atan2(dp.Y() , dp.X());
   double dist = dp.Length();
   if (dist <= max_leg_length) {
      /// Close enough to strike now
      destpos = stop;
      return;
   }
   else {
      /// Wander around for a while
      int dtheta = rng.Rand0toN(90) - 45;
      theta += (float)dtheta*(M_PI/180.0);
      double pct = 0.5 + rng.Rand0toNminus1(500)/1000.0;
      destpos = Vector(srcpos , max_leg_length*pct , theta);
   }
   /// Continue main runner
   runmain = new Runner();
   runmain->Bolt(destpos , stop , max_leg_length);
   
   /// Possibly fork
   double fork = rng.Rand0toNminus1(1000)/1000.0;
   if (fork <= fork_chance) {
      runfork = new Runner();
      runfork->Bolt(destpos , stop , max_leg_length);
   }
}



void Runner::Free() {
   if (runfork) {
      delete runfork;
      runfork = 0;
   }
   if (runmain) {
      delete runmain;
      runmain = 0;
   }
}



void Runner::Draw(EagleGraphicsContext* win , double length) {
   double l = (destpos - srcpos).Length();
   double thickness = 5.0;
   EagleColor color(255,255,255,127);
   
   if (l < length) {
      length -= l;
      win->DrawLine(srcpos.X() , srcpos.Y() , destpos.X() , destpos.Y() , thickness , color);
      win->DrawLine(srcpos.X() , srcpos.Y() , destpos.X() , destpos.Y() , 1.0 , EagleColor(255,255,255,255));
   }
   else {
      double pct = length/l;
      length -= l*pct;
      Pos2D d = Vector(srcpos , pct*l , srcpos.AngleToPoint(destpos));
      win->DrawLine(srcpos.X() , srcpos.Y() , d.X() , d.Y() , thickness , color);
      win->DrawLine(srcpos.X() , srcpos.Y() , d.X() , d.Y() , 1.0 , EagleColor(255,255,255,255));
   }
   if (length > 0.0) {
      if (runfork) {
         runfork->Draw(win , length);
      }
      if (runmain) {
         runmain->Draw(win , length);
      }
   }
}



double Runner::Length() {
   Pos2D dp = destpos - srcpos;
   double l1 = runmain?runmain->Length():0;
   double l2 = runfork?runfork->Length():0;
   double m = l2>l1?l2:l1;
   return dp.Length() + m;
}




/// -------------------     Lightning     --------------------------





LightningBolt::LightningBolt() :
   head()
{
   Init(3.0 , 1 , ANIMATION_ONCE_FORWARDS_AND_BACKWARDS , 1);
}



LightningBolt::~LightningBolt() {
   head.Free();
}



void LightningBolt::Draw(EagleGraphicsContext* win , int x , int y) {
   double l = head.Length();
   double pct = GetNormalizedPercent();
   head.Draw(win , pct*l);
}


#include "Eagle/Time.hpp"


void LightningBolt::Create(int rng_seed , int x1 , int y1 , int x2 , int y2) {
   head.Free();
   rng.SeedRng((int)ProgramTime::Elapsed()*10000.0);
   head.Bolt(Pos2D(x1,y1) , Pos2D(x2,y2) , 50.0);
}








