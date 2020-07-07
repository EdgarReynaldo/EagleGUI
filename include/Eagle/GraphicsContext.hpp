
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
 * @file GraphicsContext.hpp
 * @brief The interface for all graphics context objects
 * 
 * An EagleGraphicsContext represents a display (window) object. It allows you to draw to and work with display objects.
 */

#ifndef EagleGraphics_HPP
#define EagleGraphics_HPP



#include <list>
#include <string>
#include <map>



#include "Eagle/Area.hpp"
#include "Eagle/Color.hpp"
#include "Eagle/Events.hpp"
#include "Eagle/Image.hpp"
#include "Eagle/Font.hpp"
#include "Eagle/Container.hpp"
#include "Eagle/MousePointer.hpp"
#include "Eagle/Transforms.hpp"


#include "Eagle/Gui/Alignment.hpp"




/**! @enum EAGLE_DISPLAY_FLAGS
 *   @brief This enum allows you to choose options for a display. Use bitwise or to combine flags.
 *
 *   TODO : Convert EAGLE_DISPLAY_FLAGS into ALLEGRO_FLAGS
 */

enum EAGLE_DISPLAY_FLAGS {
   EAGLE_WINDOWED                    = 1 << 0, ///< Make this display windowed
   EAGLE_FULLSCREEN                  = 1 << 1, ///< Make this display fullscreen
   EAGLE_OPENGL                      = 1 << 2, ///< Use OpenGL for this display
   EAGLE_DIRECT3D                    = 1 << 3, ///< Use Direct3D for this display
   EAGLE_RESIZABLE                   = 1 << 4, ///< Make this display resizable (only works in windowed mode)
   EAGLE_NOFRAME                     = 1 << 5, ///< Make this display frameless
   EAGLE_GENERATE_EXPOSE_EVENTS      = 1 << 6, ///< Generate expose events on this display
   EAGLE_OPENGL_3_0                  = 1 << 7, ///< Require OpenGL 3.0 or better
   EAGLE_OPENGL_FORWARD_COMPATIBLE   = 1 << 8, ///< Require forward OpenGL compatibility
   EAGLE_FULLSCREEN_WINDOW           = 1 << 9, ///< Make this display a full screen window
   EAGLE_MINIMIZED                   = 1 << 10,///< Start this display minimized
   EAGLE_USE_PROGRAMMABLE_PIPELINE   = 1 << 11,///< Provide your own shader pipeline for this display
   NUM_EAGLE_DISPLAY_FLAGS           = 12
};

std::string PrintDisplayFlags(int flags);///< Prints the display flags

/**! @enum IMAGE_DRAWING_FLAGS
 *   @brief Determines the mirroring type for drawing this image
 */

enum IMAGE_DRAWING_FLAGS {
   DRAW_NORMAL = 0,///< Draw normally, default value
   DRAW_HFLIP  = 1,///< Draw this image flipped horizontally (left to right)
   DRAW_VFLIP  = 2,///< Draw this image flipped vertically (top to bottom)
   DRAW_HVFLIP = 3 ///< Draw this image flipped horizontally and vertically (same as rotating 180 degrees or by PI)
};




extern unsigned int GUI_TEXT_LINE_SPACING;/// TODO : Find better alternative



extern const char* eagle_default_font_path;

extern int eagle_default_font_size;

extern int eagle_default_font_flags;



/**
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

//*/

class EagleSystem;
class EagleThread;


/**! @enum QUADRANT_DIR
 *   @brief For drawing certain quadrants of an ellipse
 */

enum QUADRANT_DIR {
   QUADRANT_NE = 0,///< Northeast quadrant
   QUADRANT_NW = 1,///< Northwest quadrant
   QUADRANT_SW = 2,///< Southwest quadrant
   QUADRANT_SE = 3 ///< Southeast quadrant
};



/**! @class EagleGraphicsContext
 *   @brief A graphics context allows you to create and draw to and work with windows (a display)
 */

