


/// We need to include the main Eagle header.
/// This gives us access to everything Eagle has to offer.


#include "Eagle/Gui/WidgetBase.hpp"
#include "Eagle/Gui/WidgetColorset.hpp"


#include "Eagle.hpp"

/// We need to include the backend header that we are going to use. 
/// It imports all of the necessary system and graphics code to use Allegro 5.
#include "Eagle/backends/Allegro5Backend.hpp"



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


public:
   CSLIDER type;
   
   
};



void SetSliders(WidgetColorset* wc , RGBASlider* slarray4 , EagleColor c);



int main(int argc , char** argv) {
   
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
   gui.SetWidgetArea(Rectangle(0 , 0 , sw , sh));/// Set our position to 200,200 and our size to 400x300

   WidgetColorset wc = *gui.GetWidgetColorset();/// Get a copy of our gui's colors
   wc[SDCOL] = GetColorByName("purple");/// Set the shadow color to purple   wc[BGCOL] = GetColorByName("blue");/// Set the background color to blue
   wc[MGCOL] = GetColorByName("dark_gray");/// Set the middle ground color to dark gray
   wc[FGCOL] = GetColorByName("light_gray");/// Set the foreground color to light gray
   wc[HLCOL] = GetColorByName("green");/// Set the highlight color to green
   wc[HVRCOL] = GetColorByName("green");/// Set the highlight color to green
   wc[TXTCOL] = GetColorByName("white");/// Set the text color to white
   
   /// Now that we've finished setting our colors we assign them back to the gui widget
   WidgetColorset wc2 = wc;
   gui.SetWidgetColorset(wc);
   
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
   
   GridLayout rgbagrid(4 , 1 , "GridLayout" , "RGBA grid");
   rgbagrid.SetPreferredSize(200 , 256);
   
   
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
   int selected_color = (int)SDCOL;
   clrgroup.SelectButton(&clrbtns[selected_color]);
   
   wc2 = wc;
   
   RGBASlider rgbaslider[4];/// RGBA sliders
   for (unsigned int n = 0 ; n < 4 ; ++n) {
      rgbaslider[n].SetWidgetColorset(wc2);
      rgbaslider[n].type = (CSLIDER)n;
      rgbaslider[n].SetPreferredSize(50 , 256);
      rgbagrid.PlaceWidget(&rgbaslider[n] , n);
   }
   
   EagleColor c = wc2[selected_color];
   
   SetSliders(&wc2 , rgbaslider , c);
   
   
   
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
         gui.HandleEvent(e);
         WidgetMsg msg;
         while (gui.HasMessages()) {
            msg = gui.TakeNextMessage();
            if (msg.Topic() == TOPIC_BUTTON_WIDGET) {
               /// color button pressed, reset rgba sliders to wc[selected_color]
               SetSliders(&wc2 , rgbaslider , wc[selected_color]);

            }
            else if (msg.Topic() == TOPIC_SLIDER) {
               /// A color slider was changed, reset the BGCOL of the selected color button
               SHAREDOBJECT<WidgetColorset> wcshared = clrbtns[selected_color].GetWidgetColorset();
               (*wcshared.get())[BGCOL] = EagleColor(rgbaslider[0].GetColor(BGCOL).R() , rgbaslider[1].GetColor(BGCOL).G() ,
                                                  rgbaslider[2].GetColor(BGCOL).B() , rgbaslider[3].GetColor(BGCOL).A());
               clrbtns[selected_color].SetWidgetColorset(wcshared);
               SetSliders(wcshared.get() , rgbaslider , (*wcshared.get())[BGCOL]);
            }
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
   Slider::PrivateDisplay(win , xpos , ypos);
   xpos += InnerArea().CX();
   ypos += InnerArea().CY();
   char c[4] = {'R' , 'G' , 'B' , 'A'};
   win->DrawTextString(win->GetFont("Data/Fonts/Verdana.ttf" , -16) , StringPrintF("%c" , c[type]) ,
                       xpos , ypos , GetColor(TXTCOL) , HALIGN_CENTER , VALIGN_CENTER);
}



void SetSliders(WidgetColorset* wc , RGBASlider* slarray4 , EagleColor c) {
   WidgetColorset wc2;
   if (wc) {
      wc2 = *wc;
   }
   wc2[MGCOL] = EagleColor(c.R() , 0 , 0 , 255);
   slarray4[0].SetWidgetColorset(wc2);
   wc2[MGCOL] = EagleColor(0 , c.G() , 0 , 255);
   slarray4[1].SetWidgetColorset(wc2);
   wc2[MGCOL] = EagleColor(0 , 0 , c.B() , 255);
   slarray4[2].SetWidgetColorset(wc2);
   wc2[MGCOL] = EagleColor(c.A() , c.A() , c.A() , c.A());
   slarray4[3].SetWidgetColorset(wc2);
}









