
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
 * @file PopupWindow.cpp
 * @brief Implementations for the various popup types
 */

#include "Eagle/Gui/PopupWindow.hpp"
#include "Eagle/Gui/TextWidgets.hpp"




PopupWindow::PopupWindow(std::string system , std::string window_name) :
      WidgetHandler(0 , "PopupGUI" , window_name),
      our_system(0),
      our_window(0)
{
   our_system = Eagle::EagleLibrary::System(system);
   if (!our_system) {
      throw EagleException(StringPrintF("Failed to get system '%s' from Eagle.\n" , system.c_str()));
   }
}



PopupWindow::PopupWindow(int wx , int wy , int width , int height , int flags , std::string system , std::string window_name) :
      WidgetHandler(0 , "Popup Window" , window_name),
      our_system(0),
      our_window(0) 
{
   our_system = Eagle::EagleLibrary::System(system);
   CreatePopupWindow(wx , wy , width , height , flags);
}



PopupWindow::~PopupWindow() {
   FreePopupWindow();
}



void PopupWindow::FreePopupWindow() {
   if (our_system && our_window) {
      FreeImageBuffers();
      our_system->FreeGraphicsContext(our_window);
      our_window = 0;
   }
}



void PopupWindow::CreatePopupWindow(int sx , int sy , int width , int height , int flags) {

   FreePopupWindow();

   EAGLE_ASSERT(our_system);
   
   std::string sname = ShortName();
   our_window = our_system->CreatePopupWindow(sname , width , height , flags , sx , sy);
   if (!our_window) {
      std::string flagstr = PrintDisplayFlags(flags);
      throw EagleException(StringPrintF("Failed to create %d x %d popup window with flags %s\n",
                                        width , height , flagstr.c_str()));
   }
   SetupBuffer(width , height , our_window);
   SetWidgetArea(Rectangle(0 , 0 , width , height) , false);

}



void PopupWindow::Display() {
   our_window->DrawToBackBuffer();
   our_window->Clear();
   WidgetHandler::Display(our_window , 0 , 0);
   our_window->FlipDisplay();
}



void PopupWindow::Hide() {
   our_window->HideWindow();
}



void PopupWindow::Show() {
   our_window->ShowWindow();
   our_system->GetWindowManager()->GiveWindowFocus(our_window->GetEagleId());
}



PopupText::PopupText(int sx , int sy , int flags , std::string message , std::string fontpath , int fontpt) :
      PopupWindow("Any" , "TextPopupWindow"),
      text("Popup label"),
      our_font(0)
{
   
   CreatePopupWindow(-10000 , -10000 , 100 , 24 , flags);/// Create offscreen window to load a font
   
   EagleFont* f = our_font = our_window->GetFont(fontpath , fontpt , FONT_NORMAL);
   EAGLE_ASSERT(f && f->Valid());
   int w = f->Width(message);
   int h = f->Height(message , f->Height()/4);
   
   text.SetupText(message , f , HALIGN_CENTER , VALIGN_CENTER , 2 , 2 , f->Height()/4);
   Rectangle t = text.TextArea();
   AddWidget(&text);
   int tw = t.W();
   int th = t.H();
   
   our_window->ResizeWindow(tw + 4 , th + 4);
   SetupBuffer(tw + 4 , th + 4 , our_window);
   SetWidgetArea(Rectangle(0,0,tw+4,th+4),false);
   text.SetWidgetArea(Rectangle(2 , 2 , tw , th ) , false);
   
   SetRedrawAllFlag();
   
   if (sx != -1 && sy != -1) {
      our_window->SetWindowPosition(sx , sy);
   }
   else {
      EagleSystem* sys = our_window->GetSystem();
   
      GraphicsHardware* g = sys->GetGraphicsHardware();
      
      MONITOR_INFO i = g->GetMonitor(GRAPHICS_OPENGL , 0);
         
      our_window->SetWindowPosition(i.w/2 - w/2 , i.h/2 - h/2);/// Center the window
   }
   
}







