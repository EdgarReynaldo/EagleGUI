


#include "Eagle/backends/Allegro5/Allegro5Clipboard.hpp"
#include "Eagle/Exception.hpp"
#include "Eagle/System.hpp"
#include "Eagle/backends/Allegro5/Allegro5GraphicsContext.hpp"

void Allegro5Clipboard::CopyToClipboard(string str) {
   EagleClipboard::CopyToClipboard(str);
   /// Allegro 5 stuff here
   EAGLE_ASSERT(eagle_system);
   EagleGraphicsContext* win = eagle_system->GetWindow(0);
   EAGLE_ASSERT(win);
   Allegro5GraphicsContext* awin = dynamic_cast<Allegro5GraphicsContext*>(win);
   EAGLE_ASSERT(awin);
   ALLEGRO_DISPLAY* display = awin->AllegroDisplay();
   EAGLE_ASSERT(display);
   
   al_set_clipboard_text(display , str.c_str());
}



string Allegro5Clipboard::GetClipboardString() {
   /// Allegro 5 stuff here
   EAGLE_ASSERT(eagle_system);
   EagleGraphicsContext* win = eagle_system->GetWindow(0);
   EAGLE_ASSERT(win);
   Allegro5GraphicsContext* awin = dynamic_cast<Allegro5GraphicsContext*>(win);
   EAGLE_ASSERT(awin);
   ALLEGRO_DISPLAY* display = awin->AllegroDisplay();
   EAGLE_ASSERT(display);
   char* allegro_text = al_get_clipboard_text(display);
   clipboard_string = allegro_text;
   al_free(allegro_text);
   return EagleClipboard::GetClipboardString();
}



