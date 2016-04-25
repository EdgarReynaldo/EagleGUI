
/*
 *
 *     _______       ___       ____      __       _______
 *    /\  ____\    /|   \     /  __\    /\ \     /\  ____\
 *    \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_
 *     \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\
 *      \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_
 *       \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *        \/______/|/_/  \/_/  \_\_____/    \/_____/ \/______/
 *
 *
 *    EAGLE
 *    Edgar's Agile Gui Library and Extensions
 *
 *    Copyright 2009-2016+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */



#ifndef EagleGraphics_HPP
#define EagleGraphics_HPP

#include <list>
#include <string>

#include "Eagle/Color.hpp"
#include "Eagle/Events.hpp"
#include "Eagle/Image.hpp"
#include "Eagle/Font.hpp"
#include "Eagle/Container.hpp"
#include "Eagle/Area.hpp"
#include "Eagle/MousePointer.hpp"


enum EAGLE_DISPLAY_FLAGS {
   EAGLE_WINDOWED                    = 1 << 0,
   EAGLE_FULLSCREEN                  = 1 << 1,
   EAGLE_OPENGL                      = 1 << 2,
   EAGLE_DIRECT3D                    = 1 << 3,
   EAGLE_RESIZABLE                   = 1 << 4,
   EAGLE_NOFRAME                     = 1 << 5,
   EAGLE_GENERATE_EXPOSE_EVENTS      = 1 << 6,
   EAGLE_OPENGL_3_0                  = 1 << 7,
   EAGLE_OPENGL_FORWARD_COMPATIBLE   = 1 << 8,
   EAGLE_FULLSCREEN_WINDOW           = 1 << 9,
   EAGLE_MINIMIZED                   = 1 << 10,
   EAGLE_USE_PROGRAMMABLE_PIPELINE   = 1 << 11
};


enum IMAGE_DRAWING_FLAGS {
   DRAW_NORMAL = 0,
   DRAW_HFLIP = 1,
   DRAW_VFLIP = 2,
   DRAW_HVFLIP = 3
};


enum TEXT_HDRAWING_FLAGS {
   DRAW_TEXT_LEFT = 0,
   DRAW_TEXT_CENTER = 1,
   DRAW_TEXT_RIGHT = 2
};

enum TEXT_VDRAWING_FLAGS {
   DRAW_TEXT_TOP = 0,
   DRAW_TEXT_VCENTER = 1,
   DRAW_TEXT_BOTTOM = 2
};



extern unsigned int GUI_TEXT_LINE_SPACING;


class REGION_INFO {
   
public :
   REGION_INFO();
   REGION_INFO(float srcx , float srcy , float srcw , float srch);

   float sx;
   float sy;
   float sw;
   float sh;
};



class SCALE_INFO {

public :
   SCALE_INFO();
   SCALE_INFO(float xscale , float yscale);

   float x;
   float y;
};



class RESIZE_INFO {

public :
   RESIZE_INFO();
   RESIZE_INFO(float destx , float desty , float destw , float desth);
   
   float dx;
   float dy;
   float dw;
   float dh;
   
};// destination rectangle



class ROTATE_INFO {

public :
   ROTATE_INFO();
   ROTATE_INFO(float pivotx , float pivoty , float destx , float desty , float theta);

   float cx;
   float cy;
   float dx;
   float dy;
   float angle;
};



class EagleDrawingInfo {

private :
   void CheckUse();
   
public :

   EagleDrawingInfo();
   EagleDrawingInfo(float destx , float desty);

   bool use_any;
   bool use_region;
   bool use_scale;
   bool use_resize;// if false, destination point will be used, which is default
   bool use_rotate;
   bool use_tint;
   
   float dx;
   float dy;
   
   REGION_INFO region;// sx sy sw sh
   SCALE_INFO scale;// x y
   RESIZE_INFO resize;// dx dy dw dh
   ROTATE_INFO rotate;// cx cy dx dy angle
   EagleColor tint;// r g b a
   int flags;// DRAW_HFLIP DRAW_VFLIP DRAW_HVFLIP
   
   void SetDest(float x , float y);// overrides resize, scale, and rotate
   void SetRegion(REGION_INFO r);// sets source region
   void SetScale(SCALE_INFO s);// only used with rotate, overrides resize
   void SetResize(RESIZE_INFO r);// overrides scale and rotate, latest setting wins
   void SetRotate(ROTATE_INFO r);// overrides resize
   void SetTintColor(EagleColor c);
   void SetFlags(int f);