class EagleGraphicsContext : public EagleObject , public EagleEventSource {

protected :
   typedef std::unordered_set<EagleImage*> IMAGESET;
   typedef std::unordered_set<EagleFont*> FONTSET;

   typedef IMAGESET::iterator ISIT;
   typedef FONTSET::iterator FSIT;

   int scrw;
   int scrh;

   EagleImage* backbuffer;
   EagleImage* drawing_target;

   std::list<EagleImage*> draw_target_stack;

   IMAGESET imageset;
   FONTSET  fontset;

   MousePointerManager* mp_manager;/// Derived class is responsible for instantiating this object, b/c a virtual creation function
                                   /// cannot be called in a base class constructor
   float maxframes;
   float numframes;
   float total_frame_time;
   std::list<float> frame_times;
   float previoustime;
   float currenttime;

   EagleFont*  default_font;
   std::string default_font_path;
   int         default_font_size;
   int         default_font_flags;

   EagleThread* our_thread;
   EagleMutex*  window_mutex;
   


   virtual void PrivateFlipDisplay()=0;

public :

   /**! @fn EagleGraphicsContext::EagleGraphicsContext <std::string , std::string>
    *   @brief See NOTES In GraphicsContext.cpp
    */
   EagleGraphicsContext(std::string objclass , std::string objname);

   virtual ~EagleGraphicsContext() {}

   bool StartDrawing(EagleThread* draw_thread);///< Only necessary for multi-threaded programs, will lock the drawing mutex
   void CompleteDrawing(EagleThread* draw_thread);///< Only necessary for multi-threaded programs, will unlock the drawing mutex


   float GetFPS();///< Get the floating point frames per second

   virtual EagleSystem* GetSystem()=0;///< Get the system that owns us

   void SetOurThread(EagleThread* t) {our_thread = t;}///< Sets our thread, so multiple displays don't try to draw at the same time
   
   /// creation / destruction

   /**! @fn Create <int , int , int> @fn Valid @fn Destroy
    *   @brief Basic creation and destruction functions
    *
    *   Pure virtual functions to create an EagleGraphicsContext, check its validity, and destroy it.
    *   Implement in your driver's derived graphics context class
    */
   
   virtual bool Create(int width , int height , int flags)=0;///< Responsible for creating Font.cpp:default_font
   virtual bool Valid()=0;
   virtual void Destroy()=0;

   ///< Acknowledge a window resize event for this display
   virtual void AcknowledgeResize()=0;

   /// Query the display
   
   int Width() {return scrw;}///< Get this display's width
   int Height() {return scrh;}///< Get this display's height
   virtual int XPos()=0;///< Get this display's x position
   virtual int YPos()=0;///< Get this display's y position

   virtual Pos2I GetMaxTextureSize()=0;

   ///< Clears target bitmap to the specified color. Default is opaque black.
   virtual void Clear(EagleColor c = EagleColor(0,0,0))=0;

   /// Blender setting functions
   
   virtual void SetCopyBlender()=0;///< Set a copy blender mode (TODO : respects alpha? but doesn't copy it)
   virtual void SetFullCopyBlender()=0;///< Set a (full) copy blender mode (includes alpha channel)
   virtual void SetPMAlphaBlender()=0;///< Set a premultiplied alpha blender if you're using premultiplied textures or colors
   virtual void SetNoPMAlphaBlender()=0;///< Set a non premultiplied alpha blender if your alpha changes or you need it
   virtual void RestoreLastBlendingState()=0;///< Restore the most recently used blending state before this one

   /// Basic integral drawing operations
   
   ///< Fills the pixel at x,y with c
   virtual void PutPixel(int x , int y , EagleColor c)=0;

   ///< Draws a line from x1,y1 to x2,y2 with color c
   virtual void DrawLine(int x1 , int y1 , int x2 , int y2 , EagleColor c)=0;

   /**! @fn DrawRectangle <int , int , int , int , int , EagleColor>
    *   @brief Draws a rectangle at x,y of size w,h at the specified thickness in color c)
    */
   virtual void DrawRectangle(int x , int y , int w , int h , int thickness , EagleColor c)=0;

