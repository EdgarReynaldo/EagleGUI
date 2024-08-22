



#include <cstdio>
#include <map>

#include "Eagle.hpp"
#include "Eagle/backends/Allegro5/Allegro5OpenGL.hpp"
#include "allegro5/allegro.h"
#include "allegro5/allegro_opengl.h"

#include "GL/gl.h"
#include "GL/glu.h"
#include "GL/glext.h"


bool bpoint = false;
bool cont = true;

unsigned int errcount = 0;



std::map<unsigned int , int> errmap;

void __stdcall
MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLbyte* message,
                 const void* userParam )
{
   unsigned int err = glGetError();
   fprintf( stdout, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
           ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
            type, severity, message );
//   fprintf(stdout , "glGetError reports error %lu\n" , glGetError());
   errmap[err]++;
   errcount++;
   if (bpoint) {
      printf("Last GL error reported was %u\n" , err);
   }
}



void SetupOpenGLDebug() {
   glEnable              ( GL_DEBUG_OUTPUT );
   glDebugMessageCallback( (GLDEBUGPROC)MessageCallback, 0 );

   al_set_new_display_option(ALLEGRO_OPENGL_MAJOR_VERSION , 2 , ALLEGRO_REQUIRE);
   al_set_new_display_option(ALLEGRO_OPENGL_MINOR_VERSION , 1 , ALLEGRO_REQUIRE);
   al_set_new_display_option(ALLEGRO_FLOAT_DEPTH , 1 , ALLEGRO_SUGGEST);
   al_set_new_display_option(ALLEGRO_DEPTH_SIZE , 32 , ALLEGRO_SUGGEST);
}



unsigned int ErrCount() {return errcount;}



int begin_count = 0;
void eglBegin(int32_t mode) {
   begin_count++;
   if (begin_count > 1) {
      printf("Too many glBegin calls!\n");
   }
   cont = true;
   glBegin((GLenum)mode);
   cont = false;
}



void eglEnd() {
   begin_count--;
   if (begin_count > 0) {
      printf("Unclosed glBegin!\n");
   }
   cont = true;
   glEnd();
   cont = false;
}



void DumpErrors() {
   for (std::map<unsigned int , int>::iterator it = errmap.begin() ; it != errmap.end() ; ++it) {
      printf("%d errors of type %u\n" , it->second , it->first);
   }
}


   


