



#ifndef Terrain_HPP
#define Terrain_HPP

#include "Eagle/GraphicsContext.hpp"

struct VSPAN {
   int x;
   int y;
   int h;
};

class Terrain {

protected :
   std::vector<VSPAN> spans;
   
   
   
public :
   Terrain();
   
   
   
   void Clear();

   virtual void Generate(int sw , int sh);

   void DrawOn(EagleGraphicsContext* win);
};

#endif // Terrain_HPP