   /**! @fn DrawRectangle <Rectangle , int , EagleColor>
    *   @brief Draws a rectangle r at the specified thickness in color c)
    */
   void         DrawRectangle(Rectangle r , int thickness , EagleColor c);

   /**! @fn DrawFilledRectangle
    *   @brief Draws a filled rectangle, specifying either x,y,w,h or Rectangle r in color c
    */
   virtual void DrawFilledRectangle(int x , int y , int w , int h , EagleColor c)=0;
   void         DrawFilledRectangle(Rectangle r , EagleColor c);

   /**! @fn DrawRoundedRectangle
    *   @brief Draws a rounded rectangle, specifying either x,y,w,h or Rectangle r in color c with radii rx and ry
    */
   virtual void DrawRoundedRectangle(int x , int y , int w , int h , int rx , int ry , EagleColor c)=0;
   void         DrawRoundedRectangle(Rectangle r , int rx , int ry , EagleColor c);

   /**! @fn DrawFilledRoundedRectangle
    *   @brief Draws a filled rounded rectangle, specifying either x,y,w,h or Rectangle r in color c with radii rx and ry
    */
   virtual void DrawFilledRoundedRectangle(int x , int y , int w , int h , int rx , int ry , EagleColor c)=0;
   void         DrawFilledRoundedRectangle(Rectangle r , int rx , int ry , EagleColor c);

   /**! @fn DrawCircle <int,int,int,int,EagleColor>
    *   @brief Draws a circle with the specified center, radius, thickness, and color
    */
   virtual void DrawCircle(int cx , int cy , int radius , int thickness , EagleColor c)=0;

   /**! @fn DrawFilledCircle <int,int,int,EagleColor>
    *   @brief Draws a filled circle with the specified center, radius, thickness, and color
    */
   virtual void DrawFilledCircle(int cx , int cy , int radius , EagleColor c)=0;

   /**! @fn DrawEllipse <int,int,int,int,int,EagleColor>
    *   @brief Draws an ellipse with the specified center, radii, thickness, and color
    */
   virtual void DrawEllipse(int cx , int cy , int rx , int ry , int thickness , EagleColor c)=0;
   
   /**! @fn DrawEllipse <int,int,int,int,int,EagleColor>
    *   @brief Draws a filled ellipse with the specified center, radii, and color
    */
   virtual void DrawFilledEllipse(int cx , int cy , int rx , int ry , EagleColor c)=0;

   /**! @fn DrawFilledQuarterEllipse <Rectangle , QUADRANT_DIR , EagleColor>
    *   @brief Draws a quarter ellipse in the specified quadrant of rectangle r in color c
    */
   void         DrawFilledQuarterEllipse(Rectangle r , QUADRANT_DIR dir , EagleColor c);
   
   /**! @fn DrawTriangle <int,int,int,int,int,int,int,EagleColor>
    *   @brief Draws a triangle with the specified corners, thickness, and color
    */
   virtual void DrawTriangle(int x1 , int y1 , int x2 , int y2 , int x3 , int y3 , int thickness , EagleColor c)=0;

   /**! @fn DrawTriangle <int,int,int,int,int,int,int,EagleColor>
    *   @brief Draws a filled triangle with the specified corners, and color
    */
   virtual void DrawFilledTriangle(int x1 , int y1 , int x2 , int y2 , int x3 , int y3 , EagleColor c)=0;

   /// Sub Pixel Precise drawing operations
   
   /**! @fn PutPixel <float,float,EagleColor>
    *   @brief Draws a 1.0 x 1.0 pixel centered on x,y in color c
    */
   virtual void PutPixel(float x , float y , EagleColor c)=0;

   /**! @fn DrawLine <float,float,float,float,float,EagleColor>
    *   @brief Draws a line from x1,y1 to x2,y2 with the specified thickness and color
    */
   virtual void DrawLine(float x1 , float y1 , float x2 , float y2 , float thickness , EagleColor c)=0;

