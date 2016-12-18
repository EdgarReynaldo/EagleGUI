




#include "allegro5/allegro.h"


const int NUM_AVATARS = 12;

extern const char* avatar_names[NUM_AVATARS];

extern ALLEGRO_BITMAP* avatars160[NUM_AVATARS];
extern ALLEGRO_BITMAP* avatars64[NUM_AVATARS];
extern ALLEGRO_BITMAP* avatars32[NUM_AVATARS];
extern ALLEGRO_BITMAP* avatars32bw[NUM_AVATARS];

extern ALLEGRO_BITMAP* avatar_atlas160;
extern ALLEGRO_BITMAP* avatar_atlas64;
extern ALLEGRO_BITMAP* avatar_atlas32;
extern ALLEGRO_BITMAP* avatar_atlas32bw;

bool CreateAvatarAtlas(ALLEGRO_BITMAP*& atlas , int w , int h , const char* directory , const char* const type = "_160x160.png");
bool CreateAvatars(ALLEGRO_BITMAP* atlas , ALLEGRO_BITMAP** store_sub_avatars , int w , int h);

bool PopulateAvatars();

ALLEGRO_BITMAP* GetAvatar(int size , bool bw , int index);
















