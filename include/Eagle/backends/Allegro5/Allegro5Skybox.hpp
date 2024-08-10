



#ifndef Allegro5Skybox_HPP
#define Allegro5Skybox_HPP


#include "Eagle/Skybox.hpp"



class Allegro5Skybox : public Skybox {
   
public :
   Allegro5Skybox();
   
   
   virtual void Render(EagleGraphicsContext* win , Vec3 pos);
};

#endif // Allegro5Skybox_HPP
