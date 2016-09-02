


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
   EagleGraphicsContext* win = sys->CreateGraphicsContext(ww , wh , EAGLE_OPENGL | EAGLE_WINDOWED | EAGLE_RESIZABLE);
///   EagleGraphicsContext* win = sys->CreateGraphicsContext(ww , wh , EAGLE_WINDOWED | EAGLE_RESIZABLE);
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

   
/**
   EagleImage* i1 = win->LoadImageFromFile("Data/temp/00001.png");
   EagleImage* i2 = win->LoadImageFromFile("Data/temp/00002.png");
   EagleImage* i3 = win->LoadImageFromFile("Data/temp/00003.png");
   EagleImage* i4 = win->LoadImageFromFile("Data/temp/00004.png");
   EagleImage* i5 = win->LoadImageFromFile("Data/temp/Four.png");
   EagleImage* i6 = win->LoadImageFromFile("Data/temp/Four.jpg");
   
   if (!i1->Valid()) {
      EagleLog() << "Failed to load image." << std::endl;
   }
   
   win->Clear(EagleColor(0,0,0));
   win->Draw(i1 , 0 , 0 , HALIGN_LEFT , VALIGN_TOP);
   win->FlipDisplay();
   
   sys->Rest(3.0);
//*/
   
   
   
   IconButton icon;
   
   icon.SetImages(up , down , hover , down);
   
   
   rel.AddWidget(&icon , LayoutRectangle(0.1,0.6,0.4,0.3));
   
   GridLayout button_grid;
   button_grid.ResizeGrid(5,5);
   
   rel.AddWidget(&button_grid , LayoutRectangle(0.5,0.6,0.4,0.3));

   BasicScroller scroller1;
   BasicScroller scroller2;
   
   scroller1.SetScrollDirection(true);
   scroller2.SetScrollDirection(false);
   scroller1.SetScrollLength(100);
   scroller2.SetScrollLength(100);
   scroller1.SetupView(100,10);
   scroller2.SetupView(100,10);

///   rel.AddWidget(&scroller1 , LayoutRectangle(0.0,0.9,0.9,0.1));
///   rel.AddWidget(&scroller2 , LayoutRectangle(0.9,0.0,0.1,0.9));

   BasicScrollButton scroll_buttons[4];
   
   BasicScrollBar scrollbar1;
   BasicScrollBar scrollbar2;
   
   scrollbar1.SetScrollLength(100);
   scrollbar2.SetScrollLength(100);
   scrollbar1.SetupView(100,10);
   scrollbar2.SetupView(100,10);
   scrollbar1.SetScrollDirection(true);
   scrollbar2.SetScrollDirection(false);

   rel.AddWidget(&scrollbar1 , LayoutRectangle(0.0,0.9,0.9,0.1));
   rel.AddWidget(&scrollbar2 , LayoutRectangle(0.9,0.0,0.1,0.9));

/**   
   scroll_buttons[0].SetScrollDirection(true,true);/// left
   scroll_buttons[1].SetScrollDirection(false,true);/// right
   scroll_buttons[2].SetScrollDirection(true,false);/// up
   scroll_buttons[3].SetScrollDirection(false,false);/// down

   button_grid.PlaceWidget(&scroll_buttons[0] , 10 , false);
   button_grid.PlaceWidget(&scroll_buttons[1] , 14 , false);
   button_grid.PlaceWidget(&scroll_buttons[2] , 2 , false);
   button_grid.PlaceWidget(&scroll_buttons[3] , 22 , false);
*/   
/**
   BasicButton basicbutton;
   
   rel.AddWidget(&basicbutton , LayoutRectangle(0.5,0.6,0.5,0.3));
      
   Rectangle r = basicbutton.InnerArea();
   r.MoveBy(-r.X() , -r.Y());
   
   Triangle triangle(r.X() , r.Y() , (r.X() + r.W()/2) , r.BRY() , r.BRX() , r.Y()+r.H()/2);
   
   basicbutton.SetClickArea(&triangle , false);
*/

   BasicText dt;
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
   
   
   bool quit = false;
   bool redraw = true;
   float fps = 0.0f;
   
   
///   sys->GetSystemTimer()->Create(0.5);
   sys->GetSystemTimer()->Start();
   
   int tick_count = 0;
   bool update_fps = true;
   
   do {
      redraw = true;
      if (redraw) {

         if (update_fps) {
            fps = win->GetFPS();
         }
         
         win->Clear(EagleColor(0,0,0));
         
         gui->Display(win,0,0);
          
         win->DrawTextString(&textfont , StringPrintF("%5.1f" , fps) , 10,10,EagleColor(0,255,0));

         WidgetBase* focus_widget = gui->CurrentFocus();
         
         if (focus_widget) {
///            string focus_name = focus_widget->GetName();
            
///            win->DrawTextString(&textfont , focus_name , 100 , 10 , EagleColor(0,255,255));
            
            Rectangle r = focus_widget->OuterArea();
            
            win->DrawRectangle(r , 2 , EagleColor(0,255,0));
         }
         
         int pos = -1;
         int linenum = -1;
         
         SelectText* pselect = &select;
         
         int select_line = -1;
         int select_pos = -1;
         int caret_line = -1;
         int caret_pos = -1;
         
         linktext.FindCaretPos(gui->GetMouseX() , gui->GetMouseY() , &pos , &linenum);
         
         win->DrawTextString(&textfont , StringPrintF("Pos = %d , Linenum = %d" , pos , linenum) , 100 , 10 , EagleColor(0,255,255));
         
         pselect->GetCaretAttributes(&select_line , &select_pos , &caret_line , &caret_pos);
         
         string select_attr = StringPrintF("SL = %d , SP = %d , CL = %d , CP = %d" , select_line , select_pos , caret_line , caret_pos);
         
         win->DrawTextString(&textfont , select_attr , win->Width() - 10 , 30 , EagleColor(255,255,0) , HALIGN_RIGHT);
         
         bool up = icon.Up();
         bool hover = icon.Hover();
         
///         win->DrawTextString(&textfont , StringPrintF("ScrollBar 1 value = %d , ScrollBar 2 value = %d" ,
///                                                       scrollbar1.GetScrollValue() , scrollbar2.GetScrollValue()) ,
///                             10,30,EagleColor(0,255,255));
         
         win->DrawTextString(&textfont , StringPrintF("Button state = %s and %s" , up?"up":"down" , hover?"hover":"nohover"),
                             10,30,EagleColor(0,255,255));
                  
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
///         ee = sys->WaitForSystemEventAndUpdateState();
         ee = sys->UpdateSystemState();
         
         
         if (ee.type == EAGLE_EVENT_DISPLAY_RESIZE) {
            win->AcknowledgeResize();
            gui->SetWidgetArea(0 , 0 , win->Width() , win->Height());
         }

         gui->HandleEvent(ee);

         if (ee.type == EAGLE_EVENT_TIMER) {
            ++tick_count;
            if (tick_count == 20) {
               update_fps = true;
               tick_count = 0;
            }
            gui->Update(ee.timer.eagle_timer_source->SPT());
         }
         
         while (gui->HasMessages()) {
               WidgetMsg wmsg = gui->TakeNextMessage();
               EAGLE_DEBUG(
                  EagleLog() << wmsg << std::endl;
               );
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
   
   
   EagleLog() << std::endl << "Live object count = " << LiveObjectCount() << std::endl;
   EagleLog() << std::endl;
   
   delete gui;
   delete sys;

   return 0;
}




