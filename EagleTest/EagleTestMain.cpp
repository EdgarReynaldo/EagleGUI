
#include <vector>
#include <unordered_set>
#include <cmath>
#ifndef M_PI
   #define M_PI 3.1415269
#endif

#include <map>

#include "Eagle.hpp"
#include "Eagle/backends/Allegro5Backend.hpp"
#include "allegro5/allegro.h"
#include "allegro5/allegro_primitives.h"

enum HEXCORNER {
   HC_EAST         = 0,
   HC_SOUTHEAST    = 1,
   HC_SOUTHWEST     = 2,
   HC_WEST         = 3,
   HC_NORTHWEST    = 4,
   HC_NORTHEAST    = 5,
   NUM_HEX_CORNERS = 6
};

enum HEXDIRECTION {
   HD_NORTHEAST = 0,
   HD_SOUTHEAST = 1,
   HD_SOUTH     = 2,
   HD_SOUTHWEST = 3,
   HD_NORTHWEST = 4,
   HD_NORTH = 5,
   NUM_HEX_DIRECTIONS = 6
};

class Hexagon {
   double radius;
   std::vector<ALLEGRO_VERTEX> verts;
   
   void GenPoints();


public :
   Hexagon() : radius(0.0) , verts(NUM_HEX_DIRECTIONS , ALLEGRO_VERTEX()) {}
   Hexagon(double width);
   
   void SetRadius(double r);
   void DrawFilled(EagleGraphicsContext* win , double x , double y , ALLEGRO_COLOR color);
   void DrawOutline(EagleGraphicsContext* win , double x , double y , ALLEGRO_COLOR color);
};



class HexTile {
   static Hexagon proto;

   int owner;
   std::unordered_set<int> border;
   std::vector<HexTile*> neighbors;

   friend class HexGrid;
   friend class HexGame;
   
   void CalcBorders();
   
public :
   HexTile() : owner(0) , border() , neighbors(6 , NULL) {}
   
//   void SetRadius(double r);
   void SetRadius(double r) {
      proto.SetRadius(fabs(r));
   }
   
   void DrawFilled(EagleGraphicsContext* win , double x , double y , ALLEGRO_COLOR color);
   void DrawOutline(EagleGraphicsContext* win , double x , double y , ALLEGRO_COLOR color);
};



class HexGrid {
   unsigned int w,h,rad;
   
   std::vector<std::vector<HexTile> > grid;
   
   friend class HexGame;
   
   
public :
   HexGrid();
   
   void Resize(unsigned int width , unsigned int height);

   void SetRadius(unsigned int radius);

   void DrawGrid(EagleGraphicsContext* win , int xpos , int ypos , std::map<int , ALLEGRO_COLOR> colors);

   unsigned int Width() {return w;}
   unsigned int Height() {return h;}
};



class HexGame {
   
   HexGrid hgrid;
   double rad;
   
   
   std::map<int , ALLEGRO_COLOR> teamcolors;

   int turn;
   
   int mx,my;
   
   HexTile* hover;
   
   void Claim(HexTile* tile , int owner);
   
   
public :
   
   HexGame();

   void Resize(unsigned int width , unsigned int height , double radius);

   void HandleEvent(EagleEvent ee);

   void DisplayOn(EagleGraphicsContext* win , int xpos , int ypos);

   void Run(EagleSystem* sys , EagleGraphicsContext* win);
   
};



int main(int argc , char** argv) {
   
   (void)argc;
   (void)argv;
   
   
   Allegro5System* a5sys = GetAllegro5System();
   
   int ret = 0;
   if ((ret = a5sys->Initialize(EAGLE_FULL_SETUP)) != EAGLE_FULL_SETUP) {
      if (!(ret & (EAGLE_MOUSE | EAGLE_KEYBOARD))) {
         EagleCritical() << "Failed to install keyboard and mouse." << std::endl;
      }
   }
   
   int sw = 1024;
   int sh = 768;
   EagleGraphicsContext* win = a5sys->CreateGraphicsContext("HexGame window" , sw , sh , EAGLE_OPENGL);
   
   EAGLE_ASSERT(win && win->Valid());
   
   HexGame game;
   
   game.Resize(12,9,25);
   
   a5sys->GetSystemTimer()->Start();

   game.Run(a5sys , win);
   
   win->FlipDisplay();
   
   a5sys->GetSystemQueue()->WaitForEvent(EAGLE_EVENT_KEY_DOWN , 0);
   
   
   return 0;
}



