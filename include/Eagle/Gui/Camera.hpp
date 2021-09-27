
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
 *    Copyright 2009-2021+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file Camera.hpp
 * @brief A simple widget class to view a bitmap. Includes scrolling, timed view movement, and zooming.
 * 
 */

#ifndef Camera_H
#define Camera_H



#include "Eagle/Gui/WidgetBase.hpp"
#include "Eagle/Image.hpp"



extern const unsigned int TOPIC_CAMERA;///< The default topic for camera classes

/**! @enum CAMERA_MSGS
 *   @brief An enum representing the different message IDs a camera may queue
 */

enum CAMERA_MSGS {
   CAMERA_VIEW_MOVED        = 0,///< A message ID indicating the camera view moved or is moving
   CAMERA_VIEW_DEST_REACHED = 1 ///< A message ID indicating the camera view has reached its destination
};


///< Registers the CAMERA_VIEW_MOVED message for the TOPIC_CAMERA topic
REGISTER_WIDGET_MESSAGE(TOPIC_CAMERA , CAMERA_VIEW_MOVED);

///< Registers the CAMERA_VIEW_DEST_REACHED message for the TOPIC_CAMERA topic
REGISTER_WIDGET_MESSAGE(TOPIC_CAMERA , CAMERA_VIEW_DEST_REACHED);



extern const unsigned int TOPIC_ZOOM_CAMERA;///< The default topic for zoom cameras

/**! @enum ZOOM_CAMERA_MSGS
 *   @brief An enum representing the different messages a zoom camera may send
 */
enum ZOOM_CAMERA_MSGS {
   CAMERA_ZOOM_CHANGED      = 0,///< The zoom changed or is changing
   CAMERA_ZOOM_DEST_REACHED = 1 ///< The zoom destination has been reached
};

///< Registers the CAMERA_ZOOM_CHANGED message for the TOPIC_ZOOM_CAMERA topic
REGISTER_WIDGET_MESSAGE(TOPIC_ZOOM_CAMERA , CAMERA_ZOOM_CHANGED);

///< Registers the CAMERA_ZOOM_DEST_REACHED message for the TOPIC_ZOOM_CAMERA topic 
REGISTER_WIDGET_MESSAGE(TOPIC_ZOOM_CAMERA , CAMERA_ZOOM_DEST_REACHED);



/**! @class Camera
 *   @brief A simple widget class to store a changeable view of an image
 *
 *   @sa WidgetHandler
 *   Used by the WidgetHandler class to change the view of its buffer
 */

class WCamera : public WidgetBase {
protected :
   EagleImage* view_bmp;///< The image to view
   
   Rectangle view_area;///< What is shown of the viewed image
   
   double prev_x;
   double prev_y;
   int dest_x;
   int dest_y;
   double xvel;
   double yvel;
   double xacc;
   double yacc;
   double time_since_prev_pos;
   double move_time;
   
   int startmx;
   int startmy;
   int startxpos;
   int startypos;
   bool drag;
   bool mmb_scroll_enabled;
   bool take_focus;
   

   virtual void SetDestination(int xpos , int ypos);
   virtual void SetViewPos(int xpos , int ypos);

   void PerformMove();

public :
   
   WCamera(std::string objclass = "Camera" , std::string objname = "Nemo");
   virtual ~WCamera() {}
   
protected :

   /// WidgetBase overrides

   virtual int PrivateHandleEvent(EagleEvent ee);
///   virtual int  PrivateCheckInputs();
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);
   virtual int  PrivateUpdate(double tsec);

public :

   /// Member functions
   
   ///< Sets the view area and the image to view
   virtual void SetView(EagleImage* bmp , int x , int y , int w , int h);
   
   ///< Sets the view area for an already specified image
   virtual void SetViewArea(int x , int y , int w , int h);

   ///< Shortcut function to @sa SetView(EagleImage*,int,int,int,int)
   void SetView(EagleImage* bmp , Rectangle area_to_view);///< Set the image to view and the area of the image to view

   ///< Shortcut function to @sa SetViewArea(int,int,int,int)
   void SetViewArea(Rectangle area_to_view);// view must be set first

   void AccMoveViewTlxTo(int xpos , int ypos , double time = 0.0);   ///< Decelerated move to position, instant move if time == 0.0
   void AccMoveViewCenterTo(int xpos , int ypos , double time = 0.0);///< Decelerated move to position, instant move if time == 0.0
   void AccMoveViewBy(int dx , int dy , double time = 0.0);          ///< Decelerated move by position delta, instant move if time == 0.0
   
   void MoveViewTlxTo(int xpos , int ypos , double time = 0.0);   ///< Set velocity move to position, instant move if time == 0.0
   void MoveViewCenterTo(int xpos , int ypos , double time = 0.0);///< Set velocity move center to position, instant move if time == 0.0
   void MoveViewBy(int dx , int dy , double time = 0.0);          ///< Set velocity move by position delta, instant move if time == 0.0

   /// Setters

   void AllowMiddleMouseButtonScroll(bool allow);///< Whether to allow middle mouse button drag scrolling of the view
   void TakesFocus(bool click_takes_focus);      ///< Whether a mouse click on us will take the focus. Default setting is true

   /// Getters

   Rectangle ViewArea() const {return view_area;}    ///< Returns the view rectangle
   int       ViewX()    const {return view_area.X();}///< Returns the view left x
   int       ViewY()    const {return view_area.Y();}///< Returns the view top y
   int       ViewW()    const {return view_area.W();}///< Returns the view width
   int       ViewH()    const {return view_area.H();}///< Returns the view height
   
   virtual std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const;///< Describes this widget to a stream

};



