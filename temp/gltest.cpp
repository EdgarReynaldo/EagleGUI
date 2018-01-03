


#include "allegro5/allegro.h"
#include "allegro5/allegro_primitives.h"
#include "allegro5/allegro_opengl.h"
#include "allegro5/allegro_color.h"


#include "GL/glu.h"
#include <cmath>
#include <cstdio>


int ww = 800;
int wh = 600;
    

void resetOpenGLmatrices();

struct XYZ {
    float x,y,z;
};

XYZ uc[8] = {
    {-0.5f,-0.5f,-0.5f},
    {-0.5f, 0.5f,-0.5f},
    { 0.5f, 0.5f,-0.5f},
    { 0.5f,-0.5f,-0.5f},
    { 0.5f,-0.5f, 0.5f},
    { 0.5f, 0.5f, 0.5f},
    {-0.5f, 0.5f, 0.5f},
    {-0.5f,-0.5f, 0.5f}
};

struct RGB {
    float r,g,b,a;
};

RGB cc1[8] = {
    {1.0f,1.0f,1.0f,1.0f},
    {1.0f,0.0f,0.0f,1.0f},
    {0.0f,1.0f,0.0f,1.0f},
    {0.0f,0.0f,1.0f,1.0f},
    {1.0f,1.0f,0.0f,1.0f},
    {0.0f,0.0f,0.0f,1.0f},
    {0.0f,1.0f,1.0f,1.0f},
    {1.0f,0.0f,1.0f,1.0f}
};

RGB cc[8] = {
    {1.0f,1.0f,1.0f,1.0f},
    {1.0f,0.0f,0.0f,1.0f},
    {0.0f,0.0f,1.0f,1.0f},
    {0.0f,1.0f,1.0f,1.0f},
    {1.0f,1.0f,0.0f,1.0f},
    {0.0f,0.0f,0.0f,1.0f},
    {0.0f,1.0f,1.0f,1.0f},
    {1.0f,0.0f,1.0f,1.0f}
};

struct TEX2D {
    float u,v;
};

TEX2D tex2d[4] = {
    {0.0,0.0},
    {0.0,1.0},
    {1.0,1.0},
    {1.0,0.0}
};

struct TRI {
   XYZ* p1;
   XYZ* p2;
   XYZ* p3;
   RGB* c1;
   RGB* c2;
   RGB* c3;
   TEX2D* tuv1;
   TEX2D* tuv2;
   TEX2D* tuv3;
   
};

TRI cube_faces[12] = {
    {&uc[0] , &uc[1] , &uc[2], &cc[0] , &cc[1] , &cc[2] , &tex2d[0] , &tex2d[1] , &tex2d[2]},
    {&uc[2] , &uc[3] , &uc[0], &cc[2] , &cc[3] , &cc[0] , &tex2d[2] , &tex2d[3] , &tex2d[0]},
    {&uc[3] , &uc[2] , &uc[5], &cc[3] , &cc[2] , &cc[5] , &tex2d[0] , &tex2d[1] , &tex2d[2]},
    {&uc[5] , &uc[4] , &uc[3], &cc[5] , &cc[4] , &cc[3] , &tex2d[2] , &tex2d[3] , &tex2d[0]},
    {&uc[4] , &uc[5] , &uc[6], &cc[4] , &cc[5] , &cc[6] , &tex2d[0] , &tex2d[1] , &tex2d[2]},
    {&uc[6] , &uc[7] , &uc[4], &cc[6] , &cc[7] , &cc[4] , &tex2d[2] , &tex2d[3] , &tex2d[0]},
    {&uc[7] , &uc[6] , &uc[1], &cc[7] , &cc[6] , &cc[1] , &tex2d[0] , &tex2d[1] , &tex2d[2]},
    {&uc[1] , &uc[0] , &uc[7], &cc[1] , &cc[0] , &cc[7] , &tex2d[2] , &tex2d[3] , &tex2d[0]},
    {&uc[0] , &uc[3] , &uc[4], &cc[0] , &cc[3] , &cc[4] , &tex2d[0] , &tex2d[1] , &tex2d[2]},
    {&uc[4] , &uc[7] , &uc[0], &cc[4] , &cc[7] , &cc[0] , &tex2d[2] , &tex2d[3] , &tex2d[0]},
    {&uc[1] , &uc[6] , &uc[5], &cc[1] , &cc[6] , &cc[5] , &tex2d[0] , &tex2d[1] , &tex2d[2]},
    {&uc[5] , &uc[2] , &uc[1], &cc[5] , &cc[2] , &cc[1] , &tex2d[2] , &tex2d[3] , &tex2d[0]}
};


