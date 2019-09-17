
#include <vector>
#include <unordered_set>


#include "Eagle.hpp"
#include "Eagle/backends/Allegro5Backend.hpp"
#include "hexsrc/Game.hpp"


int main(int argc , char** argv) {
   
   (void)argc;
   (void)argv;
   
   
   Allegro5System* a5sys = GetAllegro5System();
   
   int ret = 0;
   if ((ret = a5sys->Initialize(EAGLE_FULL_SETUP)) != EAGLE_FULL_SETUP) {
      if (!(ret & (EAGLE_MOUSE | EAGLE_KEYBOARD))) {
         EagleCritical() << "Failed to install keyboard and mouse." << std::endl;
      }
   }
   
   int sw = 1024;
   int sh = 768;
   EagleGraphicsContext* win = a5sys->CreateGraphicsContext("HexGame window" , sw , sh , EAGLE_OPENGL);
   
   EAGLE_ASSERT(win && win->Valid());
   
   HexGame game;
   
   game.Resize(14,12,25);
   
   game.Position(sw/4 , sh/4);
   
   a5sys->GetSystemTimer()->Start();

   game.Run(a5sys , win);
   
//   win->FlipDisplay();
   
///   a5sys->GetSystemQueue()->WaitForEvent(EAGLE_EVENT_KEY_DOWN , 0);
   
   
   return 0;
}






/**
//if (direction == 3) //RIGHT
//if (direction == 2) //LEFT
//if (direction == 1) //UP
//if (direction == 0) //DOWN

enum DIRECTION {
   DOWN = 0,
   UP = 1,
   LEFT = 2,
   RIGHT = 3
};

struct DXY {
   int x;
   int y;
};

static const struct DXY movedir[4] = {
   {0  ,  1},/// down
   {0  , -1},/// up
   {-1 ,  0},/// left
   {1  ,  0} /// right
};

typedef struct Rectangle {
   double x,y,rx,by;/// left x, top y , right x , bottom y
};

Rectangle SweepRectangle(const Rectangle& rect , double dx , double dy) {
   Rectangle r = rect;
   r.x += (dx>0)?0:dx;
   r.y += (dy>0)?0:dy;
   r.rx += (dx>0)?dx:0;
   r.by += (dy>0)?dy:0;
   return r;
}

/// In update loop
   Rectangle swept = SweepRectangle(snake_head_rect , dt*snake_speed*DXY[direction].x , dt*snake_speed*DXY[direction].y);
   if (swept.Overlaps(fruit)) {
      /// ate some sweet fruit
      score += 1;
      remove_fruit();
      place_new_fruit();
   }
   else if (swept.Overlaps(edges) {
      /// died
   }
   else if (swept.Overlaps(body)) {
      /// ate ourselves
   }
   else {
      /// move normally
      snake_head_rect.MoveBy(dt*snake_speed*DXY[direction].x , dt*snake_speed*DXY[direction].y);
   }
</code>
   
   
*/