   /**! @fn DrawRectangle <float,float,float,float,float,EagleColor>
    *   @brief Draws an outlined rectangle of the specified thickness centered directly on the specified coordinates in color c
    */
   virtual void DrawRectangle(float x , float y , float w , float h , float thickness , EagleColor c)=0;

   /**! @fn DrawFilledRectangle <float,float,float,float,EagleColor>
    *   @brief Draws a filled rectangle at x,y of size w,h in color c
    */
   virtual void DrawFilledRectangle(float x , float y , float w , float h , EagleColor c)=0;

   /**! @fn DrawRoundedRectangle <float,float,float,float,float,float,float,EagleColor>
    *   @brief Draws a rounded rectangle inside the specified rectangle using the 
    *          specified corner radii rx and ry of specified thickness in color c
    */
   virtual void DrawRoundedRectangle(float x , float y , float w , float h , float rx , float ry , float thickness , EagleColor c)=0;

   /**! @fn DrawFilledRoundedRectangle <float,float,float,float,float,float,EagleColor>
    *   @brief Draws a filled rounded rectangle inside the specified area using the specified corner radii rx and ry in color c
    */
   virtual void DrawFilledRoundedRectangle(float x , float y , float w , float h , float rx , float ry , EagleColor c)=0;

   /**! @fn DrawCircle <float,float,float,float,EagleColor>
    *   @brief Draws a circular outline of specified thickness centered at cx,cy of specified radius and color c
    */
   virtual void DrawCircle(float cx , float cy , float radius , float thickness , EagleColor c)=0;

   /**! @fn DrawFilledCircle <float,float,float,EagleColor>
    *   @brief Draws a filled circle centered at cx,cy of specified radius and color c
    */
   virtual void DrawFilledCircle(float cx , float cy , float radius , EagleColor c)=0;

   /**! @fn DrawEllipse <float,float,float,float,float,EagleColor>
    *   @brief Draws an elliptical outline centered on cx,cy of radii rx and ry and specified thickness and color c
    */
   virtual void DrawEllipse(float cx , float cy , float rx , float ry , float thickness , EagleColor c)=0;

   /**! @fn DrawFilledEllipse <float,float,float,float,EagleColor>
    *   @brief Draws a filled ellipse centered on cx,cy of radii rx and ry in color c
    */
   virtual void DrawFilledEllipse(float cx , float cy , float rx , float ry , EagleColor c)=0;

   /**! @fn DrawTriangle <float,float,float,float,float,float,float,EagleColor>
    *   @brief Draws a triangular outline with the three specified corner points and thickness in color c
    */
   virtual void DrawTriangle(float x1 , float y1 , float x2 , float y2 , float x3 , float y3 , float thickness , EagleColor c)=0;

   /**! @fn DrawFilledTriangle <float,float,float,float,float,float,EagleColor>
    *   @brief Draws a filled triangle with the three specified corners in color c
    */
   virtual void DrawFilledTriangle(float x1 , float y1 , float x2 , float y2 , float x3 , float y3 , EagleColor c)=0;

   /**! @fn DrawShadedRectangle <const Rectangle*,EagleColor,EagleColor,EagleColor,EagleColor>
    *   @brief Draws a shaded axis aligned rectangle using the specified corner colors
    */
	virtual void DrawShadedRectangle(const Rectangle* r , EagleColor tl , EagleColor tr , EagleColor br , EagleColor bl)=0;

   /**! @fn DrawShadedQuad <float,float,EagleColor,float,float,EagleColor,float,float,EagleColor,float,float,EagleColor>
    *   @brief Draws a shaded quad using the specified corners and corner colors
    */
	virtual void DrawShadedQuad(float x1 , float y1 , EagleColor c1 ,
										 float x2 , float y2 , EagleColor c2 ,
										 float x3 , float y3 , EagleColor c3 ,
										 float x4 , float y4 , EagleColor c4)=0;

