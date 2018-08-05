

#include "Game.hpp"



#define SPLASH_TIME 8.0



bool Game::Initialize() {
   sys = new Allegro5System();
   if (EAGLE_GENERAL_SETUP != sys->Initialize(EAGLE_GENERAL_SETUP)) {
      EagleLog() << "Failed to Initialize the Allegro 5 System. Utter fail, lamer." << std::endl;
      return false;
   }
   win = sys->MakeGraphicsContext(800 , 600 , EAGLE_OPENGL | EAGLE_WINDOWED);
   if (!win->Valid()) {
      EagleLog() << "Failed to create a valid 800x600 opengl window." << std::endl;
      return false;
   }
   queue = sys->CreateEventHandler();
   if (!queue->Valid()) {
      EagleLog() << "Failed to create a valid event queue." << std::endl;
      return false;
   }
   timer = sys->CreateTimer();
   timer->Create(1.0/60.0);
   if (!timer->Valid()) {
      EagleLog() << "Failed to create a valid timer." << std::endl;
      return false;
   }
   
   sys->RegisterInputs(queue);
   win->RegisterDisplayInput(queue);
   timer->RegisterTimerInput(queue);
   
   return true;
}



bool Game::LoadResources() {
   bool success = true;
   
   santahack_splash = win->LoadImage("Data/jpgImages/SantaHackSplash.jpg" , VIDEO_IMAGE);
   squire_splash = win->LoadImage("Data/jpgImages/SquireSplash.jpg" , VIDEO_IMAGE);
   character_splash = win->LoadImage("Data/pngImages/CharacterSplash.png" , VIDEO_IMAGE);
   monster1_splash = win->LoadImage("Data/pngImages/Monster1Splash.png" , VIDEO_IMAGE);
   monster2_splash = win->LoadImage("Data/pngImages/Monster2Splash.png" , VIDEO_IMAGE);
   special_splash = win->LoadImage("Data/pngImages/SpecialSplash.png" , VIDEO_IMAGE);
   stallion_finish = win->LoadImage("Data/jpgImages/Stallions2a.jpg" , VIDEO_IMAGE);
   
   sprite_sheet = win->LoadImage("Data/pngImages/SpriteSheet.png" , VIDEO_IMAGE);
   
   success = santahack_splash->Valid() &&
             squire_splash->Valid() &&
             character_splash->Valid() &&
             monster1_splash->Valid() &&
             monster2_splash->Valid() &&
             special_splash->Valid() &&
             stallion_finish->Valid() &&
             sprite_sheet->Valid();
   
   if (sprite_sheet->Valid()) {
      character_sheet = win->CreateSubImage(sprite_sheet , 0 , 0 , 256 , 192);
      monster_sheet = win->CreateSubImage(sprite_sheet , 0 , 192 , 256 , 384);
      special_sheet = win->CreateSubImage(sprite_sheet , 0 , 576 , 256 , 96);
   }
   
   for (int i = 0 ; i < NUM_CHARS ; ++i) {
      character_images[i] = win->CreateSubImage(character_sheet , 64*(i%4) , 96*(i/4) , 64 , 96);
      success = success && character_images[i]->Valid();
   }
   for (int i = 0 ; i < NUM_MONSTERS ; ++i) {
      monster_images[i] = win->CreateSubImage(monster_sheet , 64*(i%4) , 96*(i/4) , 64 , 96);
      success = success && monster_images[i]->Valid();
   }
   for (int i = 0 ; i < NUM_SPECIALS ; ++i) {
      special_images[i] = win->CreateSubImage(character_sheet , 64*(i%4) , 96*(i/4) , 64 , 96);
      success = success && special_images[i]->Valid();
   }
   
   tile_splash = win->LoadImage("Data/pngImages/TileSplash.png" , VIDEO_IMAGE);
   tile_sheet = win->LoadImage("Data/pngImages/TileSheet.png" , VIDEO_IMAGE);
   success = success && tile_splash->Valid() && tile_sheet->Valid();
   if (tile_sheet->Valid()) {
      for (int i = 0 ; i < NUM_TILES ; ++i) {
         tile_images[i] = win->CreateSubImage(tile_sheet , 64*(i%5) , 0 , 64 , 96);
         success = success && tile_images[i]->Valid();
      }
   }

   weapon_splash = win->LoadImage("Data/pngImages/WeaponSplash.png" , VIDEO_IMAGE);
   weapon_sheet = win->LoadImage("Data/pngImages/WeaponSheet.png" , VIDEO_IMAGE);
   success = success && weapon_splash->Valid() && weapon_sheet->Valid();
   
   if (weapon_sheet->Valid()) {
      for (int i = 0 ; i < NUM_WEAPONS ; ++i) {
         weapon_images[i] = win->CreateSubImage(weapon_sheet , 64*(i%4) , 96*(i/4) , 64 , 96);
         success = success && weapon_images[i]->Valid();
      }
   }

   potion_splash = win->LoadImage("Data/pngImages/Potion2Splash.png" , VIDEO_IMAGE);
   potion_sheet = win->LoadImage("Data/pngImages/Potion2Sheet.png" , VIDEO_IMAGE);
   success = success && potion_splash->Valid() && potion_sheet->Valid();

   if (potion_sheet->Valid()) {
      for (int i = 0 ; i < NUM_POTIONS ; ++i) {
         potion_images[i] = win->CreateSubImage(potion_sheet , 64*(i%NUM_POTIONS) , 0 , 64 , 96);
         success = success && potion_images[i]->Valid();
      }
   }

   return success;

}