   void ClearSettings();
};



class Rectangle;



class EagleGraphicsContext : public EagleObject {

protected :
   int scrw;
   int scrh;
   
   EagleImage* backbuffer;
   EagleImage* drawing_target;
   
   std::list<EagleImage*> draw_target_stack;
   
   PointerManager<EagleImage> images;
   PointerManager<EagleFont> fonts;
   
   MousePointerManager* mp_manager;// Derived class is responsible for instantiating this object, b/c a virtual creation function
                                   // cannot be called in a base class constructor
   
public :
   EagleGraphicsContext(std::string name);
   
   virtual ~EagleGraphicsContext() {}

   // creation/destruction
   virtual bool Create(int width , int height , int flags)=0;
   virtual bool Valid()=0;
   virtual void Destroy()=0;
   virtual void AcknowledgeResize()=0;

   // clears target bitmap
   virtual void Clear(EagleColor c)=0;
   
   // Query
   float Width() {return scrw;}
   float Height() {return scrh;}
   
   // Blender setting functions
   virtual void SetCopyBlender()=0;
   virtual void SetPMAlphaBlender()=0;
   virtual void SetNoPMAlphaBlender()=0;
   virtual void RestoreLastBlendingState()=0;
   
   // basic drawing operations
   virtual void PutPixel(int x , int y , EagleColor c)=0;
   
   virtual void DrawLine(int x1 , int y1 , int x2 , int y2 , EagleColor c)=0;
   
   virtual void DrawRectangle(int x , int y , int w , int h , int thickness , EagleColor c)=0;
   void         DrawRectangle(Rectangle r , int thickness , EagleColor c);

   virtual void DrawFilledRectangle(int x , int y , int w , int h , EagleColor c)=0;
   void         DrawFilledRectangle(Rectangle r , EagleColor c);

   virtual void DrawRoundedRectangle(int x , int y , int w , int h , int rx , int ry , EagleColor c)=0;
   void         DrawRoundedRectangle(Rectangle r , int rx , int ry , EagleColor c);

   virtual void DrawFilledRoundedRectangle(int x , int y , int w , int h , int rx , int ry , EagleColor c)=0;
   void         DrawFilledRoundedRectangle(Rectangle r , int rx , int ry , EagleColor c);

   virtual void DrawCircle(int cx , int cy , int radius , int thickness , EagleColor c)=0;
   virtual void DrawFilledCircle(int cx , int cy , int radius , EagleColor c)=0;
   
   virtual void DrawEllipse(int cx , int cy , int rx , int ry , int thickness , EagleColor c)=0;
   virtual void DrawFilledEllipse(int cx , int cy , int rx , int ry , EagleColor c)=0;
   
   virtual void DrawTriangle(int x1 , int y1 , int x2 , int y2 , int x3 , int y3 , int thickness , EagleColor c)=0;
   virtual void DrawFilledTriangle(int x1 , int y1 , int x2 , int y2 , int x3 , int y3 , EagleColor c)=0;
   
   // precise drawing operations
   virtual void PutPixel(float x , float y , EagleColor c)=0;
   virtual void DrawLine(float x1 , float y1 , float x2 , float y2 , float thickness , EagleColor c)=0;
   virtual void DrawRectangle(float x , float y , float w , float h , float thickness , EagleColor c)=0;
   virtual void DrawFilledRectangle(float x , float y , float w , float h , EagleColor c)=0;
   virtual void DrawRoundedRectangle(float x , float y , float w , float h , float rx , float ry , float thickness , EagleColor c)=0;
   virtual void DrawFilledRoundedRectangle(float x , float y , float w , float h , float rx , float ry , EagleColor c)=0;
   virtual void DrawCircle(float cx , float cy , float radius , float thickness , EagleColor c)=0;
   virtual void DrawFilledCircle(float cx , float cy , float radius , EagleColor c)=0;
   virtual void DrawTriangle(float x1 , float y1 , float x2 , float y2 , float x3 , float y3 , float thickness , EagleColor c)=0;
   virtual void DrawFilledTriangle(float x1 , float y1 , float x2 , float y2 , float x3 , float y3 , EagleColor c)=0;
	virtual void DrawShadedRectangle(const Rectangle* r , EagleColor tl , EagleColor tr , EagleColor br , EagleColor bl)=0;
	virtual void DrawShadedQuad(float x1 , float y1 , EagleColor c1 ,
										 float x2 , float y2 , EagleColor c2 ,
										 float x3 , float y3 , EagleColor c3 , 
										 float x4 , float y4 , EagleColor c4)=0;