   /**! @fn DrawShadedTexturedQuad <float,float,float,float,EagleColor,
                                    float,float,float,float,EagleColor,
                                    float,float,float,float,EagleColor,
                                    float,float,float,float,EagleColor,
                                    EagleImage*>
    *   @brief Draws a shaded textured quad using the specified corners and corner colors
    */
   virtual void DrawShadedTexturedQuad(float x1 , float y1 , float u1 , float v1 , EagleColor c1 ,
                                       float x2 , float y2 , float u2 , float v2 , EagleColor c2 ,
                                       float x3 , float y3 , float u3 , float v3 , EagleColor c3 ,
                                       float x4 , float y4 , float u4 , float v4 , EagleColor c4 ,
                                       EagleImage* texture)=0;

   /// TODO : Consistent naming, FIXME : DOCS
										 										 
   /// image drawing operations

   /**! @fn DrawTintedStretchedRegion <EagleImage* , float , float , float , float , float , float , float , float , EagleColor , int>
    *   @brief Draw the source rectangle stretched to fit the destination rectangle using the specified flags and tint
    *          This function is the workhorse of the graphics engine. All image drawing is deferred here.
    */
   virtual void DrawTintedStretchedRegion(EagleImage* img , float sx , float sy , float sw , float sh ,
                                                    float dx , float dy , float dw , float dh , 
                                                    EagleColor tint = EagleColor(255,255,255,255) , int flags = DRAW_NORMAL)=0;

   /**! @fn DrawTintedStretchedRegion <EagleImage* , Rectangle , Rectangle , EagleColor , int>
    *   @brief Draw the source rectangle stretched to fit the destination rectangle using the specified flags and tint
    *
    */
   void DrawTintedStretchedRegion(EagleImage* img , Rectangle src , Rectangle dest , 
                                  EagleColor tint = EagleColor(255,255,255,255) , int flags = DRAW_NORMAL);

   /**! @fn Draw <EagleImage* , float , float , HALIGNMENT , VALIGNMENT , int>
    *   @brief Draws an @ref EagleImage via pointer to x,y with the specified horizontal and vertical alignment and flags.
    */
   void Draw(EagleImage* img , float x , float y , HALIGNMENT halign = HALIGN_LEFT , VALIGNMENT valign = VALIGN_TOP , 
             EagleColor tint = EagleColor(255,255,255,255) , int flags = DRAW_NORMAL);

   /**! @fn DrawStretchedRegion <EagleImage* , Rectangle , Rectangle , int>
    *   @brief Same as @ref DrawStretchedRegion <EagleImage*,float,float,float,float,float,float,float,float,EagleColor,int>
    */
   void DrawStretchedRegion(EagleImage* img , Rectangle src , Rectangle dest , int flags = DRAW_NORMAL);

   /**! @fn DrawStretched <EagleImage* , Rectangle , int>
    *   @brief Draws the specified image to the destination rectangle stretched as necessary to fit using the specified flags
    */
   void DrawStretched(EagleImage* img , Rectangle dest , int flags = DRAW_NORMAL);

   /**! @fn DrawTintedStretched <EagleImage* , Rectangle , EagleColor , int>
    *   @brief Draws the specified image to the destination rectangle stretched and tinted as necessary to fit using the specified flags
    */
   void DrawTintedStretched(EagleImage* img , Rectangle dest , 
                            EagleColor tint = EagleColor(255,255,255,255) , int flags = DRAW_NORMAL);
   
   /**! @fn DrawTinted <EagleImage* , int , int , EagleColor>
    *   @brief Draws the specified image using the specified tint color at x,y
    */
   void DrawTinted(EagleImage* img , int x , int y , EagleColor col = EagleColor(255,255,255,255) , int flags = DRAW_NORMAL);

   /**! @fn DrawTintedRegion <EagleImage*,Rectangle,float,float,EagleColor>
    *   @brief Same as @ref DrawStretchedRegion but tinted col
    */
   void DrawTintedRegion(EagleImage* img , Rectangle src , float x , float y , EagleColor col = EagleColor(255,255,255,255) , int flags = DRAW_NORMAL);

   ///< Centers the image, no scaling is performed
   void DrawImageCenter (EagleImage* img , Rectangle dest , EagleColor tint = EagleColor(255,255,255,255) , int flags = DRAW_NORMAL);

