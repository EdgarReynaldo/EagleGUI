


/// We need to include the main Eagle header.
/// This gives us access to everything Eagle has to offer.


#include "Eagle/Gui/WidgetBase.hpp"
#include "Eagle/Gui/WidgetColorset.hpp"


#include "Eagle.hpp"

/// We need to include the backend header that we are going to use. 
/// It imports all of the necessary system and graphics code to use Allegro 5.
#include "Eagle/backends/Allegro5Backend.hpp"


extern int selected_color;

enum CSLIDER {
   CSLIDERR = 0,
   CSLIDERG = 1,
   CSLIDERB = 2,
   CSLIDERA = 3,
   NUMCSLIDERS = 4
};

class RGBASlider : public Slider {
protected:

   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);///< Virtual function that controls how a widget is drawn

//   virtual int HandleEvent(EagleEvent e);
   virtual int PrivateHandleEvent(EagleEvent e);
public:
   CSLIDER type;
   
   
};



EagleColor GetSliderColor(RGBASlider* s4);

void SetSliders(RGBASlider* slarray4 , EagleColor c);



int main(int argc , char** argv) {
   
   (void)argc;
   (void)argv;
   
   
   /// To do most anything in Eagle, we need a system driver
   Allegro5System* sys = GetAllegro5System();
   EAGLE_ASSERT(sys);/// EAGLE_ASSERT is a regular assert but it logs and
                     /// throws an exception if null. EAGLE_DEBUG must
                     /// be defined or it will reduce to nothing.
   
   /// Now we have to initialize the system. Generally EAGLE_FULL_SETUP is used
   int sysret = sys->Initialize(EAGLE_FULL_SETUP);/// Return value is the
                                                  /// degree of success
   if (EAGLE_FULL_SETUP != sysret) {
      /// Some part of setup failed
      PrintFailedEagleInitStates(EAGLE_FULL_SETUP , sysret);/// Log the failure
      /// Test for essentials
      if (!(sysret & EAGLE_STANDARD_SETUP)) {
         /// Failed to init an essential service
         return -1;
      }
      /// Just keep going. Either touch or shaders failed but we don't need
      /// them in this case
   }
   
   EagleLog() << "Setup eagle successfully" << std::endl;
   
   int sw = 1024;
   int sh = 768;
   
   /// Create a named window size 800x600 using an OpenGL window centered on the display
   EagleGraphicsContext* win = sys->CreateGraphicsContext("Main window" , sw , sh , EAGLE_OPENGL | EAGLE_WINDOWED , -1 , -1);
   
   /// Use our EAGLE_ASSERT mechanism to check the validity of the window
   EAGLE_ASSERT(win && win->Valid());
   
   /// Clear the window to the default color, which is black
   win->Clear();
   
   /// Update the screen by flipping the display - nothing will be shown until FlipDisplay() is called
   win->FlipDisplay();

   /** Start of WidgetBase widget use tutorial */
   
   EagleFont* gfont = win->GetFont("Data/Fonts/Verdana.ttf" , -16);
   EAGLE_ASSERT(gfont);
   
   /// To handle all our widgets, we need a WidgetHandler
   WidgetHandler gui(win);
   gui.SetupBuffer(sw , sh , win);
   
   /// Since a WidgetHandler is derived from WidgetBase, we can use all our widget functions on our gui
   /// Widgets start at a size of 0,0 and position of 0,0 so we need to give them an area to occupy. Layouts do this for us
   /// But here we see how to do it manually
   gui.SetWidgetArea(Rectangle(0 , 0 , sw , sh));/// Set our position to the top left corner and our size to screen size

   SHAREDOBJECT<WidgetColorset> swc = gui.GetWidgetColorset();/// Get a shared copy of our gui's colors
   WidgetColorset& wc = *swc.get();
   wc[SDCOL] = GetColorByName("purple");/// Set the shadow color to purple   wc[BGCOL] = GetColorByName("blue");/// Set the background color to blue
   wc[MGCOL] = GetColorByName("dark_gray");/// Set the middle ground color to dark gray
   wc[FGCOL] = GetColorByName("light_gray");/// Set the foreground color to light gray
   wc[HLCOL] = GetColorByName("green");/// Set the highlight color to green
   wc[HVRCOL] = GetColorByName("green");/// Set the highlight color to green
   wc[TXTCOL] = GetColorByName("white");/// Set the text color to white
   

   /// Now that we've finished setting our colors we assign them back to the gui widget
   gui.SetWidgetColorset(swc);
   
   /// A gui comes with a dumb root layout. If you want to position everything manually, this is the way.
   /// However, setting the root layout will allow you to position all widgets added to the gui
   /// Here we're going to use a flow layout for the root layout, which holds several other widget layouts
   FlowLayout flow;
   gui.SetRootLayout(&flow);
   
   
   flow.SetFlowDirection(FLOW_FAVOR_HORIZONTAL);/// can be FLOW_FAVOR_VERTICAL or FLOW_FAVOR_HORIZONTAL
   flow.SetAnchorPosition(FBOX_ANCHOR_NW);/// Can be FBOX_ANCHOR_* [NW SE SW NE]
   flow.SetAlignment(HALIGN_CENTER , VALIGN_CENTER);/// Left top is default alignment, let's use centered alignment
   flow.SetBoxSpacing(BOX_SPACE_EVEN);/// Spread out the extra space evenly
   
//   flow.Resize(16);

   GridLayout cgrid(1 , EAGLE_NUMCOLORS , "GridLayout" , "Color button grid");
   cgrid.SetPreferredSize(125,220);
//   cgrid.SetWidgetColorset(wc);
   
   GridLayout rgbagrid(4 , 1 , "GridLayout" , "RGBA grid");
   rgbagrid.SetPreferredSize(256 , 100);
   rgbagrid.SetWidgetColorset(wc);/// Copy wc but private now
   WidgetColorset& wc2 = *(rgbagrid.GetWidgetColorset().get());/// Get direct reference to the second grid's colorset
   
   flow.Resize(2);
   flow.AddWidget(&cgrid);
   flow.AddWidget(&rgbagrid);
   
   GuiButton clrbtns[EAGLE_NUMCOLORS];/// color buttons
   RadioGroup clrgroup;/// make them a radio button
   for (unsigned int n = 0 ; n < EAGLE_NUMCOLORS ; ++n) {
      clrbtns[n].SetButtonType(RECTANGLE_BTN , TOGGLE_BTN , BUTTON_CLASS_HOVER);
      clrbtns[n].SetLabel(WidgetColorName((WIDGETCOLOR)n));
      clrbtns[n].SetPreferredSize(125 , 20);
      clrbtns[n].SetFont(gfont);
      cgrid.PlaceWidget(&clrbtns[n] , n);
      clrgroup.AddRadioButton(&clrbtns[n]);
   }
   clrgroup.SelectButton(&clrbtns[selected_color]);
   
   RGBASlider rgbaslider[4];/// RGBA sliders
   SetSliders(&rgbaslider[0] , wc[BGCOL]);
   for (unsigned int n = 0 ; n < 4 ; ++n) {
///      rgbaslider[n].SetWidgetColorset(wc2);
      rgbaslider[n].type = (CSLIDER)n;
      rgbaslider[n].SetPreferredSize(24,256);
      rgbaslider[n].SetOrientation(false);
      rgbaslider[n].SetInversion(true);
      rgbagrid.PlaceWidget(&rgbaslider[n] , n);
   }
   
   
   
   EagleColor c = wc[selected_color];
   SetSliders(rgbaslider , c);
   
   /// If we want to get events from our gui we have two options :
   /// 1 : Have the EagleEventHandler queue we're using listen to the GUI, OR
   /// 2 : Always listen for WidgetMsgs from the GUI. They come in response to handling events passed to them by the gui
   /// The best option is 1. using the system queue for a listener. The reasoning behind it is because the system already listens
   /// to everything that emits events system wide and all we have to do is wait for it to nab an event.
   sys->GetSystemQueue()->ListenTo(&gui);/// In addition, all messages from any widgets handled by this gui will pass up the chain
   /// Through the gui parent child structure.
   
   
   /// For testing and laziness only.
   gui.SetFullRedraw();
   
   bool redraw = true;
   bool quit = false;
   sys->GetSystemTimer()->Start();
   
   while (!quit) {
      if (redraw) {
         win->Clear();
         gui.Display(win , 0 , 0);
         win->FlipDisplay();
         redraw = false;
      }
      
      do {
         EagleEvent e = sys->WaitForSystemEventAndUpdateState();
         
         if ((e.type != EAGLE_EVENT_TIMER) && (e.type != EAGLE_EVENT_MOUSE_AXES)) {
            EagleLog() << EagleEventName(e.type) << std::endl;
         }
         if (e.type == EAGLE_EVENT_WIDGET) {
            if (e.widget.topic == TOPIC_BUTTON_WIDGET) {
               bool slider_button = true;
               for (unsigned int i = 0 ; i < EAGLE_NUMCOLORS ; ++i) {
                  if (&clrbtns[i] == e.widget.from) {
                     selected_color = i;
                     slider_button = false;
                     break;
                  }
               }
               if (slider_button) {
                  wc[selected_color] = wc2[BGCOL] = GetSliderColor(rgbaslider);;
               }
               else {
                  /// color button pressed, reset rgba sliders to wc[selected_color]
                  
                  wc2[BGCOL] = wc[selected_color];
                  
                  SetSliders(&rgbaslider[0] , wc2[BGCOL]);
               }
            }
            else if (e.widget.topic == TOPIC_SLIDER) {
               /// A color slider was changed, reset the BGCOL of the selected color button
               if (e.widget.msgs == SLIDER_VALUE_CHANGED) {
                  wc[selected_color] = wc2[BGCOL] = GetSliderColor(rgbaslider);
                  e.widget.from->SetBgRedrawFlag();
               }
            }
         } else {
           gui.HandleEvent(e);
         }
         WidgetMsg msg;
         if (gui.HasMessages()) {
            gui.ClearMessages();
         } 
         
         
         if (e.type == EAGLE_EVENT_TIMER) {
            redraw = true;
            gui.Update(e.timer.eagle_timer_source->SPT());
         } else {
            if (e.type == EAGLE_EVENT_KEY_DOWN && e.keyboard.keycode == EAGLE_KEY_ESCAPE) {
               quit = true;
            }
            if (e.type == EAGLE_EVENT_DISPLAY_CLOSE) {
               quit = true;
            }
         }
      } while (!sys->UpToDate());
   }
   return 0;
}