#include "Eagle.hpp"
#include "Eagle/backends/Allegro5Backend.hpp"
#include "allegro5/allegro_color.h"

class TestWidget : public WidgetBase {
   
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);
   virtual int PrivateHandleEvent(EagleEvent ee);
   
   virtual void OnFlagChanged(WIDGET_FLAGS f , bool on);

   
public :
   TestWidget(std::string objname) :
         WidgetBase("TestWidget" , objname) {}
   
};



void TestWidget::PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos) {
   Rectangle r = InnerArea();
   r.MoveBy(xpos,ypos);
   unsigned int flags = Flags();
   EagleColor c1 = GetColor(MGCOL);
   EagleColor c2 = GetColor(BGCOL);
   if (flags & HASFOCUS) {
      c1 = GetColor(HLCOL);
      c2 = GetColor(FGCOL);
   }
   win->DrawFilledRectangle(r , (flags & HOVER)?c1:c2);
   std::string n = ShortName();
   win->DrawTextString(win->DefaultFont() , n , InnerArea().CX() , InnerArea().CY() , EagleColor(255,0,0) , HALIGN_CENTER , VALIGN_CENTER);
}



int TestWidget::PrivateHandleEvent(EagleEvent ee) {
   if (ee.type == EAGLE_EVENT_MOUSE_BUTTON_DOWN && ee.mouse.button == 1) {
      if (OuterArea().Contains(ee.mouse.x , ee.mouse.y)) {
         return DIALOG_TAKE_FOCUS;
      }
   }
   return DIALOG_OKAY;
}



void TestWidget::OnFlagChanged(WIDGET_FLAGS f , bool on) {
   if (f == HOVER) {
      EagleLog() << "HOVER changed to " << (on?"on":"off") << std::endl;
      SetRedrawFlag();
   }
   if (f == HASFOCUS) {
      EagleLog() << "HASFOCUS changed to " << (on?"on":"off") << std::endl;
      SetRedrawFlag();
   }
}