   ///< Maintains aspect and stretches to fit
   void DrawImageFit    (EagleImage* img , Rectangle dest , EagleColor tint = EagleColor(255,255,255,255) , int flags = DRAW_NORMAL);

   ///< Maintains aspect and stretches to cover
   void DrawImageCover  (EagleImage* img , Rectangle dest , EagleColor tint = EagleColor(255,255,255,255) , int flags = DRAW_NORMAL);

   ///< Stretches the image to fill
   void DrawImageStretch(EagleImage* img , Rectangle dest , EagleColor tint = EagleColor(255,255,255,255) , int flags = DRAW_NORMAL);


   ///< Converts the specified color in the specified image to alpha zero, clear
   virtual void ConvertColorToAlpha(EagleImage* img , EagleColor alpha_color)=0;

   /// text drawing operations

   /**! @brief Draws the specified text horizontally left to right in color c at x,y using the specified font and alignment */
   virtual void DrawTextString(EagleFont* font , std::string str , float x , float y , EagleColor c ,
                               HALIGNMENT halign = HALIGN_LEFT ,
                               VALIGNMENT valign = VALIGN_TOP)=0;

   /**! @brief Draws the specified text vertically top to bottom in color c at x,y using the specified font and alignment */
   virtual void DrawVTextString(EagleFont* font , std::string str , float x , float y , EagleColor c ,
                               HALIGNMENT halign = HALIGN_LEFT ,
                               VALIGNMENT valign = VALIGN_TOP)=0;

   /**! @brief Draws the specified multi-line text in color c at x,y using the specified font and alignment */
   void DrawMultiLineTextString(EagleFont* font , std::string str , float x , float y , EagleColor c , float line_spacing ,
                               HALIGNMENT halign = HALIGN_LEFT ,
                               VALIGNMENT valign = VALIGN_TOP);


   /**! @brief Draws the specified gui text in color c at x,y using the specified font and alignment 
    *   Gui text has any character after an ampersand underlined
    */
   void DrawGuiTextString(EagleFont* font , std::string str , float x , float y , EagleColor c ,
                          HALIGNMENT halign = HALIGN_LEFT ,
                          VALIGNMENT valign = VALIGN_TOP);


   /// getters

   virtual EagleImage* GetBackBuffer()=0;///< Pure virtual function to get the back buffer
   virtual EagleImage* GetScreen()=0;///< Pure virtual function to get the screen (usu. the same as the back buffer)
   virtual EagleImage* GetDrawingTarget()=0;///< Pure virtual function to get the current drawing target

   /// utilities

   void FlipDisplay();///< Flip the display and show its contents on screen. Overload @ref PrivateFlipDisplay to affect this

   virtual void HoldDrawing()=0;///< Pure virtual function to hold the drawing
   virtual void ReleaseDrawing()=0;///< Pure virtual function to resume drawing
   virtual void SetDrawingTarget(EagleImage* dest)=0;///< Pure virtual function to set the drawing target

   void DrawToBackBuffer();///< Sets the drawing target to the backbuffer for this display

   /// Image creation / loading / sub division - these images are owned by the window

   ///< Create an empty image
   virtual EagleImage* EmptyImage(std::string iname = "Nemo")=0;

   ///< Clone an image
   virtual EagleImage* CloneImage(EagleImage* clone , std::string iname = "Nemo")=0;
   
   ///< Create a sub image from an existing EagleImage
   virtual EagleImage* CreateSubImage(EagleImage* parent , int x , int y , int width , int height , std::string iname = "Nemo")=0;

   ///< Create an image of the specified width, height, @redf IMAGE_TYPE , and name
   virtual EagleImage* CreateImage(int width , int height , IMAGE_TYPE type = VIDEO_IMAGE , std::string iname = "Nemo")=0;

   ///< Load an image from file using the specified @ref IMAGE_TYPE
   virtual EagleImage* LoadImageFromFile(std::string file , IMAGE_TYPE type = VIDEO_IMAGE)=0;

