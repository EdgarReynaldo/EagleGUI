

#ifndef Allegro5GraphicsContext_HPP
#define Allegro5GraphicsContext_HPP



#include "Eagle/GraphicsContext.hpp"

#include "Eagle/backends/Allegro5/Allegro5Image.hpp"
#include "Eagle/backends/Allegro5/Allegro5Transform.hpp"
#include "Eagle/backends/Allegro5/Allegro5Threads.hpp"
#include "Eagle/backends/Allegro5/Allegro5Mutex.hpp"


#include "allegro5/allegro.h"
#include "allegro5/allegro_primitives.h"

#include <vector>


//#ifdef LoadImage
//   #warning "LoadImage already defined @@@"
//#endif

ALLEGRO_VERTEX MakeAllegro5Vertex(float x , float y , float z , float u , float v , ALLEGRO_COLOR ac);



typedef struct BLENDER {
   int blender_op;
   int blender_src;
   int blender_dest;
} BLENDER;



EagleGraphicsContext* GetAssociatedContext(ALLEGRO_DISPLAY* display);


EagleEvent GetDisplayEvent(ALLEGRO_EVENT ev);


///void* A5WindowProcess(EagleThread* thread , void* context);




class Allegro5GraphicsContext : public EagleGraphicsContext {
   
private :
   
   ALLEGRO_DISPLAY* display;
   Allegro5Image realbackbuffer;
   
   int blender_op;
   int blender_src;
   int blender_dest;
   std::vector<BLENDER> blender_stack;

   Allegro5Transformer allegro5transformer;


///   Allegro5Thread window_thread;

///   friend void* A5WindowProcess(EagleThread* thread , void* context);

///   ALLEGRO_EVENT_SOURCE window_event_source;
///   ALLEGRO_EVENT_QUEUE* window_queue;

   
   void Init();

   void ResetBackBuffer();
   virtual void PrivateFlipDisplay();

   friend class Allegro5WindowManager;

   Allegro5GraphicsContext();
   Allegro5GraphicsContext(int width , int height , int flags);

public :


   ~Allegro5GraphicsContext();

   virtual EagleSystem* GetSystem();
   

   /// creation/destruction
   virtual bool Create(int width , int height , int flags);
   virtual bool Valid();
   virtual void Destroy();
   virtual void AcknowledgeResize();

   virtual int XPos();
   virtual int YPos();

   /// Load Default font
   void LoadDefaultFont();

   /// clears target bitmap
   virtual void Clear(EagleColor c = EagleColor(0,0,0));
   
   /// Blender setting functions
   void SetCopyBlender();
   void SetPMAlphaBlender();
   void SetNoPMAlphaBlender();
   void RestoreLastBlendingState();
protected:
   
   void StoreBlender();
   void RestoreBlender();
public :


   /// basic drawing operations
   virtual void PutPixel(int x , int y , EagleColor c);
   virtual void DrawLine(int x1 , int y1 , int x2 , int y2 , EagleColor c);
   virtual void DrawRectangle(int x , int y , int w , int h , int thickness , EagleColor c);
   virtual void DrawFilledRectangle(int x , int y , int w , int h , EagleColor c);
   virtual void DrawRoundedRectangle(int x , int y , int w , int h , int rx , int ry , EagleColor c);
   virtual void DrawFilledRoundedRectangle(int x , int y , int w , int h , int rx , int ry , EagleColor c);
   virtual void DrawCircle(int cx , int cy , int radius , int thickness , EagleColor c);
   virtual void DrawFilledCircle(int cx , int cy , int radius , EagleColor c);
   virtual void DrawEllipse(int cx , int cy , int rx , int ry , int thickness , EagleColor c);
   virtual void DrawFilledEllipse(int cx , int cy , int rx , int ry , EagleColor c);
   virtual void DrawTriangle(int x1 , int y1 , int x2 , int y2 , int x3 , int y3 , int thickness , EagleColor c);
   virtual void DrawFilledTriangle(int x1 , int y1 , int x2 , int y2 , int x3 , int y3 , EagleColor c);
   
