


#include "Eagle/backends/Allegro5/Allegro5Clipboard.hpp"
#include "Eagle/Lib.hpp"
#include "Eagle/System.hpp"
#include "Eagle/backends/Allegro5/Allegro5GraphicsContext.hpp"



Allegro5Clipboard::Allegro5Clipboard(std::string objname) :
      EagleClipboard("Allegro5Clipboard" , objname)
{}



void Allegro5Clipboard::CopyToClipboard(std::string str) {
   EagleClipboard::CopyToClipboard(str);
   /// Allegro 5 stuff here
   EagleGraphicsContext* win = Eagle::EagleLibrary::System("Allegro5")->GetWindowManager()->GetActiveWindow();
   EAGLE_ASSERT(win);
   Allegro5GraphicsContext* awin = dynamic_cast<Allegro5GraphicsContext*>(win);
   EAGLE_ASSERT(awin);
   ALLEGRO_DISPLAY* display = awin->AllegroDisplay();
   EAGLE_ASSERT(display);
   
   al_set_clipboard_text(display , str.c_str());
}



std::string Allegro5Clipboard::GetClipboardString() {
   /// Allegro 5 stuff here
   EagleGraphicsContext* win = Eagle::EagleLibrary::System("Allegro5")->GetWindowManager()->GetActiveWindow();
   EAGLE_ASSERT(win);
   Allegro5GraphicsContext* a5win = dynamic_cast<Allegro5GraphicsContext*>(win);
   EAGLE_ASSERT(a5win);
   ALLEGRO_DISPLAY* display = a5win->AllegroDisplay();
   EAGLE_ASSERT(display);
   char* allegro_text = al_get_clipboard_text(display);
   clipboard_string = allegro_text;
   al_free(allegro_text);
   return EagleClipboard::GetClipboardString();
}



