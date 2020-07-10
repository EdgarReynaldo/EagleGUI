
/**
 *
 *         _______       ___       ____      __       _______
 *        /\  ____\    /|   \     /  __\    /\ \     /\  ____\
 *        \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_
 *         \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\
 *          \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_
 *           \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *            \/______/|/_/  \/_/  \_\_____/    \/_____/ \/______/
 *
 *
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2019+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file Allegro5GraphicsContext.hpp
 * @brief The interface for the Allegro 5 graphics driver
 */

#ifndef Allegro5GraphicsContext_HPP
#define Allegro5GraphicsContext_HPP



#include "Eagle/GraphicsContext.hpp"
#include "Eagle/Mutexes.hpp"

#include "Eagle/backends/Allegro5/Allegro5Image.hpp"
#include "Eagle/backends/Allegro5/Allegro5Transform.hpp"
#include "Eagle/backends/Allegro5/Allegro5Threads.hpp"

#include "allegro5/allegro.h"
#include "allegro5/allegro_primitives.h"

#include <vector>



ALLEGRO_VERTEX MakeAllegro5Vertex(float x , float y , float z , float u , float v , ALLEGRO_COLOR ac);///< Helper function



typedef struct BLENDER {
   int blender_op;
   int blender_src;
   int blender_dest;
} BLENDER;



EagleGraphicsContext* GetAssociatedContext(ALLEGRO_DISPLAY* display);///< Get the associated window from an ALLEGRO_DISPLAY*


EagleEvent GetDisplayEvent(ALLEGRO_EVENT ev);///< Get a display event from an ALLEGRO_EVENT


/**! @class Allegro5GraphicsContext
 *   @brief The Allegro 5 graphics driver
 */

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

   friend class Allegro5WindowManager;///< To create a window, use the window manager

   Allegro5GraphicsContext(std::string objname = "Nemo" , int width = 0 , int height = 0 , int flags = 0);///< Private constructor!

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

   virtual Pos2I GetMaxTextureSize();

   /// Load Default font
   void LoadDefaultFont();

   /// clears target bitmap
   virtual void Clear(EagleColor c = EagleColor(0,0,0));

   /// Blender setting functions
   void SetCopyBlender();
   void SetFullCopyBlender();
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
   /**! @fn DrawShadedTexturedQuad <float , float , float , float , EagleColor , 
                                    float , float , float , float , EagleColor , 
                                    float , float , float , float , EagleColor , 
                                    float , float , float , float , EagleColor ,
                                    EagleImage*>
    *   @brief This function is the workhorse of the graphics engine. All image drawing is deferred here.
    */
    
   virtual void DrawShadedTexturedQuad(float x1 , float y1 , float u1 , float v1 , EagleColor c1 ,
                                       float x2 , float y2 , float u2 , float v2 , EagleColor c2 ,
                                       float x3 , float y3 , float u3 , float v3 , EagleColor c3 ,
                                       float x4 , float y4 , float u4 , float v4 , EagleColor c4 ,
                                       EagleImage* texture);

   /// image drawing operations
   /**! @fn DrawTintedStretchedRegion <EagleImage* , float , float , float , float , float , float , float , float , EagleColor , int>
    *   @brief Draw the source rectangle stretched to fit the destination rectangle using the specified flags and tint
    */
   virtual void DrawTintedStretchedRegion(EagleImage* img , float sx , float sy , float sw , float sh ,
                                                      float dx , float dy , float dw , float dh , 
                                                      EagleColor tint = EagleColor(255,255,255,255) , int flags = DRAW_NORMAL);

   virtual void ConvertColorToAlpha(EagleImage* img , EagleColor alpha_color);

   /// text drawing operations

   virtual void DrawTextString(EagleFont* font , std::string str , float x , float y , EagleColor c,
                               HALIGNMENT halign = HALIGN_LEFT ,
                               VALIGNMENT valign = VALIGN_TOP);

   /**! @brief Draws the specified text vertically top to bottom in color c at x,y using the specified font and alignment */
   virtual void DrawVTextString(EagleFont* font , std::string str , float x , float y , EagleColor c ,
                                HALIGNMENT halign = HALIGN_LEFT ,
                                VALIGNMENT valign = VALIGN_TOP , int letter_spacing = -1);

   /// getters
   virtual EagleImage* GetBackBuffer();
   virtual EagleImage* GetScreen();
   virtual EagleImage* GetDrawingTarget();

   /// utilities
   virtual void HoldDrawing();
   virtual void ReleaseDrawing();
   virtual void SetDrawingTarget(EagleImage* dest);

   /// Image creation, these functions return a new image owned by the window
   EagleImage* EmptyImage(std::string iname = "Nemo");

   EagleImage* AdoptImage(ALLEGRO_BITMAP* img , std::string iname = "Nemo");
   EagleImage* ReferenceImage(ALLEGRO_BITMAP* img , std::string iname = "Nemo");

   EagleImage* CloneImage(EagleImage* img , std::string iname = "Nemo");
   EagleImage* CreateSubImage(EagleImage* parent , int x , int y , int width , int height , std::string iname = "Nemo");

   EagleImage* CreateImage(int width , int height , IMAGE_TYPE type = VIDEO_IMAGE , std::string iname = "Nemo");
   EagleImage* LoadImageFromFile(std::string file , IMAGE_TYPE type = VIDEO_IMAGE);

   /// Font loading, this function returns a new font owned by the window
   EagleFont* LoadFont(std::string file , int height , int flags = LOAD_FONT_NORMAL , IMAGE_TYPE type = VIDEO_IMAGE);

   ALLEGRO_DISPLAY* AllegroDisplay() {return display;}

   Transformer* GetTransformer();

   /// Window functions
   
   virtual void MakeDisplayCurrent();
   virtual void SetWindowPosition(int screenx , int screeny);
   virtual void ShowWindow();
   virtual void HideWindow();

};



#endif // Allegro5GraphicsContext_HPP



