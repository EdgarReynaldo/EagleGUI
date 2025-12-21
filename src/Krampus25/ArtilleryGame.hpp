



#ifndef ArtilleryGame_HPP
#define ArtilleryGame_HPP


#include "Scene.hpp"
#include "Terrain.hpp"


#include "Eagle/Events.hpp"



#include <vector>

class EagleGraphicsContext;
class ArtilleryPlayer;
class Projectile;

class ArtilleryGame : public Scene {

   EagleGraphicsContext* window;
   Terrain terrain;
   
   int level;
   int turn;
   double windvel;
   double windangled;
   
   bool game_over;
   
   std::vector<ArtilleryPlayer*> players;
   std::vector<Projectile*> live_rounds;

public :
   ArtilleryGame(EagleGraphicsContext* win);
   ~ArtilleryGame();
   
   
   void Clear();
   
   virtual int HandleEvent(EagleEvent e);
   virtual void DisplayOn(EagleGraphicsContext* win);
   virtual int Update(double dt);

   void AddPlayer(ArtilleryPlayer* newp);
   void AddProjectile(Projectile* newp);

};




#endif // ArtilleryGame_HPP


