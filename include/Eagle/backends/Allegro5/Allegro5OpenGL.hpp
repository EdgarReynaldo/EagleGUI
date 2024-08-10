



#ifndef Allegro5OpenGL_HPP
#define Allegro5OpenGL_HPP


#include <cstdint>


void SetupOpenGLDebug();

unsigned int ErrCount();

extern bool bpoint;
extern bool cont;

void DumpErrors();

void eglBegin(int32_t mode);
void eglEnd();

#endif // Allegro5OpenGL_HPP