   // image drawing operations
   virtual void Draw(EagleImage* img , float x , float y , int flags = DRAW_NORMAL)=0;
   virtual void DrawRegion(EagleImage* img , Rectangle src , float x , float y , int flags = DRAW_NORMAL)=0;
   virtual void DrawStretchedRegion(EagleImage* img , float sx , float sy , float sw , float sh ,
                                                      float dx , float dy , float dw , float dh , int flags = DRAW_NORMAL)=0;
   void         DrawStretchedRegion(EagleImage* img , Rectangle src , Rectangle dest , int flags = DRAW_NORMAL);
   virtual void ConvertColorToAlpha(EagleImage* img , EagleColor alpha_color)=0;
/// TODO  virtual void Draw(EagleImage* src , EagleDrawingInfo info = EagleDrawingInfo())=0;

   // text drawing operations
   virtual void DrawTextString(EagleFont* font , std::string s , float x , float y , EagleColor c ,
                               TEXT_HDRAWING_FLAGS halign = DRAW_TEXT_LEFT ,
                               TEXT_VDRAWING_FLAGS valign = DRAW_TEXT_TOP)=0;

   void DrawGuiTextString(EagleFont* font , std::string s , float x , float y , EagleColor c ,
                          TEXT_HDRAWING_FLAGS halign = DRAW_TEXT_LEFT ,
                          TEXT_VDRAWING_FLAGS valign = DRAW_TEXT_TOP);


   // getters
   virtual EagleImage* GetBackBuffer()=0;
   virtual EagleImage* GetScreen()=0;
   virtual EagleImage* GetDrawingTarget()=0;
   
   // utilities
   virtual void FlipDisplay()=0;
   virtual void HoldDrawing()=0;
   virtual void ReleaseDrawing()=0;
protected :
   virtual void SetDrawingTarget(EagleImage* dest)=0;
public :
   void DrawToBackBuffer();
   
   // image creation / loading / sub division
   virtual EagleImage* EmptyImage()=0;
   virtual EagleImage* CloneImage(EagleImage* clone)=0;
   virtual EagleImage* CreateImage(int width , int height , IMAGE_TYPE type = VIDEO_IMAGE)=0;
   virtual EagleImage* LoadImageFromFile(std::string file , IMAGE_TYPE type = VIDEO_IMAGE)=0;
   virtual EagleImage* CreateSubImage(EagleImage* parent , int x , int y , int width , int height)=0;
   void                FreeImage(EagleImage* img);
   
   // font loading
   virtual EagleFont* LoadFont(std::string file , int height , int flags , IMAGE_TYPE type = VIDEO_IMAGE)=0;
   void               FreeFont(EagleFont* font);
   
   // event handler registration
   virtual void RegisterDisplayInput(EagleEventHandler* queue)=0;
   
   // mouse control
//   MousePointerManager* GetMousePointerManager() {return mp_manager;}
   
   void ShowMouse();
   void HideMouse();
   
   bool AcquireMousePointer(void* caller , MOUSE_POINTER_TYPE ptype , bool use_system_pointer);
   void ReleaseMousePointer(const void* caller);
   void ReAcquireMouse();// call when window loses and regains focus if there is more than one, as the mp is shared
   
   bool SetCustomPointer(MousePointerInfo info);
//   void SetCustomPointerSet(MousePointerSet* pointer_set);
   
   void SetMousePosition(int mousex , int mousey);
   
   
   // drawing target
   void PushDrawingTarget(EagleImage* img);
   void PopDrawingTarget();
   
   /// TODO : Move clipping support here, since it operates on a target bitmap
   
};


/// Simple class to automatically push and pop drawing targets onto the drawing target stack
/// DrawingTargeter drawing_targeter(my_win , my_image);// pushes my_image onto my_win's drawing stack
/// Draw to my_image
/// drawing_targeter goes out of scope and pops my_image off the drawing stack
class DrawingTargeter {

private :
   EagleGraphicsContext* win;
   
public :
   DrawingTargeter(EagleGraphicsContext* window , EagleImage* target_image);// pushes target_image onto the drawing stack
   ~DrawingTargeter();// pops target image off drawing stack automatically when this goes out of scope.
   
   
};



#endif // EagleGraphics_HPP

