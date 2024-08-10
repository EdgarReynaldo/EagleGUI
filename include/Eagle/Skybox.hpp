



#ifndef Skybox_HPP
#define Skybox_HPP


#include "Eagle/UnitCube.hpp"
#include "Eagle/Vec3.hpp"

#include <string>


class EagleImage;
class EagleGraphicsContext;

class Skybox {
protected :
   TexturedUnitCube sbcube;
   
   EagleImage* sbimage;
   EagleImage* sbfaces[NUM_CUBE_FACES];
public :
   Skybox();
   virtual ~Skybox();
   
   void Clear();

   void MakeSkybox(EagleGraphicsContext* win , std::string skybox_file);

   virtual void Render(EagleGraphicsContext* win , Vec3 pos)=0;
};

#endif // Skybox_HPP



