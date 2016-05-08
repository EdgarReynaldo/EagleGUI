


#include "Eagle\backends\Allegro5Backend.hpp"

#include "Eagle.hpp"

#include "TextTestMain.hpp"



int TextTestMain(int argc , char** argv) {
   
   (void)argc;
   (void)argv;
   
   EagleSystem* sys = new Allegro5System();
   
   
   if (EAGLE_FULL_SETUP != sys->Initialize(EAGLE_FULL_SETUP)) {
      return 1;
   }
   
   int ww = 800;
   int wh = 600;
   EagleGraphicsContext* win = sys->CreateGraphicsContext(ww,wh,EAGLE_OPENGL | EAGLE_WINDOWED | EAGLE_RESIZABLE);
   if (!win->Valid()) {
      return 2;
   }
   
/**
   float x = 200;
   float y = 150;
   float w = 400;
   float h = 300;
   ALLEGRO_COLOR fillcolor = al_map_rgb(0,255,0);
   ALLEGRO_COLOR linecolor = al_map_rgb(0,0,0);
   
   float thickness = 10.0;
   
   win->DrawToBackBuffer();
   al_clear_to_color(al_map_rgb(0,0,255));
   al_draw_filled_rectangle(x , y , x + w , y + h , fillcolor);
   al_draw_rectangle(x , y , x + w , y + h , linecolor , thickness);
   win->FlipDisplay();
   sys->GetSystemQueue()->Clear();
   sys->GetSystemQueue()->WaitForEvent(ALLEGRO_EVENT_KEY_DOWN);
   sys->Rest(3.0);
//*/
   

   Allegro5Font textfont("Verdana.ttf" , 20 , LOAD_FONT_MONOCHROME);
   Allegro5Font textfont2("Consola.ttf" , -20 , LOAD_FONT_MONOCHROME);
   Allegro5Font textfont3("AlexBrush.ttf" , -30 , LOAD_FONT_MONOCHROME);
   
   EAGLE_ASSERT(textfont.Valid());
   EAGLE_ASSERT(textfont2.Valid());
   EAGLE_ASSERT(textfont3.Valid());
   

   WidgetHandler* gui = new WidgetHandler();
   gui->SetDrawWindow(win);
   gui->SetupBufferDimensions(ww , wh);
   gui->SetWidgetArea(0,0,ww,wh);
   gui->SetFocusDrawType(FOCUS_HIGHLIGHT_THICK_DOTTED);
   
   RelativeLayout rel;
   gui->SetRootLayout(&rel);

   EagleImage* up = win->LoadImageFromFile("Data/GuiTestImages/RBA_Up.png");
   EagleImage* down = win->LoadImageFromFile("Data/GuiTestImages/RBA_Down.png");
   EagleImage* hover = win->LoadImageFromFile("Data/GuiTestImages/RBA_Hover.png");

   IconButton icon;
   
   icon.SetImages(up , down , hover , down);
   
   
   rel.AddWidget(&icon , LayoutRectangle(0.1,0.6,0.4,0.3));
   
   GridLayout button_grid;
   button_grid.ResizeGrid(5,5);
   
   rel.AddWidget(&button_grid , LayoutRectangle(0.5,0.6,0.5,0.3));

   BasicScrollButton scroll_buttons[4];
   
   scroll_buttons[0].SetScrollDirection(true,true);/// left
   scroll_buttons[1].SetScrollDirection(false,true);/// right
   scroll_buttons[2].SetScrollDirection(true,false);/// up
   scroll_buttons[3].SetScrollDirection(false,false);/// down

   button_grid.PlaceWidget(&scroll_buttons[0] , 10 , false);
   button_grid.PlaceWidget(&scroll_buttons[1] , 14 , false);
   button_grid.PlaceWidget(&scroll_buttons[2] , 2 , false);
   button_grid.PlaceWidget(&scroll_buttons[3] , 22 , false);
   
/**
   BasicButton basicbutton;
   
   rel.AddWidget(&basicbutton , LayoutRectangle(0.5,0.6,0.5,0.3));
      
   Rectangle r = basicbutton.InnerArea();
   r.MoveBy(-r.X() , -r.Y());
   
   Triangle triangle(r.X() , r.Y() , (r.X() + r.W()/2) , r.BRY() , r.BRX() , r.Y()+r.H()/2);
   
   basicbutton.SetClickArea(&triangle , false);
*/

   DumbText dt;
   dt.SetupText(HALIGN_LEFT , VALIGN_TOP , 10 , 10 , 0 , "" , &textfont);
   dt.SetText("This is just dumb text." , &textfont);
   
   rel.AddWidget(&dt , LayoutRectangle(0.1,0.1,0.8,0.1));
   
   HALIGNMENT halign = HALIGN_LEFT;
   VALIGNMENT valign = VALIGN_TOP;

   SelectText select;
   select.SetupText(halign , valign , 0,0,0 , "" , &textfont2);
   select.SetText("This text is part of a TextSelect object.\nYou can select some or all of multiple\nlines of text." , &textfont2);
   
   rel.AddWidget(&select , LayoutRectangle(0.1,0.2,0.8,0.2));
   
   LinkText linktext;
   linktext.SetupText(HALIGN_LEFT , VALIGN_TOP , 20 , 20 , 0 , "" , &textfont);
   linktext.SetText("http://www.allegro.cc" , &textfont);
   
   dt.SetMarginsContractFromOuter(10,10,10,10);
   select.SetMarginsContractFromOuter(10,10,10,10);
   linktext.SetMarginsContractFromOuter(10,10,10,10);
   
   rel.AddWidget(&linktext , LayoutRectangle(0.1 , 0.4 , 0.8 , 0.2));
   
///   EagleLog() << "Dumb text object :" << std::endl;
///   EagleLog() << dt << std::endl;
///   EagleLog() << "Select Text object :" << std::endl;
///   EagleLog() << select << std::endl;
   EagleLog() << "Link Text object : " << std::endl;
   EagleLog() << linktext << std::endl;
   
   
   int tickcount = 0;
   bool quit = false;
   bool redraw = true;
   float fps = 0.0f;
   
   
///   sys->GetSystemTimer()->Create(0.5);
   sys->GetSystemTimer()->Start();
   
   do {
      if (redraw) {
         ++tickcount;
///         win->Clear(EagleColor(0,tickcount*4,0));

         
         if (tickcount == 20) {
            fps = win->GetFPS();
            tickcount = 0;
         }
         
         gui->Display(win,0,0);
         win->DrawTextString(&textfont , StringPrintF("%5.1f" , fps) , 10,10,EagleColor(0,255,0));

         Rectangle r = select.TextArea();
                  
         win->DrawRectangle(r , 2 , EagleColor(0,255,0));
                  
         bool up = icon.Up();
         bool hover = icon.Hover();
         
         win->DrawTextString(&textfont , StringPrintF("Button state = %s and %s" , up?"up":"down" , hover?"hover":"nohover"),
                             10,30,EagleColor(0,0,255));
                  
         win->FlipDisplay();
         redraw = false;
/**
         int caretpos = -1;
         int caretline = -1;

        select.FindCaretPos(gui->GetMouseX() , gui->GetMouseY() , &caretpos , &caretline);
         
         win->DrawTextString(&textfont2 , StringPrintF("Caret pos = %d , Carent line = %d" , caretpos , caretline),
                             10 , 10 , EagleColor(0,255,0));
         win->DrawTextString(&textfont2 , StringPrintF("Mouse x = %d , Mouse y = %d" , gui->GetMouseX() , gui->GetMouseY()),
                             10 , 40 , EagleColor(0,255,0));
//*/
                                      
      }
      
      do {
         EagleEvent ee;
         ee = sys->WaitForSystemEventAndUpdateState();
         
         if (ee.type == EAGLE_EVENT_DISPLAY_RESIZE) {
            win->AcknowledgeResize();
            gui->SetWidgetArea(0 , 0 , win->Width() , win->Height());
         }

         gui->HandleEvent(ee);

         while (gui->HasMessages()) {
            WidgetMsg wmsg = gui->TakeNextMessage();
            EagleLog() << wmsg << std::endl;
         }
         if (ee.type == EAGLE_EVENT_KEY_DOWN) {
               if (ee.keyboard.keycode == EAGLE_KEY_1) {halign = HALIGN_LEFT;}
               if (ee.keyboard.keycode == EAGLE_KEY_2) {halign = HALIGN_CENTER;}
               if (ee.keyboard.keycode == EAGLE_KEY_3) {halign = HALIGN_RIGHT;}
               if (ee.keyboard.keycode == EAGLE_KEY_4) {valign = VALIGN_TOP;}
               if (ee.keyboard.keycode == EAGLE_KEY_5) {valign = VALIGN_CENTER;}
               if (ee.keyboard.keycode == EAGLE_KEY_6) {valign = VALIGN_BOTTOM;}
               select.Realign(halign , valign);
         }

         if (ee.type == EAGLE_EVENT_DISPLAY_CLOSE) {
            quit = true;
         }
         if (ee.type == EAGLE_EVENT_KEY_DOWN && ee.keyboard.keycode == EAGLE_KEY_ESCAPE) {
            quit = true;
         }
         if (ee.type == EAGLE_EVENT_TIMER) {
            redraw = true;
         }
         
      } while(!sys->UpToDate());
   } while (!quit);
    
   return 0;
}




