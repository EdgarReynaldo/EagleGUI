



#include "Eagle/backends/Allegro5/Allegro5Skybox.hpp"
#include "Eagle.hpp"
#include "GL/gl.h"
#include "GL/glu.h"

/// Microsoft only supports OpenGL 1.1 which doesn't include GL_CLAMP_TO_EDGE
#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F
#endif


Allegro5Skybox::Allegro5Skybox() :
      Skybox()
{}



void Allegro5Skybox::Render(EagleGraphicsContext* win , Vec3 pos) {
   glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , GL_CLAMP_TO_EDGE);
   win->RenderTexturedFacesFront(&sbcube.Inside() , SpatialInfo(pos , Orient()) , Vec3(500,500,500));
///      sbcube.Outside().RenderFaces(SpatialInfo() , Vec3(10,10,10));
}

#ifdef GL_CLAMP_TO_EDGE
#undef GL_CLAMP_TO_EDGE
#endif
