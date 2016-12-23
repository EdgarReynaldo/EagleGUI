

#include "Lol.hpp"



const char * lolbunnylogo = 
"(\\ /)______#_____####___#\n"
"(O.o)¯¯¯¯¯#¯¯¯¯¯#¯¯#¯¯¯#\n"
"(> <)____####__####___####\n"
;

const char* lolbunnysig = 
   "Megabytes are where I keep my Data.";

const char* lolbunny =
"(\\ /)\n"
"(O.o)\n"
"(> <)\n"
;
   
const char* fatbunny = 
"\\'\\/'/\n"
" \\\\//\n"
"  oo\n"
" ( * )\n"
"( v v )\n"
"  M M\n"
;

const char* cutebunny =
"() ()\n"
"('.')\n"
"(u u)O\n"
;



const char* lolbytes =
   "1010\n"
   "0101\n"
   "1010\n"
   "0101\n"
;

/**
const char* const font_paths[NUM_TEST_FONTS] = {
   "data/fonts/augusta.regular.ttf",
   "data/fonts/augusta.shadow.ttf",
   "data/fonts/baldur.regular.ttf",
   "data/fonts/baldur.shadow.ttf",
   "data/fonts/blackchancery.regular.ttf"
};
*/

const char* const font_paths[NUM_TEST_FONTS] = {
   "data/fonts/augusta.regular.ttf",
   "data/fonts/augusta.shadow.ttf",
   "data/fonts/baldur.regular.ttf",
   "data/fonts/baldur.shadow.ttf",
   "data/fonts/blackchancery.regular.ttf",
   "data/fonts/olde-english.regular.ttf",
   "data/fonts/pt-mono.bold.ttf",
   "data/fonts/pt-mono.regular.ttf",
   "data/fonts/ubuntu.mono-italic.ttf",
   "data/fonts/ubuntu.regular.ttf",
   "data/fonts/unispace bd it.ttf",
   "data/fonts/unispace bd.ttf",
   "data/fonts/unispace it.ttf",
   "data/fonts/unispace rg.ttf",
   "data/fonts/unispace.bold-italic.ttf",
   "data/fonts/unispace.bold.ttf",
   "data/fonts/unispace.italic.ttf"
};


/// ------------------------      TextImage      ---------------------------------



TextImage::TextImage(const char* text_name , const char* str , int width , int height) :
      text("Lol text drawer"),
      win(0),
      buffer(0),
      name(text_name),
      text_str(str),
      w(width),
      h(height)
{
}






TextImage::~TextImage() {
   win->FreeImage(buffer);
   buffer = 0;
}



void TextImage::Create(EagleGraphicsContext* window) {
   win = window;
   EAGLE_ASSERT(win && win->Valid());
   buffer = win->CreateImage(w,h);
   win->PushDrawingTarget(buffer);
   win->Clear(EagleColor(0,0,0,0));
   win->PopDrawingTarget();
}


void TextImage::Draw(EagleFont* font , bool scale_to_fit) {
   text.SetWidgetArea(0,0,buffer->W(),buffer->H());
   text.SetupText(HALIGN_LEFT , VALIGN_CENTER , 0 , 0 , 0 , text_str , font);
   text.ScaleToFit(scale_to_fit);
   win->PushDrawingTarget(buffer);
   win->Clear(EagleColor(0,0,0,0));
   text.Display(win , 0 , 0);
   win->PopDrawingTarget();
}








/// ------------------------     LolMaker     -------------------------------------





bool LolMaker::SaveImages() {
   bool success = true;
   for (std::map<const char* , EagleImage**>::iterator it = buf_map.begin() ; it != buf_map.end() ; ++it) {
      EagleImage* img = *(it->second);
      std::string path = StringPrintF("data/images/%s_%dx%d_%s" , it->first , img->W() , img->H() , scale_to_fit?"stretch":"unscaled");
      std::string ext = ".png";
      bool s = img->Save(path,ext);
      success = success && s;
      EagleInfo() << StringPrintF("%s bitmap %s%s" , s?"Saved":"Failed to save" , path.c_str() , ext.c_str()) << std::endl;
   }
   return success;
}