void Game::Free() {
   if (sys) {
      delete sys;
      sys = 0;
   }
}



Game::Game() :
      sys(0),
      win(0),
      queue(0),
      timer(0),

      santahack_splash(0),
      squire_splash(0),
      character_splash(0),
      monster1_splash(0),
      monster2_splash(0),
      special_splash(0),
      stallion_finish(0),

      sprite_sheet(0),
      character_sheet(0),
      monster_sheet(0),
      special_sheet(0),

      character_images(),
      monster_images(),
      special_images(),
      
      tile_splash(0),
      tile_sheet(0),
      tile_images(),
      
      weapon_splash(0),
      weapon_sheet(0),
      weapon_images(),

      potion_splash(0),
      potion_sheet(0),
      potion_images(),

      system_ready(false),
      resources_ready(false),
      
      state(STATE_SANTAHACK_SPLASH),
      
      elapsed(0.0),
      splash_time(SPLASH_TIME)
{
   system_ready = Initialize();
   if (system_ready) {
      resources_ready = LoadResources();
   }
}



Game::~Game() {
   Free();
}



GAME_STATE Game::HandleEvent(EagleEvent ev) {
   
   EagleLog() << "Handling event # " << ev.type << " - " << EagleEventName(ev.type) << std::endl;

   if (ev.type == EAGLE_EVENT_TIMER) {
      state = Update(timer->SPT());
   }
   else if (ev.type == EAGLE_EVENT_DISPLAY_CLOSE) {
      return STATE_QUIT;
   }
   
   switch (state) {
      case STATE_SANTAHACK_SPLASH :
         if (ev.type == EAGLE_EVENT_KEY_DOWN) {
            elapsed = 0.0;
            return STATE_SQUIRE_SPLASH;
         }
         break;
      case STATE_SQUIRE_SPLASH :
         if (ev.type == EAGLE_EVENT_KEY_DOWN) {
            elapsed = 0.0;
            return STATE_CLOSING_SPLASH;
         }
         break;
      case STATE_MENU :
         
         break;
      case STATE_CHARACTER_SELECT :
         
         break;
      case STATE_ITEM_SELECT :
         
         break;
      case STATE_PLAY_INIT :
         
         break;
      case STATE_PLAY_RUN :
         
         break;
      case STATE_CLOSING_SPLASH :
         if (ev.type == EAGLE_EVENT_KEY_DOWN) {
            elapsed = 0.0;
            return STATE_QUIT;
         }
         break;
      case STATE_QUIT :
         
         break;
      case NUM_GAME_STATES : EAGLE_ASSERT(false);break;
   }

   return state;
}



GAME_STATE Game::Update(double dt) {
   EagleLog() << "Updating game by " << dt << " , state is " << state << std::endl;
   switch (state) {
      case STATE_SANTAHACK_SPLASH :
         elapsed += dt;
         if (elapsed > splash_time) {
            elapsed = 0.0;
            return STATE_SQUIRE_SPLASH;
         }
         break;
      case STATE_SQUIRE_SPLASH :
         elapsed += dt;
         if (elapsed > splash_time) {
            elapsed = 0.0;
            return STATE_CLOSING_SPLASH;
         }
         break;
      case STATE_MENU :
         
         break;
      case STATE_CHARACTER_SELECT :
         
         break;
      case STATE_ITEM_SELECT :
         
         break;
      case STATE_PLAY_INIT :
         
         break;
      case STATE_PLAY_RUN :
         
         break;
      case STATE_CLOSING_SPLASH :
/*
         elapsed += dt;
         if (elapsed > splash_time) {
            return STATE_QUIT;
         }
*/
         break;
      case STATE_QUIT :
         
         break;
      case NUM_GAME_STATES : EAGLE_ASSERT(false);break;
   }
   
   return state;
}



void Game::Display() {
   switch (state) {
      case STATE_SANTAHACK_SPLASH :
         win->Draw(santahack_splash , 0 , 0 , 0);
         win->FlipDisplay();
         break;
      case STATE_SQUIRE_SPLASH :
         win->Draw(squire_splash , 0 , 0 , 0);
         win->FlipDisplay();
         break;
      case STATE_MENU :
         
         break;
      case STATE_CHARACTER_SELECT :
         
         break;
      case STATE_ITEM_SELECT :
         
         break;
      case STATE_PLAY_INIT :
         
         break;
      case STATE_PLAY_RUN :
         
         break;
      case STATE_CLOSING_SPLASH :
         win->Draw(stallion_finish , 0 , 0 , 0);
         win->FlipDisplay();
         break;
      case STATE_QUIT :
         
         break;
      case NUM_GAME_STATES : EAGLE_ASSERT(false);break;
   }
}



void Game::Run() {
   if (!Ready()) {return;}
   
   state = STATE_SANTAHACK_SPLASH;
   
   timer->Start();
   
   EagleEvent ev;
   do {
      Display();
      
      do {
         ev = queue->WaitForEvent();
         EagleLog() << "Received event # " << ev.type << " - " << EagleEventName(ev.type) << std::endl;
         HandleInputEvent(ev);
         state = HandleEvent(ev);
         EagleLog() << "State is now " << state << std::endl;
         
      } while (queue->HasEvent());
      
   } while (state != STATE_QUIT);
}