int main(int argc , char** argv) {
    
    if (!al_init()) {return 1;}
    if (!al_init_primitives_addon()) {return 2;}
    
    if (!al_install_keyboard()) {return 3;}
    if (!al_install_mouse()) {return 4;}
    
    al_set_new_display_option(ALLEGRO_DEPTH_SIZE , ALLEGRO_SUGGEST , 24);
    
    al_set_new_display_flags(ALLEGRO_RESIZABLE | ALLEGRO_OPENGL | ALLEGRO_WINDOWED);
    
    ALLEGRO_DISPLAY* d = al_create_display(ww,wh);
    
    if (!d) {return -1;}
    
    ALLEGRO_TIMER* t = al_create_timer(1.0/60.0);
    
    if (!t) {return -2;}
    
    ALLEGRO_EVENT_QUEUE* q = al_create_event_queue();
    
    if (!q) {return -3;}
    
    al_register_event_source(q , al_get_display_event_source(d));
    al_register_event_source(q , al_get_timer_event_source(t));
    al_register_event_source(q , al_get_keyboard_event_source());
    al_register_event_source(q , al_get_mouse_event_source());
    
    int tw = 256;
    ALLEGRO_BITMAP* atex = al_create_bitmap(tw,tw);
    
    al_set_target_bitmap(atex);
    al_clear_to_color(al_map_rgba(255,255,255,255));

/*
    for (int i = tw/2 ; i >=1  ; --i) {
        float c = (2.0f*i/tw);
        al_draw_filled_circle(tw/2,tw/2,i,al_map_rgba_f(c,c,c,1.0f));
    }
*/

    al_draw_filled_rectangle(0,0,tw/2,tw/2 , al_map_rgba(0,0,0,255));
    al_draw_filled_rectangle(tw/2,tw/2,tw,tw,al_map_rgba(0,0,0,255));
        
/**
//   Uncomment the following 3 lines to make the program display properly
    al_set_target_backbuffer(d);
    al_clear_to_color(al_map_rgb(0,0,255));
    al_flip_display();
//*/
    
    float rotv = 0.5;
    float rotx = 0.0;
    float roty = 0.0;
    float rotz = 0.0;
    float rotxv = 4*rotv;
    float rotyv = 2*rotv;
    float rotzv = rotv;
    float rotlimit = 360;
    
    bool redraw = true;
    bool quit = false;
    
    int shrink = 0;
    int shrink_dir = 1;
    
    al_start_timer(t);
    
    al_clear_to_color(al_map_rgb(0,0,0));
    al_flip_display();
    
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDisable(GL_LIGHTING);
    glFrontFace(GL_CCW);
    glCullFace(GL_NONE);
    
    while (!quit) {
        if (redraw) {
            al_set_target_backbuffer(d);
            al_clear_to_color(al_map_rgb(255,255,255));
        
            glClear(GL_DEPTH_BUFFER_BIT);
        
            resetOpenGLmatrices();
            
            float scale = 1.0f/(ww/2 - shrink);
            scale = 1.0;
            glRotatef(rotz , 0 , 0 , 1);
            glRotatef(rotx , 1 , 0 , 0);
            glRotatef(roty , 0 , 1 , 0);
            glScalef(scale , scale , scale);
///            glTranslatef(0,0,ww/2);
            
            GLuint texid = al_get_opengl_texture(atex);
            glBindTexture(GL_TEXTURE_2D , texid);

            glBegin(GL_TRIANGLES);
            int i = 0;
            while (i < 12) {
                TRI* face = &cube_faces[i];
                int j = 0;
                XYZ* f[3] = {face->p1 , face->p2 , face->p3};
                RGB* col[3] = {face->c1 , face->c2 , face->c3};
                TEX2D* txy[3] = {face->tuv1 , face->tuv2 , face->tuv3};

                while (j < 3) {
                    XYZ* p = f[j];
                    RGB* c = col[j];
                    TEX2D* t = txy[j];
                    glColor4f(c->r,c->g,c->b,1.0f);
                    glTexCoord2f(t->u , t->v);
                    glVertex3f(p->x , p->y , p->z);
                    ++j;
                }
                ++i;
            }
            
            glEnd();
            al_flip_display();
            
            redraw = false;
        }
        do {
            ALLEGRO_EVENT ev;
            al_wait_for_event(q , &ev);
            if (ev.type == ALLEGRO_EVENT_DISPLAY_RESIZE) {
                al_acknowledge_resize(d);
                ww = al_get_display_width(d);
                wh = al_get_display_height(d);
            }
            if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {quit = true;}
            if (ev.type == ALLEGRO_EVENT_KEY_DOWN && ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {quit = true;}
            if (ev.type == ALLEGRO_EVENT_TIMER) {
                redraw = true;
                shrink += shrink_dir;
                if (shrink > ww/2) {
                    shrink = ww/2;
                    shrink_dir = -1;
                }
                if (shrink < 0) {
                    shrink = 0;
                    shrink_dir = 1;
                }
                rotx += rotxv;
                rotx = fmod(rotx , rotlimit);
                roty += rotyv;
                roty = fmod(roty , rotlimit);
                rotz += rotzv;
                rotz = fmod(rotz , rotlimit);
            }
/*
            if (ev.type == ALLEGRO_EVENT_MOUSE_AXES) {
                roty = 360.0f*((ww/2) - ev.mouse.x)/(ww/2);
                rotx = 360.0f*((wh/2) - ev.mouse.y)/(wh/2);
                rotz += 30.0f*ev.mouse.dz;
                rotz = fmod(rotz , rotlimit);
                redraw = true;
            }
*/
        } while (!al_is_event_queue_empty(q));
        
    }
    
    
    return 0;
}


void resetOpenGLmatrices() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
///    glOrtho(0 , ww , 0 , wh , -1 , 1);
///    glOrtho(-2 , 2 , -2 , 2 , -2 , 2);
///    glFrustum(-ww/2 , ww/2 , -wh/2 , wh/2 , 0 , ww);
///    glFrustum(-1,1,-1,1,1.5,11.5);
    gluPerspective(90 , 4/3.0f , 1 , 3);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0 , 0 , -2 , 0 , 0 , 0 , 0 , 1 , 0);
}



