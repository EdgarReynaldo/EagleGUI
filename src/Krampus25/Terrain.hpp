



#ifndef Terrain_HPP
#define Terrain_HPP

#include <vector>


struct VSPAN {
   int x;
   int y;
   int h;
};

class EagleGraphicsContext;

class Terrain {
public :
   std::vector<VSPAN> spans;

   Terrain();
   
   
   
   void Clear();

   virtual void Generate(int sw , int sh);

   void DisplayOn(EagleGraphicsContext* win);
};

#endif // Terrain_HPP
