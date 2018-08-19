

#include "Eagle5/backends/Allegro5Backend.hpp"


enum CHARACTERS {
   HATCHET = 0,
   WHAMMER = 1,
   POLEX = 2,
   GALAHAD = 3,
   TEMPLAR = 4,
   ELVERION = 5,
   ELENTHIEL = 6,
   HADRIGAN = 7,
   NUM_CHARS = 8
};

enum MONSTERS {
   ORCI = 0,
   ORCII = 1,
   ORCIII = 2,
   ORCIV = 3,
   ORCV = 4,
   ORCVI = 5,
   ORCVII = 6,
   ORCVIII = 7,
   ORCIX = 8,
   DUERGARI = 9,
   DUERGARII = 10,
   LIVINGDEAD = 11,
   SKELETONI = 12,
   GHOULI = 13,
   GHOULII = 14,
   SKELETONII = 15,
   NUM_MONSTERS = 16
};



enum SPECIALS {
   OGRE = 0,
   ASSASSIN = 1,
   HEALER = 2,
   HOLYCRUSADER = 3,
   NUM_SPECIALS = 4
};

enum TILES {
   GRASS = 0,
   MONGENI = 1,
   MONGENII = 2,
   MONGENIII = 3,
   MONGENIV = 4,
   NUM_TILES = 5
};

enum WEAPONS {
   SPEARS = 0,
   SWORD = 1,
   CROSSBOW = 2,
   SHIELD = 3,
   MACE = 4,
   POLEAX = 5,
   AXE = 6,
   HELMET = 7,
   CASTLE = 8,
   LONGBOW = 9,
   CATAPULT = 10,
   FLAIL = 11,
   NUM_WEAPONS = 12
};

enum POTIONS {
   HEALTH = 0,
   SPIRIT = 1,
   REGEN = 2,
   ICHOR = 3,
   ANTIDOTE = 4,
   NUM_POTIONS = 5
};




enum GAME_STATE {
   STATE_SANTAHACK_SPLASH = 0,
   STATE_SQUIRE_SPLASH = 1,
   STATE_MENU = 2,
   STATE_CHARACTER_SELECT = 3,
   STATE_ITEM_SELECT = 4,
   STATE_PLAY_INIT = 5,
   STATE_PLAY_RUN = 6,
   STATE_CLOSING_SPLASH = 7,
   STATE_QUIT = 8,
   NUM_GAME_STATES = 9
};
   



class Game {

private :
   EagleSystem* sys;
   EagleGraphicsContext* win;
   EagleEventHandler* queue;
   EagleTimer* timer;
   
   EagleImage* santahack_splash;
   EagleImage* squire_splash;
   EagleImage* character_splash;
   EagleImage* monster1_splash;
   EagleImage* monster2_splash;
   EagleImage* special_splash;
   EagleImage* stallion_finish;
   
   EagleImage* sprite_sheet;
   
   EagleImage* character_sheet;
   EagleImage* monster_sheet;
   EagleImage* special_sheet;
   
   EagleImage* character_images[NUM_CHARS];
   EagleImage* monster_images[NUM_MONSTERS];
   EagleImage* special_images[NUM_SPECIALS];
   
   EagleImage* tile_splash;
   EagleImage* tile_sheet;
   EagleImage* tile_images[NUM_TILES];
   
   EagleImage* weapon_splash;
   EagleImage* weapon_sheet;
   EagleImage* weapon_images[NUM_WEAPONS];
   
   EagleImage* potion_splash;
   EagleImage* potion_sheet;
   EagleImage* potion_images[NUM_POTIONS];
   
   bool system_ready;
   bool resources_ready;

   GAME_STATE state;
   
   double elapsed;
   double splash_time;



   bool Initialize();
   bool LoadResources();
   
   void Free();

   
public :
   
   Game();
   ~Game();
   
   
   
   GAME_STATE HandleEvent(EagleEvent ev);
   GAME_STATE Update(double dt);
   void Display();
   
   void Run();
   
   bool Ready() {return system_ready && resources_ready;}
};


