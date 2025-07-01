


/// We need to include the main Eagle header.
/// This gives us access to everything Eagle has to offer.
#include "Eagle.hpp"

/// We need to include the backend header that we are going to use. 
/// It imports all of the necessary system and graphics code to use Allegro 5.
#include "Eagle/backends/Allegro5Backend.hpp"

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
   
   /// Create a named window size 800x600 using an OpenGL window centered on the display
   EagleGraphicsContext* win = sys->CreateGraphicsContext("Main window" , 800 , 600 , EAGLE_OPENGL | EAGLE_WINDOWED , -1 , -1);
   
   /// Use our EAGLE_ASSERT mechanism to check the validity of the window
   EAGLE_ASSERT(win && win->Valid());
   
   /// Clear the window to the default color, which is black
   win->Clear();
   
   /// Update the screen by flipping the display - nothing will be shown until FlipDisplay() is called
   win->FlipDisplay();

   /** Start of WidgetBase widget use tutorial */
   
   /// To handle all our widgets, we need a WidgetHandler
   WidgetHandler gui;
   
   /// Since a WidgetHandler is derived from WidgetBase, we can use all our widget functions on our gui
   /// Widgets start at a size of 0,0 and position of 0,0 so we need to give them an area to occupy. Layouts do this for us
   /// But here we see how to do it manually
   gui.SetWidgetArea(Rectangle(200,200,400,300));/// Set our position to 200,200 and our size to 400x300

   WidgetColorset wc = *gui.GetWidgetColorset();/// Get a copy of our gui's colors
   wc[SDCOL] = GetColorByName("purple");/// Set the shadow color to purple   wc[BGCOL] = GetColorByName("blue");/// Set the background color to blue
   wc[MGCOL] = GetColorByName("dark_gray");/// Set the middle ground color to dark gray
   wc[FGCOL] = GetColorByName("light_gray");/// Set the foreground color to light gray
   wc[HLCOL] = GetColorByName("green");/// Set the highlight color to green
   wc[TXTCOL] = GetColorByName("white");/// Set the text color to white
   
   /// Now that we've finished setting our colors we assign them back to the gui widget
   gui.SetWidgetColorset(wc);
   
   /// A gui comes with a dumb root layout. If you want to position everything manually, this is the way.
   /// However, setting the root layout will allow you to position all widgets added to the gui
   /// Here we're going to use a flow layout for the root layout
   FlowLayout flow;
   gui.SetRootLayout(&flow);
   
   flow.SetFlowDirection(FLOW_FAVORED_VERTICAL);/// can be FLOW_FAVORED_VERTICAL or FLOW_FAVORED_HORIZONTAL
   flow.SetAnchorPosition(FBOX_ANCHOR_NW);/// Can be FBOX_ANCHOR_* [NW SE SW NE]
   flow.SetAlignment(HALIGN_CENTER , VALIGN_CENTER);/// Left top is default alignment, let's use centered alignment
   flow.SetBoxSpacing(BOX_SPACE_EVEN);/// Spread out the extra space evenly
   
   
   
   return 0;
}
