void RGBASlider::PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos) {
   EagleColor bg = GetColor(BGCOL);
   Rectangle i = InnerArea();
   switch(type) {
   case CSLIDERR :
      bg = EagleColor(bg.R() , 0 , 0 , 255);
      break;
   case CSLIDERG :
      bg = EagleColor(0 , bg.G() , 0 , 255);
      break;
   case CSLIDERB :
      bg = EagleColor(0 , 0 , bg.B() , 255);
      break;
   case CSLIDERA :
      bg = EagleColor(bg.A() , bg.A() , bg.A() , bg.A());
      break;
   default :
      break;
   }
   i.DrawGuiRectDown(win , bg , GetColor(SDCOL));
   handle->Display(win , xpos , ypos);
//   Slider::PrivateDisplay(win , xpos , ypos);
   xpos += InnerArea().CX();
   ypos += InnerArea().CY();
   char c[4] = {'R' , 'G' , 'B' , 'A'};
   win->DrawTextString(win->GetFont("Data/Fonts/Verdana.ttf" , -16) , StringPrintF("%c" , c[type]) ,
                       xpos , ypos , GetColorByName("white") , HALIGN_CENTER , VALIGN_CENTER);
}



int RGBASlider::PrivateHandleEvent(EagleEvent e) {
   int ret = DIALOG_OKAY;
   ret = Slider::PrivateHandleEvent(e);
   return ret;
}


int selected_color = (int)SDCOL;


EagleColor GetSliderColor(RGBASlider* s4) {
   
   int r = s4[0].GetSliderValue();
   int g = s4[1].GetSliderValue();
   int b = s4[2].GetSliderValue();
   int a = s4[3].GetSliderValue();
   
   
   EagleColor c(r,g,b,a);
   return c;
}
void SetSliders(RGBASlider* slarray4 , EagleColor c) {
   slarray4[0].SetupSlider(c.R() , 255);
   slarray4[1].SetupSlider(c.G() , 255);
   slarray4[2].SetupSlider(c.B() , 255);
   slarray4[3].SetupSlider(c.A() , 255);
}