int main3(int argc , char** argv) {
   EnableLog();
   
   SendOutputToFile("Eagle.log" , "" , false);
   
   (void)argc;
   (void)argv;
   
///   EagleSystem* sys = Eagle::EagleLibrary::System("Allegro5");
///   EagleSystem* sys = Eagle::EagleLibrary::System("Allegro5");
   Allegro5System* sys = GetAllegro5System();
   
   int sw = 1200;
   int sh = 900;
   
   if (EAGLE_FULL_SETUP != sys->Initialize(EAGLE_FULL_SETUP)) {
      EagleWarn() << "Failed to install some components." << std::endl;
   }
   EagleGraphicsContext* win = sys->GetWindowManager()->CreateEagleWindow("win" , sw , sh , EAGLE_OPENGL | EAGLE_WINDOWED | EAGLE_RESIZABLE);
   
   win->Clear(EagleColor(0,255,255));
   win->FlipDisplay();

   std::string bgfile = "stallions.jpg";
   EagleImage* bg = win->LoadImageFromFile(bgfile);
   if (!bg || (bg && !bg->Valid())) {
      EagleError() << StringPrintF("Failed to load %s from disk.\n" , bgfile.c_str());
      return -1;
   }
   
   /// Setup some custom colors for our margin, border, and padding
   SHAREDOBJECT<WidgetColorset> pwc = GetColorsetByName("Default");
   EAGLE_ASSERT(pwc);
   WidgetColorset& wc = *pwc.get();
   wc[PADCOL] = EagleColor(127,127,127,255);
   wc[BORDCOL] = EagleColor(255,255,255,255);
   wc[MARGCOL] = EagleColor(0,0,0,255);
   
   
   /// Outer root gui
   WidgetHandler gui(win , "WidgetHandler" , "GUI1");
   gui.SetWidgetArea(WIDGETAREA(10 , 15 , 25 , Rectangle(150,150,900,600)) , false);
   
   
   BasicScrollBar hscroller("BasicScrollBar" , "hscroller");
   BasicScrollBar vscroller("BasicScrollBar" , "vscroller");

   hscroller.SetScrollDirection(true);
   vscroller.SetScrollDirection(false);
   
   hscroller.SetupView(100,25);
   vscroller.SetupView(100,25);
   
/*
   int hx = 0;
   int hy = 0;
   BasicScrollButton bsbleft;
   BasicScrollButton bsbright;
   BasicScrollButton bsbup;
   BasicScrollButton bsbdown;
   
   bsbleft.SetScrollDirection(true , true);
   bsbright.SetScrollDirection(false , true);
   bsbup.SetScrollDirection(true , false);
   bsbdown.SetScrollDirection(false , false);
//*/
/*
   BasicButton bsbleft;
   BasicButton bsbright;
   BasicButton bsbup;
   BasicButton bsbdown;
//*/   

   RelativeLayout rl;
   
/*
   rl.AddWidget(&hscroller , LayoutRectangle(0.3,0.1,0.6,0.2));
   rl.AddWidget(&vscroller , LayoutRectangle(0.1,0.3,0.2,0.6));

   rl.AddWidget(&bsbleft , LayoutRectangle(0.4,0.5,0.1,0.1));
   rl.AddWidget(&bsbright , LayoutRectangle(0.6,0.5,0.1,0.1));
   rl.AddWidget(&bsbup , LayoutRectangle(0.5,0.4,0.1,0.1));
   rl.AddWidget(&bsbdown , LayoutRectangle(0.5,0.6,0.1,0.1));
   
   BasicText hlabel;
   BasicText vlabel;
   
   hlabel.SetFont(win->DefaultFont());
   vlabel.SetFont(win->DefaultFont());
   
   rl.AddWidget(&hlabel , LayoutRectangle(0.6 , 0.05 , 0.3 , 0.1));
   rl.AddWidget(&vlabel , LayoutRectangle(0.05 , 0.6 , 0.1 , 0.3));
   
   gui.SetRootLayout(&rl);
*/   
//**   
   BasicIcon stallion_icon;
   stallion_icon.SetImage(StackObject(bg));
   
   stallion_icon.SetWidgetArea(Rectangle(0,0,bg->W() , bg->H()) , false);
   
   ScrollArea scrollview("IconScroller");
   
   
//   scrollview.SetScrollbarPosition(false , false);
   scrollview.SetScrollbarPosition(true , true);
   scrollview.PlaceWidget(&stallion_icon , 2);
   
   rl.AddWidget(&scrollview , LayoutRectangle(0,0,1,1));
///   scrollview.SetWidgetArea(Rectangle(150 , 100 , 600 , 400) , false);
///   gui.GiveWidgetFocus(&scrollview);
//*/   

   gui.SetRootLayout(&rl);

   EagleLog() << "******* SETUP COMPLETE ********" << std::endl;
   EagleLog() << gui << std::endl << std::endl;
   EagleLog() << "******* SETUP COMPLETE ********" << std::endl;
         
   bool quit = false;
   bool redraw = true;
   
//   int mx = 0;
//   int my = 0;
   
   EagleFont* pfont = win->LoadFont("verdana.ttf" , -32);
   EAGLE_ASSERT(pfont && pfont->Valid());
   
   const char* txt = "Hello Popup";
   
   EagleInfo() << "[" << txt << "] dim = " << pfont->Width(txt) << " x " << pfont->Height() << std::endl;
   
   PopupText ptext(sw/2 - 50 , sh/2 - 10 , EAGLE_OPENGL , std::string(txt) , pfont);
   
   EagleInfo() << ptext << std::endl;
   
   ptext.Hide();
   bool popup_hidden = true;
   
   sys->GetSystemTimer()->Start();

   while (!quit) {
      if (redraw) {
         win->DrawToBackBuffer();
         win->Clear(EagleColor(0,0,0));

         gui.Display(win , 0 , 0);
         win->FlipDisplay();
         if (!popup_hidden) {
            ptext.Display();
         }
         redraw = false;
      }
      do {
         EagleEvent ev = sys->WaitForSystemEventAndUpdateState();
         
         if (!popup_hidden) {
            if (ev.window != win) {
               if (ptext.HandleEvent(ev) & DIALOG_INPUT_USED) {
                  continue;
               }
            }
         }
         if (ev.type != EAGLE_EVENT_TIMER && ev.type != EAGLE_EVENT_MOUSE_AXES) {
            /// Log non timer and non mouse axes events
            EagleInfo() << "Event " << EagleEventName(ev.type) << " received in main." << std::endl;
         }
         
         if (ev.type == EAGLE_EVENT_KEY_DOWN && ev.keyboard.keycode == EAGLE_KEY_F1) {
            popup_hidden = !popup_hidden;
            if (popup_hidden) {
               ptext.Hide();
            }
            else {
               ptext.Show();
               EagleInfo() << "Giving focus to " << 
                  sys->GetWindowManager()->GiveWindowFocus(ptext.OurWindow()->GetEagleId()) << std::endl;
            }
         }
         
         /// Event handling
         if (ev.type == EAGLE_EVENT_DISPLAY_CLOSE) {
            quit = true;
         }
         else if (ev.type == EAGLE_EVENT_KEY_DOWN && ev.keyboard.keycode == EAGLE_KEY_ESCAPE) {
            quit = true;
         }
         else if (ev.type == EAGLE_EVENT_TIMER) {
            gui.Update(ev.timer.eagle_timer_source->SPT());
            redraw = true;
         }
         else {
            gui.HandleEvent(ev);
         }
         
         /// Check our gui for messages
         while (gui.HasMessages()) {
            WidgetMsg wmsg = gui.TakeNextMessage();
            EagleLog() << "Widget Message [" << wmsg << "]" << std::endl;
            ///const WidgetMsg scrollbar_msg(this , TOPIC_SCROLL_WIDGET , SCROLL_VALUE_CHANGED);
            if (wmsg.IsMessageTopic(TOPIC_SCROLL_WIDGET , SCROLL_VALUE_CHANGED)) {
               std::string name = wmsg.from->ShortName();
               EagleLog() << StringPrintF("Message from widget %s at %p (h = %p , v = %p)\n" ,
                                           name.c_str() , wmsg.from , &hscroller , &vscroller);
               if (wmsg.from == &hscroller) {
//                  hlabel.SetText(StringPrintF("%d" , hscroller.GetScrollValue()));
               }
               else if (wmsg.from == &vscroller) {
//                  vlabel.SetText(StringPrintF("%d" , vscroller.GetScrollValue()));
               }
            }
         }
         if (gui.FlagOn(NEEDS_REDRAW)) {
            redraw = true;
         }
         
      } while (!sys->UpToDate());
   }
   
///   sys->GetSystemQueue()->WaitForEvent(EAGLE_EVENT_KEY_DOWN , 0);
   
   EagleLog() << "Exited main loop." << std::endl;
   
///   sys->Shutdown();
   atexit(Eagle::EagleLibrary::ShutdownEagle);

   return 0;
}