   void                FreeImage(EagleImage* img);///< Frees any references this window has to the image and destroys it
   void                FreeAllImages();///< Frees all images owned by this display

   /// Font loading - these fonts are owned by the window

   ///< Pure virtual function to load a font from a file with the specified height, @ref FONT_LOADING_FLAGS , and @ref IMAGE_TYPE
   virtual EagleFont* LoadFont(std::string file , int height , int flags = LOAD_FONT_NORMAL , IMAGE_TYPE type = VIDEO_IMAGE)=0;

   void               FreeFont(EagleFont* font);///< Frees any references this window has to the font and destroys it
   void               FreeAllFonts();///< Frees all fonts owned by this window
   
   /**! @fn DefaultFont <>
    *   @brief Get a pointer to the default font
    *   
    *   The default font is created using the default font file, size, and flags. See @ref EagleFont and
    *   @ref Font.hpp
    */
   EagleFont* DefaultFont();

   std::string DefaultFontPath();///< Returns the path used by the default font
   int DefaultFontSize();///< Returns the size used by the default font
   int DefaultFontFlags();///< Returns the flags used by the default font

   
   /// Window control
   virtual void SetWindowPosition(int screenx , int screeny)=0;
   virtual void ShowWindow()=0;
   virtual void HideWindow()=0;
   
   /// mouse control

   ///< Show the mouse on this display
   void ShowMouse();
   
   ///< Hide the mouse on this display
   void HideMouse();

   /**! @fn AcquireMousePointer <void*,MOUSE_POINTER_TYPE,bool>
    *   @brief Take ownership of the mouse pointer
    *   
    *   @param caller The address of the object that wants to take ownership of the mouse pointer
    *   @param ptype The type of pointer to use
    *   @param use_system_pointer Whether or not to use a system pointer, or a user supplied one
    *   
    *   Call @ref ReleaseMousePointer when you are finished using the pointer
    */   
   bool AcquireMousePointer(void* caller , MOUSE_POINTER_TYPE ptype , bool use_system_pointer);
   
   /**! @fn ReleaseMousePointer <const void*>
    *   @brief Releases the lock on the mouse pointer from the specified owner address
    *   @param caller The address passed to @ref AcquireMousePointer as caller
    */
   void ReleaseMousePointer(const void* caller);

   void ReAcquireMouse();///< Call when your window loses and regains focus if there is more than one, as the mp is shared

   bool SetCustomPointer(MousePointerInfo info);///< Sets a custom mouse pointer using the specified @ref MousePointerInfo
   
///   void SetCustomPointerSet(MousePointerSet* pointer_set);

   void SetMousePosition(int mousex , int mousey);///< Sets the mouse position relative to the display

   /// drawing target

   void PushDrawingTarget(EagleImage* img);///< Pushes a new drawing target onto the drawing stack
   void PopDrawingTarget();///< Pops an image off the drawing stack, and restores the previous one, reverting to the back buffer when empty

   virtual Transformer* GetTransformer()=0;///< Pure virtual function to get the Transformer object for this window

   virtual void MakeDisplayCurrent()=0;///< Pure virtual function to make the display current in multi threaded applications
   
   virtual std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const ;
   
};


#include "Eagle/Position2.hpp"


/**! @class DrawingTargeter
 *   @brief Simple class to automatically push and pop drawing targets onto the drawing target stack
 *   
 *   Usage :
 *   
 *   ```
 *       {
 *           DrawingTargeter drawing_targeter(my_win , my_image);/// pushes my_image onto my_win's drawing stack
 *           /// Draw to my_image
 *       }/// drawing_targeter goes out of scope and pops my_image off the drawing stack
 *   ```
 */

class DrawingTargeter {

private :
   EagleGraphicsContext* win;

public :
   DrawingTargeter(EagleGraphicsContext* window , EagleImage* target_image);///< pushes target_image onto the drawing stack
   ~DrawingTargeter();///< pops target image off drawing stack automatically when this goes out of scope.
};



#endif // EagleGraphics_HPP



