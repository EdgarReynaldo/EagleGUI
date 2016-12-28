


#include "Avatars.hpp"
#include <cstdio>


const char* avatar_names[NUM_AVATARS] = {
   "358_Elias_avatar",
   "940_Amarillion_avatar",
   "1666_Derezo_avatar",
///   "3110_MiguelFire_avatar",
   "3861_GullRaDriel_avatar",
   "5337_mc_avatar",
   "6152_Onewing_avatar",
   "7827_SiegeLord_avatar",
   "8592_EdgarReynaldo_avatar",
   "8643_Vanneto_avatar",
   "1146_MarkOates_avatar",
   "14841_EricJohnson_avatar",
   "16609_TakaakiFuruKawa_avatar"
};



ALLEGRO_BITMAP* avatars160[NUM_AVATARS] = {0};
ALLEGRO_BITMAP* avatars64[NUM_AVATARS] = {0};
ALLEGRO_BITMAP* avatars32[NUM_AVATARS] = {0};
ALLEGRO_BITMAP* avatars32bw[NUM_AVATARS] = {0};

ALLEGRO_BITMAP* avatar_atlas160 = 0;
ALLEGRO_BITMAP* avatar_atlas64 = 0;
ALLEGRO_BITMAP* avatar_atlas32 = 0;
ALLEGRO_BITMAP* avatar_atlas32bw = 0;



bool CreateAvatarAtlas(ALLEGRO_BITMAP*& atlas , int w , int h , const char* directory , const char* const type) {
   
   char avatar_path[4096] = {0};

   int total_width = NUM_AVATARS*(w+2);
   int total_height = h + 2;

   if (atlas) {
      if (!al_is_sub_bitmap(atlas)) {
         al_destroy_bitmap(atlas);
         atlas = 0;
      }
   }
   
   if (!atlas) {
      atlas = al_create_bitmap(total_width , total_height);
   }
   if (!atlas) {
      printf("Failed to create %d x %d atlas.\n" , total_width , total_height);
      return false;
   }
   
   
   bool fail = false;
   
   al_set_target_bitmap(atlas);
   al_set_blender(ALLEGRO_ADD , ALLEGRO_ONE , ALLEGRO_ZERO);
   al_clear_to_color(al_map_rgba(0,0,0,0));
   for (int i = 0 ; i < NUM_AVATARS ; ++i) {
      sprintf(avatar_path , "%s/%s%s" , directory , avatar_names[i] , type);
      ALLEGRO_BITMAP* avatar = al_load_bitmap(avatar_path);
      if (!avatar) {
         printf("Failed to load avatar %s\n" , avatar_path);
         fail = true;
      }
      else {
         al_draw_bitmap(avatar , i*(w+2) + 1 , 1 , 0);
         al_destroy_bitmap(avatar);
         avatar = 0;
      }
   }
   al_set_blender(ALLEGRO_ADD , ALLEGRO_ONE , ALLEGRO_INVERSE_ALPHA);
   if (fail) {
      al_destroy_bitmap(atlas);
      atlas = 0;
   }
   
   return atlas;
}



bool CreateAvatars(ALLEGRO_BITMAP* atlas , ALLEGRO_BITMAP** store_sub_avatars , int w , int h) {
   bool success = true;
   for (int i = 0 ; i < NUM_AVATARS ; ++i) {
      success = success && (store_sub_avatars[i] = al_create_sub_bitmap(atlas , i*(w+2) + 1 , 1 , w , h));
   }
   return success;
}



bool PopulateAvatars() {
   bool ret = true;
   
   ret = ret && CreateAvatarAtlas(avatar_atlas160 , 160 , 160  , "data/avatarpacks/avatarpack160" , "_160x160.png");
   ret = ret && CreateAvatarAtlas(avatar_atlas64   , 64  , 64   , "data/avatarpacks/avatarpack64"  , "_64x64.png");
   ret = ret && CreateAvatarAtlas(avatar_atlas32   , 32  , 32   , "data/avatarpacks/avatarpack32"  , "_32x32.png");
   ret = ret && CreateAvatarAtlas(avatar_atlas32bw , 32  , 32 , "data/avatarpacks/avatarpack32"  , "_32x32bw.png");
   
   ret = ret && CreateAvatars(avatar_atlas160  , avatars160  , 160 , 160);
   ret = ret && CreateAvatars(avatar_atlas64   , avatars64   , 64  , 64);
   ret = ret && CreateAvatars(avatar_atlas32   , avatars32   , 32  , 32);
   ret = ret && CreateAvatars(avatar_atlas32bw , avatars32bw , 32  , 32);
   
   return ret;
}



ALLEGRO_BITMAP* GetAvatar(int size , bool bw , int index) {
   ALLEGRO_BITMAP** avatars = 0;
   if (size == 160) {
      avatars = avatars160;
   }
   else if (size == 64) {
      avatars = avatars64;
   }
   else if (size == 32) {
      if (bw) {
         avatars = avatars32bw;
      }
      else {
         avatars = avatars32;
      }
   }
   else {
      return 0;
   }
   if (index < 0 || index >= NUM_AVATARS) {
      return 0;
   }
   return avatars[index];
}