int main2(int argc , char** argv) {

   EnableLog();
   
   SendOutputToFile("Eagle.log" , "" , false);
   
   (void)argc;
   (void)argv;
   
///   EagleSystem* sys = Eagle::EagleLibrary::System("Allegro5");
///   EagleSystem* sys = Eagle::EagleLibrary::System("Allegro5");
   Allegro5System* sys = GetAllegro5System();
   
   int sw = 1200;
   int sh = 900;
   
   if (EAGLE_FULL_SETUP != sys->Initialize(EAGLE_FULL_SETUP)) {
      EagleWarn() << "Failed to install some components." << std::endl;
   }
   EagleGraphicsContext* win = sys->GetWindowManager()->CreateEagleWindow("win" , sw , sh , EAGLE_OPENGL | EAGLE_WINDOWED | EAGLE_RESIZABLE);
   
   win->Clear(EagleColor(0,255,255));
   win->FlipDisplay();

   std::string bgfile = "stallions.jpg";
   EagleImage* bg = win->LoadImageFromFile(bgfile);
   if (!bg || (bg && !bg->Valid())) {
      EagleError() << StringPrintF("Failed to load %s from disk.\n" , bgfile.c_str());
      return -1;
   }
   
   /// Setup some custom colors for our margin, border, and padding
   SHAREDOBJECT<WidgetColorset> pwc = GetColorsetByName("Default");
   EAGLE_ASSERT(pwc);
   WidgetColorset& wc = *pwc.get();
   wc[PADCOL] = EagleColor(127,127,127,255);
   wc[BORDCOL] = EagleColor(255,255,255,255);
   wc[MARGCOL] = EagleColor(0,0,0,255);
   
   
   /// Outer root gui
   WidgetHandler gui(win , "WidgetHandler" , "GUI1");
   gui.SetWidgetArea(WIDGETAREA(10 , 15 , 25 , Rectangle(150,150,900,600)) , false);
   
   /// A WidgetMover allows us to move any widgets we want
   WidgetMover wmover("Widget mover");
   wmover.SetWidgetArea(Rectangle(-1000,-1000,1,1) , false);
   wmover.SetHotKey(input_key_held(EAGLE_KEY_LSHIFT) && input_key_press(EAGLE_KEY_ENTER));
   wmover.SetAbilities(true , true);

   gui << wmover;
   
   /// Inner gui
   WidgetHandler gui2(win , "WidgetHandler" , "GUI2");
   gui2.SetupBuffer(800,600,win);
   gui2.SetWidgetArea(WIDGETAREA(5,10,15 , Rectangle(100,30,640,480)) , false);
   gui.AllowMiddleMouseButtonScroll(false);
   gui2.AllowMiddleMouseButtonScroll(true);

   gui << gui2;

   /// We can have transparent backgrounds in our gui
   gui.SetBackgroundColor(EagleColor(127,127,127,255));
   gui2.SetBackgroundColor(EagleColor(127,127,127,255));

   /// A RelativeLayout allows us to keep relative positions and sizes of our widgets
   RelativeLayout rl1("RLAYOUT1");
   rl1.Resize(5);
   rl1.SetLayoutRectangle(0 , LayoutRectangle(0.2 , 0.2 , 0.6 , 0.2));
   rl1.SetLayoutRectangle(1 , LayoutRectangle(0.2 , 0.6 , 0.6 , 0.2));
   rl1.SetLayoutRectangle(2 , LayoutRectangle(0.4 , 0.2 , 0.2 , 0.6));
   rl1.SetLayoutRectangle(3 , LayoutRectangle(0.1 , 0.2 , 0.2 , 0.6));
   rl1.SetLayoutRectangle(4 , LayoutRectangle(0.7 , 0.2 , 0.2 , 0.6));
   
   gui2.SetRootLayout(&rl1);
   
   TestWidget tw1("TESTWIDGETH1");
   TestWidget tw2("TESTWIDGETH2");
   TestWidget tw3("TESTWIDGETV1");
   TestWidget tw4("TESTWIDGETV2");
   TestWidget tw5("TESTWIDGETV3");
   
   rl1.PlaceWidget(&tw1 , 0);
   rl1.PlaceWidget(&tw2 , 1);
   rl1.PlaceWidget(&tw3 , 2);
   rl1.PlaceWidget(&tw4 , 3);
   rl1.PlaceWidget(&tw5 , 4);
   
   tw1.SetWidgetArea(WIDGETAREA(tw1.OuterArea() , 3,5,7));
   tw2.SetWidgetArea(WIDGETAREA(tw2.OuterArea() , 7,5,3));
   tw3.SetWidgetArea(WIDGETAREA(tw3.OuterArea() , 2,4,6));
   tw4.SetWidgetArea(WIDGETAREA(tw4.OuterArea() , 6,4,2));
   tw5.SetWidgetArea(WIDGETAREA(tw5.OuterArea() , 5,5,5));
   
   EagleLog() << "******* SETUP COMPLETE ********" << std::endl;
         
   bool quit = false;
   bool redraw = true;
   
   int mx = 0;
   int my = 0;
   
   sys->GetSystemTimer()->Start();
   
   while (!quit) {
      if (redraw) {
         win->DrawToBackBuffer();
         win->Clear(EagleColor(0,0,0));
         win->DrawStretched(bg , Rectangle(0 , 0 , sw , sh));
         gui.Display(win , 0 , 0);
///         WidgetBase* hw = gui.GetWidgetAt(mx,my);
         WidgetBase* hw = wmover.GetMoveWidget();
         std::string name = (hw?hw->FullName():"NULL");
         win->DrawTextString(win->DefaultFont() , StringPrintF("Widget at %d,%d is [%s]" , mx , my , name.c_str()) , sw - 10 , sh - win->DefaultFont()->Height() - 5 , EagleColor(0,0,0) , HALIGN_RIGHT , VALIGN_TOP);
         win->FlipDisplay();
         redraw = false;
      }
      do {
         EagleEvent ev = sys->WaitForSystemEventAndUpdateState();
         if (ev.type != EAGLE_EVENT_TIMER && ev.type != EAGLE_EVENT_MOUSE_AXES) {
            /// Log non timer and non mouse axes events
            EagleInfo() << "Event " << EagleEventName(ev.type) << " received in main." << std::endl;
         }
         
         /// Event handling
         if (ev.type == EAGLE_EVENT_DISPLAY_CLOSE) {
            quit = true;
         }
         else if (ev.type == EAGLE_EVENT_KEY_DOWN && ev.keyboard.keycode == EAGLE_KEY_ESCAPE) {
            quit = true;
         }
         else if (ev.type == EAGLE_EVENT_TIMER) {
            gui.Update(ev.timer.eagle_timer_source->SPT());
            redraw = true;
         }
         else {
            gui.HandleEvent(ev);
         }
         
         /// Check our gui for messages
         while (gui.HasMessages()) {
            WidgetMsg wmsg = gui.TakeNextMessage();
            EagleLog() << "Widget Message [" << wmsg << "]" << std::endl;
         }
         if (gui.Flags().FlagOn(NEEDS_REDRAW)) {
            redraw = true;
         }
         
      } while (!sys->UpToDate());
   }
   
///   sys->GetSystemQueue()->WaitForEvent(EAGLE_EVENT_KEY_DOWN , 0);
   
   EagleLog() << "Exited main loop." << std::endl;
   
///   sys->Shutdown();
   atexit(Eagle::EagleLibrary::ShutdownEagle);

   return 0;
}