bool LolMaker::Init(EagleGraphicsContext* window) {
   const int NUM_TEXT_IMAGES = 6;
   TextImage* const text_images[NUM_TEXT_IMAGES] = {
      &bunny_logo,
      &bunny_sig,
      &lol_bunny,
      &fat_bunny,
      &cute_bunny,
      &lol_byte
   };
   for (int i = 0 ; i < NUM_TEST_FONTS ; ++i) {
      text_fonts[i] = window->LoadFont(font_paths[i] , -20 , LOAD_FONT_MONOCHROME);
      text_fonts[i]->SetName(font_paths[i]);
   }
   for(int i = 0 ; i < NUM_TEXT_IMAGES ; ++i) {
      TextImage* text_image = text_images[i];
      text_image->Create(window);
   }
   return true;
}



void LolMaker::Free(EagleGraphicsContext* window) {
/**
   const int NUM_TEXT_MAKERS = 6;
   TextImage* const text_images[NUM_TEXT_MAKERS] = {
      &bunny_logo,
      &bunny_sig,
      &lol_bunny,
      &fat_bunny,
      &cute_bunny,
      &lol_byte
   };
//*/
   for (int i = 0 ; i < NUM_TEST_FONTS ; ++i) {
      window->FreeFont(text_fonts[i]);
   }
}



void LolMaker::Display(EagleGraphicsContext* window) {
   const int NUM_TEXT_IMAGES = 6;
   TextImage* const text_images[NUM_TEXT_IMAGES] = {
      &bunny_logo,
      &bunny_sig,
      &lol_bunny,
      &fat_bunny,
      &cute_bunny,
      &lol_byte
   };
      
   int y = 0;
   EagleFont* font = text_fonts[font_num];
   for (int i = 0 ; i < NUM_TEXT_IMAGES ; ++i) {
      TextImage* text_image = text_images[i];
      if (text_image == &lol_byte) {
         text_image->Draw(font , true);
      }
      else {
         text_image->Draw(font , scale_to_fit);
      }
   }
   window->PushDrawingTarget(window->GetBackBuffer());
   window->Clear();
   for (int i = 0 ; i < NUM_TEXT_IMAGES ; ++i) {
      TextImage* text_image = text_images[i];
      window->Draw(text_image->buffer , 0 , y);
      y += text_image->buffer->H();
   }
   window->DrawTextString(font , font_paths[font_num] , 10 , window->Height() - font->Height() , EagleColor(255,255,255));
   window->PopDrawingTarget();
   window->FlipDisplay();
}



int LolMaker::HandleEvent(EagleEvent ev) {
   
   if (ev.type == EAGLE_EVENT_KEY_DOWN) {
      if (ev.keyboard.keycode == EAGLE_KEY_ESCAPE) {
         return STATE_QUIT;
      }
      else if (ev.keyboard.keycode == EAGLE_KEY_EQUALS) {
         font_num = (font_num + 1)%NUM_TEST_FONTS;
      }
      else if (ev.keyboard.keycode == EAGLE_KEY_MINUS) {
         font_num = (font_num + NUM_TEST_FONTS - 1)%NUM_TEST_FONTS;
      }
      else if (input_key_press(EAGLE_KEY_S)) {
         if (input_key_held(EAGLE_KEY_LCTRL)) {
            SaveImages();
         }
         else {
            scale_to_fit = !scale_to_fit;
         }
      }
      else if (ev.keyboard.keycode == EAGLE_KEY_D) {
         display_widget_areas = !display_widget_areas;
         WidgetBase::DisplayWidgetArea(display_widget_areas);
      }
   }
   return STATE_RUNNING;
}



int LolMaker::Update(double tsec) {
   (void)tsec;
   return STATE_RUNNING;
}

