


#ifndef LOL_HPP
#define LOL_HPP


#include "Eagle.hpp"
#include "Scene.hpp"




extern const char* lolbunnylogo;
extern const char* lolbunnysig;

extern const char* lolbunny;
extern const char* fatbunny;
extern const char* cutebunny;

extern const char* lolbytes;




#define NUM_TEST_FONTS 17

extern const char* const font_paths[NUM_TEST_FONTS];

void DrawLolText(EagleGraphicsContext* win , EagleImage* target , const char* str , EagleFont* font , bool scale);






class TextImage {
public :
   BasicText text;
   EagleGraphicsContext* win;
   EagleImage* buffer;
   const char* name;
   const char* text_str;
   int w;
   int h;
   
   
//   TextImage(EagleGraphicsContext* win , const char* name , const char* str , int width , int height);
   TextImage(const char* text_name , const char* str , int width , int height);
   ~TextImage();
   
   void Create(EagleGraphicsContext* window);

   void Draw(EagleFont* font , bool scale_to_fit);

};






class LolMaker : public Scene {
   EagleFont* text_fonts[NUM_TEST_FONTS];
   int font_num;
   bool scale_to_fit;
   bool display_widget_areas;
   
   TextImage bunny_logo;///192x64
   TextImage bunny_sig;/// 192x32
   TextImage lol_bunny;/// 32x32
   TextImage fat_bunny;/// 64x64
   TextImage cute_bunny;/// 32x32
   TextImage lol_byte;/// 64x64
   
   std::map<const char*, EagleImage**> buf_map;

public :
   
///   LolMaker();
   LolMaker() :
         text_fonts(),
         font_num(0),
         scale_to_fit(false),
         display_widget_areas(false),
         bunny_logo("Bunny_Logo" , lolbunnylogo , 384 , 128),
         bunny_sig ("Bunny_Sig"  , lolbunnysig  , 384 , 64),
         lol_bunny ("LOL_Bunny"  , lolbunny     , 64  , 64),
         fat_bunny ("Fat_Bunny"  , fatbunny     , 128  , 128),
         cute_bunny("Cute_Bunny" , cutebunny    , 64  , 64),
         lol_byte  ("LOL_Bytes"  , lolbytes     , 64  , 64),
         buf_map()
   {
      buf_map[bunny_logo.name] = &(bunny_logo.buffer);
      buf_map[bunny_sig.name] = &(bunny_sig.buffer);
      buf_map[lol_bunny.name] = &(lol_bunny.buffer);
      buf_map[fat_bunny.name] = &(fat_bunny.buffer);
      buf_map[cute_bunny.name] = &(cute_bunny.buffer);
      buf_map[lol_byte.name] = &(lol_byte.buffer);
   }

   EagleImage* GetImage(const char* name) {return *buf_map[name];}
   
   bool SaveImages();
   
   
   virtual bool Init(EagleGraphicsContext* window);
   virtual void Free(EagleGraphicsContext* window);
   
   virtual void Display(EagleGraphicsContext* window);
   virtual int HandleEvent(EagleEvent ev);
   virtual int Update(double tsec);


};


#endif // LOL_HPP