void Hexagon::GenPoints() {
   double theta = 0.0;
   verts.clear();
   verts.resize(NUM_HEX_CORNERS , ALLEGRO_VERTEX());
   for (unsigned int i = 0 ; i < NUM_HEX_CORNERS ; ++i) {
      ALLEGRO_VERTEX* v = &verts[i];
      v->x = radius*cos(theta);
      v->y = radius*sin(theta);
      v->z = 0.0;
      v->u = 0;
      v->v = 0;
      v->color = al_map_rgb(0,0,0);
      theta -= M_PI/3.0;
   }
}



Hexagon::Hexagon(double width) :
      radius(fabs(width/2.0)),
      verts()
{
   SetRadius(fabs(width/2.0));
}



void Hexagon::SetRadius(double r) {
   radius = r;
   GenPoints();
}



void Hexagon::DrawFilled(EagleGraphicsContext* win , double x , double y , ALLEGRO_COLOR color) { 
   (void)win;
   std::vector<float> vtx(2*verts.size() , 0.0);
   for (unsigned int i = 0 ; i < NUM_HEX_CORNERS ; ++i) {
      vtx[2*i + 0] = x + verts[i].x;
      vtx[2*i + 1] = y + verts[i].y;
   }
   al_draw_filled_polygon(&vtx[0] , 6 , color);
}



