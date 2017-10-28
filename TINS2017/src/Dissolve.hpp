


#ifndef Dissolve_HPP
#define Dissolve_HPP

#include "Eagle/Color.hpp"
#include "Eagle/Position.hpp"
#include "Eagle/Image.hpp"
#include "Eagle/AnimationBase.hpp"

#include "Animation.hpp"

struct Particle {
   EagleColor c;
   Pos2D p;
   
   Particle() : c() , p(0,0) {}
   Particle(EagleColor col , double x , double y) : c(col) , p(x,y) {}
};

Particle InterpolatePartical(const Particle& s , const Particle& d , double percent);



class Glob {
public :
   EagleImage* src;
   std::vector<Particle> spart;
   
   int NParticles();
   Particle* GetParticle(int n);

   void ConstructFromSource(EagleImage* source);
   void ConstructFromSource(EagleImage* source , int srcx , int srcy);
   
   void ConstructFromParticleMap(std::vector<std::pair<Particle*,Particle*> >& pmap , double percent);
   
};


class Dissolver : public Animation {
   Glob srcglob;
   Glob destglob;
   Glob currentglob;
   
   std::vector<std::pair<Particle* , Particle*> > transfer_map;
   
   void RecreateTransferMap();


protected :
   virtual void OnSetAnimationPercent();

   virtual void OnLoopComplete();
   virtual void OnComplete();

public :
   
   Dissolver();

   virtual void Draw(EagleGraphicsContext* win , int x , int y);

   void ConstructDissolveMapFromImages(EagleImage* src , EagleImage* dest);
   void ConstructDissolveMapFromImages(EagleImage* src , int srcx , int srcy , EagleImage* dest , int destx , int desty);

   void ConstructDissolveMapFromSources(EagleImage* src , Glob& result);

   int NParticles() {return (int)transfer_map.size();}
};

#endif // Dissolve_HPP