   /// precise drawing operations
   virtual void PutPixel(float x , float y , EagleColor c);
   virtual void DrawLine(float x1 , float y1 , float x2 , float y2 , float thickness , EagleColor c);
   virtual void DrawRectangle(float x , float y , float w , float h , float thickness , EagleColor c);
   virtual void DrawFilledRectangle(float x , float y , float w , float h , EagleColor c);
   virtual void DrawRoundedRectangle(float x , float y , float w , float h , float rx , float ry , float thickness , EagleColor c);
   virtual void DrawFilledRoundedRectangle(float x , float y , float w , float h , float rx , float ry , EagleColor c);
   virtual void DrawCircle(float cx , float cy , float radius , float thickness , EagleColor c);
   virtual void DrawFilledCircle(float cx , float cy , float radius , EagleColor c);
   virtual void DrawEllipse(float cx , float cy , float rx , float ry , float thickness , EagleColor c);
   virtual void DrawFilledEllipse(float cx , float cy , float rx , float ry , EagleColor c);
   virtual void DrawTriangle(float x1 , float y1 , float x2 , float y2 , float x3 , float y3 , float thickness , EagleColor c);
   virtual void DrawFilledTriangle(float x1 , float y1 , float x2 , float y2 , float x3 , float y3 , EagleColor c);
	virtual void DrawShadedRectangle(const Rectangle* r , EagleColor tl , EagleColor tr , EagleColor br , EagleColor bl);
	virtual void DrawShadedQuad(float x1 , float y1 , EagleColor c1 ,
										 float x2 , float y2 , EagleColor c2 ,
										 float x3 , float y3 , EagleColor c3 , 
										 float x4 , float y4 , EagleColor c4);

   /// image drawing operations
   virtual void Draw(EagleImage* img , float x , float y , int flags = DRAW_NORMAL);
   virtual void DrawRegion(EagleImage* img , Rectangle src , float x , float y , int flags = DRAW_NORMAL);
   virtual void DrawStretchedRegion(EagleImage* img , float sx , float sy , float sw , float sh ,
                                                      float dx , float dy , float dw , float dh , int flags = DRAW_NORMAL);
   virtual void DrawTinted(EagleImage* img , int x , int y , EagleColor col = EagleColor(255,255,255,255));

   virtual void ConvertColorToAlpha(EagleImage* img , EagleColor alpha_color);
///   void DrawStretchedRegion(EagleImage* img , Rectangle src , Rectangle dest , int flags = DRAW_NORMAL);
/// TODO ADD   virtual void Draw(EagleImage* src , EagleDrawingInfo info);

   /// text drawing operations
///#ifdef DrawText
///   #warning "DrawText defined as " ## DrawText
///#endif
   virtual void DrawTextString(EagleFont* font , std::string str , float x , float y , EagleColor c,
                               HALIGNMENT halign = HALIGN_LEFT ,
                               VALIGNMENT valign = VALIGN_TOP);


   /// getters
   virtual EagleImage* GetBackBuffer();
   virtual EagleImage* GetScreen();
   virtual EagleImage* GetDrawingTarget();
   
   /// utilities
   virtual void HoldDrawing();
   virtual void ReleaseDrawing();
   virtual void SetDrawingTarget(EagleImage* dest);
   
   /// image creation / loading / sub division
   EagleImage* EmptyImage();
   EagleImage* AdoptImage(ALLEGRO_BITMAP* img);
   EagleImage* ReferenceImage(ALLEGRO_BITMAP* img);
   EagleImage* CloneImage(EagleImage* img);
   EagleImage* CreateImage(int width , int height , IMAGE_TYPE type = VIDEO_IMAGE);
   EagleImage* LoadImageFromFile(std::string file , IMAGE_TYPE type = VIDEO_IMAGE);
   EagleImage* CreateSubImage(EagleImage* parent , int x , int y , int width , int height);

   /// font loading
   EagleFont* LoadFont(std::string file , int height , int flags = LOAD_FONT_NORMAL , IMAGE_TYPE type = VIDEO_IMAGE);

   ALLEGRO_DISPLAY* AllegroDisplay() {return display;}

/*   NO, register directly below <.>
   /// Allegro event source
   void ReadEvents();
*/
   Transformer* GetTransformer();

   virtual void MakeDisplayCurrent();
};



#endif // Allegro5GraphicsContext_HPP