void Hexagon::DrawOutline(EagleGraphicsContext* win , double x , double y , ALLEGRO_COLOR color) { 
   (void)win;
   std::vector<float> vtx(2*verts.size() , 0.0);
   for (unsigned int i = 0 ; i < NUM_HEX_CORNERS ; ++i) {
      vtx[2*i + 0] = x + verts[i].x;
      vtx[2*i + 1] = y + verts[i].y;
   }
   al_draw_polygon(&vtx[0] , 6 , ALLEGRO_LINE_JOIN_MITER , color , 2.0 , 10.0);
}



/// -------------------------     HexTile     -----------------------



Hexagon HexTile::proto;


void HexTile::CalcBorders() {
   border.clear();
   for (unsigned int i = 0 ; i < NUM_HEX_DIRECTIONS ; ++i) {
      HexTile* n = neighbors[i];
      if (n->owner != 0) {
         border.insert(owner);
      }
   }
}



void HexTile::DrawFilled(EagleGraphicsContext* win , double x , double y , ALLEGRO_COLOR color) {
   proto.DrawFilled(win , x , y , color);
}



void HexTile::DrawOutline(EagleGraphicsContext* win , double x , double y , ALLEGRO_COLOR color) {
   proto.DrawOutline(win , x , y , color);
}



