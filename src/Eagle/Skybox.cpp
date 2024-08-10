



#include "Eagle/Exception.hpp"
#include "Eagle/Skybox.hpp"
#include "Eagle/GraphicsContext.hpp"
#include "Eagle/Image.hpp"
#include "Eagle/StringWork.hpp"
#include "Eagle/Lib.hpp"


Skybox::Skybox() :
      sbcube(),
      sbimage(0),
      sbfaces()
{
   
   for(unsigned int i = 0 ; i < NUM_CUBE_FACES ; ++i) {sbfaces[i] = 0;}
}



Skybox::~Skybox() {
   Clear();
}



void Skybox::Clear() {
   
   for(unsigned int i = 0 ; i < NUM_CUBE_FACES ; ++i) {
      if (sbfaces[i]) {
         sbfaces[i]->ParentContext()->FreeImage(sbfaces[i]);
         sbfaces[i] = 0;
      }
   }
   if (sbimage) {
      sbimage->ParentContext()->FreeImage(sbimage);
      sbimage = 0;
   }
}



void Skybox::MakeSkybox(EagleGraphicsContext* win , std::string skybox_file) {
   Clear();
   
   sbimage = win->LoadImageFromFile(skybox_file);
   EAGLE_ASSERT(sbimage && sbimage->Valid());
   
   int tw = sbimage->W();
   int th = sbimage->H();
   EAGLE_ASSERT(tw*3 == th*4);/// Enforce aspect ratio of skybox image - it should be a grid of 4x3 squares, any size
/*   
enum CUBE_FACE {
   CUBE_FACE_LEFT   = 0,
   CUBE_FACE_RIGHT  = 1,
   CUBE_FACE_TOP    = 2,
   CUBE_FACE_BOTTOM = 3,
   CUBE_FACE_BACK   = 4,
   CUBE_FACE_FRONT  = 5,
   NUM_CUBE_FACES   = 6
};
*/
   const int x[NUM_CUBE_FACES] = {
      0 , tw/2 , tw/4 , tw/4 , tw/4 , 3*tw/4
   };
   const int y[NUM_CUBE_FACES] = {
      th/3 , th/3 , 0 , 2*th/3 , th/3 , th/3
   };
   const int w = tw/4;
   const int h = th/3;
   EAGLE_ASSERT(w == h);/// Each side of the cube must be square
   
   for(unsigned int i = 0 ; i < NUM_CUBE_FACES ; ++i) {
      EagleImage* sub = win->CreateSubImage(sbimage , x[i] , y[i] , w , h , StringPrintF("Subbitmap#%d" , i));
      sbfaces[i] = sub;
   }
   sbcube.Texture(sbfaces);
}