/**! @class SBCamera
 *   @brief A sub bitmap camera? TODO : AM I USEFUL? Or should someone just use a sub bitmap with the Camera class?
 *
 *   Camera that uses a sub bitmap of the viewing area
 */

/** TODO : UPDATE
 
class SBCamera : public Camera {
protected :
   BITMAP* sbcam;
   BMP_DRAW_TYPE draw_type;
   
   virtual void SetViewPos(int xpos , int ypos);

public :
   SBCamera(std::string name , BMP_DRAW_TYPE bmp_draw_type);
   SBCamera(std::string name , BITMAP* view , BMP_DRAW_TYPE bmp_draw_type , Rectangle area_to_view , Rectangle display_area , UINT wflags = DEFAULT_FLAGS);
   ~SBCamera() {Free();}
   
   
   void Free();/// Since SBCamera uses sub bitmaps, you must call Free() before freeing the parent bitmap

   virtual void DisplayOn(BITMAP* bmp , int x , int y);

   void SetDrawType(BMP_DRAW_TYPE bmp_draw_type);
   
   BITMAP* Bitmap() {return sbcam;}

};



/// Camera that can zoom
/// ZoomCamera generally can only use a memory bitmap as the view bitmap when using Allegro's stretch.
/// A system bitmap can be used if the GFX_HW_SYS_STRETCH_BLIT flag is set and the destination is a video
/// bitmap. Likewise, a video bitmap can be used as the view bitmap if the GFX_HW_VRAM_STRETCH_BLIT flag
/// is set and the destination is a video bitmap. When using the smooth stretch, the view bitmap can be of
/// any type, but will be slow to draw if it is a video bitmap. (Reading from video memory is slow).
class ZoomCamera : public Camera {
protected :
   int vcx,vcy;
   int dvcx,dvcy;// desired view center position
   Rectangle display_area;
   
   double prev_w;
   double prev_h;
   int dest_w;
   int dest_h;
   double wvel;
   double hvel;
   double wacc;
   double hacc;
   double time_since_prev_zoom;
   double zoom_time;
   
   STRETCH_TYPE stretch_type;
   
   int min_zoom_w;
   int min_zoom_h;
   int zoom_value_100ths;
   int zoom_rate_100ths;
   double zoom_scale;// view.W()*zoom_scale = view_bmp->w , view.H()*zoom_scale = view_bmp->h
   
   int min_zoom_value_100ths;
   int max_zoom_value_100ths;
   double min_zoom_scale;
   double max_zoom_scale;
   
   BitmapHandler buffer;
   bool drawn;
   
   
//   virtual void SetDestination(xpos,ypos);
   void SetDestinationDimensions(int width , int height);

   void SetViewPos(int xpos , int ypos);
   void SetViewDimensions(int width , int height);
   
   void PerformZoom();
   
   void ResetBuffer();
   void ResetDisplayArea();

public :

//   Camera(BITMAP* view , Rectangle area_to_view , Rectangle display_area , UINT wflags = DEFAULT_FLAGS);
   ZoomCamera(std::string name , STRETCH_TYPE cam_stretch_type , BITMAP* view , Rectangle area_to_view ,
               Rectangle position , UINT wflags = DEFAULT_FLAGS);
   
   virtual WidgetMsg Update (double tsec);
   virtual WidgetMsg CheckInputs(int msx , int msy);
   virtual void DisplayOn(BITMAP* bmp , int x , int y);

   virtual void SetDrawPos(int xpos , int ypos);
   virtual void SetDrawDimensions(int width , int height);
   virtual void SetArea(int xpos , int ypos , int width , int height);
   virtual void SetArea(const Rectangle& r);

   virtual void SetView(BITMAP* bmp , Rectangle area_to_view);
   virtual void SetViewArea(Rectangle area_to_view);
   
   void AccZoomTo(int width , int height , double time = 0.0 , int center_x = -1 , int center_y = -1);
   void AccZoomBy(int dw , int dh , double time = 0.0 , int center_x = -1 , int center_y = -1);

   void MoveZoomTo(int width , int height , double time = 0.0 , int center_x = -1 , int center_y = -1);
   void MoveZoomBy(int dw , int dh , double time = 0.0 , int center_x = -1 , int center_y = -1);
   
   void SetZoomCenter(int center_x = -1 , int center_y = -1);
   
   void SetZoomValue(int pow2_100ths);// [0,oo) , 0 is no zoom , higher values indicate closer zoom 0=1x,100=2x,200=4x,300=8x,400=16x
   void ChangeZoomValueBy(int delta_pow2_100ths);// negative delta zooms out , positive delta zooms in
   void SetZoomScale(double scale);// Magnification value, the higher the number, the greater the zoom [1,oo)
   void ChangeZoomScaleBy(double delta_scale);// negative delta zooms out , positive delta zooms in

   void SetZoomRate(int rate_100ths);

   void SetMinZoomDimensions(int min_width , int min_height);
   
   void SetStretchType(STRETCH_TYPE cam_stretch_type);
   

//   void SetDisplayPos(int xpos , int ypos);
//   void SetDisplayDimensions(int width , int height);
//   void SetDisplayArea(int xpos , int ypos , int width , int height);
//   void SetDisplayArea(const Rectangle& r);

};


//*/



#endif // Camera_H