/// ------------------------    HexGrid     -------------------------



HexGrid::HexGrid() :
      w(0),
      h(0),
      rad(0),
      grid()
{}



void HexGrid::Resize(unsigned int width , unsigned int height) {
   
   w = width;
   h = height;
   grid.clear();
   grid.resize(height , std::vector<HexTile>(width , HexTile()));
   for (unsigned int y = 0 ; y < height ; ++y) {
      for (unsigned int x = 0 ; x < width ; ++x) {
         grid[y][x].neighbors.clear();
         grid[y][x].neighbors.resize(6 , NULL);
         
         /// In a hex grid, n sometimes changes the y value
         /// In a hex grid, s doesn't always change the y value
         const int N = (x%2==0)?y:y-1;
         const int S = (x%2==0)?y+1:y;
         
         /// SOUTH
         if (y < height - 1) {
            grid[y][x].neighbors[HD_SOUTH] = &grid[y + 1][x];
            /// SOUTHEAST
            if (x < width - 1) {
               grid[y][x].neighbors[HD_SOUTHEAST] = &grid[S][x + 1];
            }
            /// SOUTHWEST
            if (x > 0) {
               grid[y][x].neighbors[HD_SOUTHWEST] = &grid[S][x - 1];
            }
         }
         /// NORTH
         if (y > 0) {
            grid[y][x].neighbors[HD_NORTH] = &grid[y - 1][x];
            /// NORTHEAST
            if (x < width - 1) {
               grid[y][x].neighbors[HD_NORTHEAST] = &grid[N][x + 1];
            }
            /// NORTHWEST
            if (x > 0) {
               grid[y][x].neighbors[HD_NORTHWEST] = &grid[N][x - 1];
            }
         }
      }
   }
}



void HexGrid::SetRadius(unsigned int radius) {
   rad = radius;
   if (grid.size()) {
      if (grid[0].size()) {
         grid[0][0].SetRadius(radius);
      }
   }
}



void HexGrid::DrawGrid(EagleGraphicsContext* win , int xpos , int ypos , std::map<int , ALLEGRO_COLOR> colors) {
   static const double root3 = sqrt(3);
   
   /// Width and height of grid
   const double dx = 1.5*rad;
   const double dy = root3*rad;

   /// Top left
   const double ty = ypos;
   const double lx = xpos;

   /// Draw team colors
   for (unsigned int row = 0 ; row < h ; ++row) {
      double y = ty + dy*row;
      for (unsigned int col = 0 ; col < w ; ++col) {
         double ly = (col%2==0)?y:(y-dy/2.0);/// Offset odd columns up by dy
         double x = lx + dx*col;
         grid[row][col].DrawFilled(win , x , ly , colors[grid[row][col].owner]);
      }
   }
   /// Outline in white
   for (unsigned int row = 0 ; row < h ; ++row) {
      double y = ty + dy*row;
      for (unsigned int col = 0 ; col < w ; ++col) {
         double ly = (col%2==0)?y:(y-dy/2.0);/// Offset odd columns up by dy
         double x = lx + dx*col;
         grid[row][col].DrawOutline(win , x , ly , al_map_rgb(255,255,255));
      }
   }
}



