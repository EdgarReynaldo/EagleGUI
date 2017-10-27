


#include "Dissolve.hpp"
#include "Eagle/backends/Allegro5Backend.hpp"


Particle InterpolatePartical(const Particle& s , const Particle& d , double percent) {
   if (percent < 0.0) {return s;}
   if (percent >= 1.0) {return d;}
   
   Particle c;
   c.p = s.p + percent*(d.p - s.p);

   EagleColor sc = s.c;
   EagleColor dc = d.c;
   int r = sc.r + percent*(dc.r - sc.r);
   int g = sc.g + percent*(dc.g - sc.g);
   int b = sc.b + percent*(dc.b - sc.b);
   int a = sc.a + percent*(dc.a - sc.a);
   c.c = EagleColor(r,g,b,a);
   return c;
}



/// --------------------------      Glob class      -----------------------------------



void Glob::ConstructFromSource(EagleImage* source) {
   ConstructFromSource(source , 0 , 0);
}



void Glob::ConstructFromSource(EagleImage* source , int srcx , int srcy) {
   spart.clear();
   src = source;
   ALLEGRO_BITMAP* bmp = (dynamic_cast<Allegro5Image*>(src))->AllegroBitmap();
   if (!bmp) {return;}
   ALLEGRO_LOCKED_REGION* dat = al_lock_bitmap(bmp , ALLEGRO_PIXEL_FORMAT_ANY_32_WITH_ALPHA , ALLEGRO_LOCK_READONLY);
   for (int y = 0 ; y < src->H() ; ++y) {
      for (int x = 0 ; x < src->W() ; ++x) {
         ALLEGRO_COLOR ac = al_get_pixel(bmp , x , y);
         EagleColor c = GetEagleColor(ac);
         if (c.fa > 0.1) {
            spart.push_back(Particle(c , x + srcx , y + srcy));
         }
      }
   }
   al_unlock_bitmap(bmp);
}



int Glob::NParticles() {return (int)spart.size();}



Particle* Glob::GetParticle(int n) {
   return &(spart[n]);
}



void Glob::ConstructFromParticleMap(std::vector<std::pair<Particle*,Particle*> >& pmap , double percent) {
   spart.clear();
   spart.resize(pmap.size());
   for (int i = 0 ; i < (int)pmap.size() ; ++i) {
      spart[i] = InterpolatePartical(*(pmap[i].first) , *(pmap[i].second) , percent);
   }
}



/// ----------------------     Dissolver class     ----------------------------------



void Dissolver::RecreateTransferMap() {
   double nsrc = srcglob.NParticles();
   double ndest = destglob.NParticles();
   if (nsrc >= ndest) {
      for (int i = 0 ; i < (int)nsrc ; ++i) {
         Particle* sp = srcglob.GetParticle(i);
         int j = (int)(i*ndest/nsrc);
         EAGLE_ASSERT(j < ndest);
         Particle* dp = destglob.GetParticle(j);
         transfer_map.push_back(std::pair<Particle*,Particle*>(sp,dp));
      }
   }
   else {
      for (int j = 0 ; j < (int)ndest ; ++j) {
         Particle* dp = destglob.GetParticle(j);
         int i = (int)(j*nsrc/ndest);
         EAGLE_ASSERT(i < nsrc);
         Particle* sp = srcglob.GetParticle(i);
         transfer_map.push_back(std::pair<Particle*,Particle*>(sp,dp));
      }
   }
}



void Dissolver::OnSetAnimationPercent() {
   currentglob.ConstructFromParticleMap(transfer_map , GetAnimationPercent());
}



void Dissolver::OnLoopComplete() {(void)0;}



void Dissolver::OnComplete() {(void)0;}



Dissolver::Dissolver() :
      Animation(),
      srcglob(),
      destglob(),
      currentglob(),
      transfer_map()
{}


   
void Dissolver::Draw(EagleGraphicsContext* win , int x , int y) {
   (void)win;
   const int NP = currentglob.NParticles();
///   EagleInfo() << "Drawing " << NP << " particles" << std::endl;
   ALLEGRO_VERTEX* vtx = new ALLEGRO_VERTEX[NP];
   for (int i = 0 ; i < NP ; ++i) {
      Particle* p = currentglob.GetParticle(i);
      ALLEGRO_VERTEX* v = &(vtx[i]);
      v->x = p->p.X() + x;
      v->y = p->p.Y() + y;
      v->z = 0;
      v->color = GetAllegroColor(p->c);
///      EagleInfo() << "Point " << p->p << " color " << p->c << std::endl;
   }
   EagleInfo() << al_draw_prim(vtx , 0 , 0 , 0 , NP , ALLEGRO_PRIM_POINT_LIST) << " primitives drawn" << std::endl;
   delete [] vtx;
}



void Dissolver::ConstructDissolveMapFromImages(EagleImage* src , EagleImage* dest) {
   ConstructDissolveMapFromImages(src , 0 , 0 , dest , 0 , 0);
}



void Dissolver::ConstructDissolveMapFromImages(EagleImage* src , int srcx , int srcy , EagleImage* dest , int destx , int desty) {
   transfer_map.clear();
   srcglob.ConstructFromSource(src , srcx , srcy);
   destglob.ConstructFromSource(dest , destx , desty);
   RecreateTransferMap();
}



void Dissolver::ConstructDissolveMapFromSources(EagleImage* src , Glob& result) {
   transfer_map.clear();
   srcglob.ConstructFromSource(src);
   destglob = result;
   RecreateTransferMap();
}