/// ------------------------     HexGame     -----------------------------



void HexGame::Claim(HexTile* tile , int owner) {
   if (!tile) {return;}
   tile->owner = owner;
   if (owner > 0) {
      std::vector<HexTile*>& nb = tile->neighbors;
      for (unsigned int i = 0 ; i < NUM_HEX_CORNERS ; ++i) {
         HexTile* neighbor = nb[i];
         neighbor->border.insert(owner);
      }
   }
   else {
      std::vector<HexTile*>& nb = tile->neighbors;
      for (unsigned int i = 0 ; i < NUM_HEX_CORNERS ; ++i) {
         HexTile* neighbor = nb[i];
         neighbor->CalcBorders();
      }
   }
}



HexGame::HexGame() :
      hgrid(),
      rad(0.0),
      teamcolors(),
      turn(0),
      mx(0),
      my(0),
      hover(0)
{
   teamcolors[0] = al_map_rgb(0,0,0);
   teamcolors[1] = al_map_rgb(255,0,0);
   teamcolors[2] = al_map_rgb(0,255,0);
   teamcolors[3] = al_map_rgb(0,0,255);
}
   


void HexGame::Resize(unsigned int width , unsigned int height , double radius) {
   rad = radius;
   if (radius < 10.0) {radius = 10.0;}
   hgrid.Resize(width,height);
   hgrid.SetRadius(radius);
}
   
   

void HexGame::HandleEvent(EagleEvent ee) {
   if (ee.type == EAGLE_EVENT_MOUSE_AXES) {
      mx = ee.mouse.x;
      my = ee.mouse.y;
      HexTile* oldhover = hover;
      hover = 0;
      static const double root3 = sqrt(3.0);
      const double irad = rad*root3;
      const double lx = 0.0 - irad*0.5*root3;
      int col = (int)(((double)mx + 0.5 - lx)/(irad*root3));
      double ly = 0.0;
      if (col % 2 == 1) {
         ly -= rad*0.5*root3;
      }
      int row = (int)(((double)my + 0.5) - ly)/(rad*root3);
      if (col >= 0 && col < (int)hgrid.Width()) {
         if (row >= 0 && row < (int)hgrid.Height()) {
            hover = &(hgrid.grid[row][col]);
         }
      }
   }
   if (ee.type == EAGLE_EVENT_MOUSE_BUTTON_DOWN) {
      if (hover) {
         if (hover->owner != 0) {
            Claim(hover , turn);
         }
      }
   }
}



void HexGame::DisplayOn(EagleGraphicsContext* win , int xpos , int ypos) {
   hgrid.DrawGrid(win , xpos , ypos , teamcolors);
}



void HexGame::Run(EagleSystem* sys , EagleGraphicsContext* win) {
   EagleEventHandler* q = sys->GetSystemQueue();
   
   bool quit = false;
   bool redraw= true;
   do {
      if (redraw) {
         DisplayOn(win , 0 , 0);
         redraw = false;
         win->FlipDisplay();
      }
      do {
         EagleEvent ev = sys->WaitForSystemEventAndUpdateState();
      
         if (ev.type == EAGLE_EVENT_DISPLAY_CLOSE) {
            quit = true;
         }
         if (ev.type == EAGLE_EVENT_KEY_DOWN && ev.keyboard.keycode == EAGLE_KEY_ESCAPE) {
            quit = true;
         }
         
         HandleEvent(ev);
         
         if (ev.type == EAGLE_EVENT_TIMER) {
            redraw = true;
         }
         
      } while (q->HasEvent(0));
   } while (!quit);
}
