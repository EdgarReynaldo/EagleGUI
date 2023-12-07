


#include "Eagle.hpp"
#include "Eagle/backends/Allegro5Backend.hpp"
#include "physfs.h"


void PipeToEagleLog(const char* str) {
   EagleInfo() << str << std::endl;
}



int main(int argc , char** argv) {
   
   (void)argc;
   (void)argv;
   
   PHYSFS_init(argv[0]);
   
   al_set_config_value(al_get_system_config() , "trace" , "level" , "error");
   
   al_register_trace_handler(PipeToEagleLog);
   
   SendOutputToFile("ExResource.log" , "" , false);
   
   Allegro5System* sys = GetAllegro5System();
   
   if (sys->Initialize(EAGLE_FULL_SETUP) != EAGLE_FULL_SETUP) {
      EagleWarn() << "Some subsystems not initialized. Proceeding" << std::endl;
   }
   
   FileSystem* fsys = sys->GetFileSystem();
   
   int sw = 800;
   int sh = 600;
   
   EagleGraphicsContext* win = sys->CreateGraphicsContext("Main Window" , sw , sh , EAGLE_OPENGL | EAGLE_WINDOWED | EAGLE_RESIZABLE);
   
   EAGLE_ASSERT(win && win->Valid());
   
   
   
   /** GLOBAL RESOURCES */
   EagleFont* font = win->GetFont("Data/Fonts/Verdana.ttf" , -20);
   
   EAGLE_ASSERT(font && font->Valid());
   
   EagleImage* checkbox_down = win->LoadImageFromFile("Data/Images/Checkbox_Checked.png");
   EagleImage* checkbox_up = win->LoadImageFromFile("Data/Images/Checkbox_Unchecked.png");
   
   EAGLE_ASSERT(checkbox_down && checkbox_up);
   
   /**     GUI SETUP     */
   WidgetHandler gui(win , "GUI" , "Example GUI");
   
   gui.SetupBuffer(sw , sh , win);
   
   gui.SetWidgetArea(Rectangle(0 , 0 , sw , sh));
   

   SIMPLE_MENU_ITEM fmenu[] = {{SPRING_BTN , false , (EagleImage*)0 , (EagleImage*)0 , font , "&Load"  , input_key_press(EAGLE_KEY_L) , "L"},
                               {SPRING_BTN , false , (EagleImage*)0 , (EagleImage*)0 , font , "&Quit"  , input_key_held(EAGLE_KEY_ANY_CTRL) && input_key_press(EAGLE_KEY_Q) , "CTRL-Q"}};

   SIMPLE_MENU_ITEM omenu[] = {{TOGGLE_BTN , true , checkbox_up , checkbox_down , font , "Option &1" , input_key_press(EAGLE_KEY_1) , "1"},
                               {TOGGLE_BTN , true , checkbox_up , checkbox_down , font , "Option &2" , input_key_press(EAGLE_KEY_2) , "2"},
                               {TOGGLE_BTN , true , checkbox_up , checkbox_down , font , "Option &3" , input_key_press(EAGLE_KEY_3) , "3"}};

   SIMPLE_MENU_BAR_ITEM mbar[] = {{font , "&File"    , input_key_press(EAGLE_KEY_F)},
                                  {font , "&Options" , input_key_press(EAGLE_KEY_O)}};
   
   
   ClassicMenu options;
   ClassicMenu foptions;
   ClassicMenuBar menubar;
   
   options.SetItems(&omenu[0] , 3);
   foptions.SetItems(&fmenu[0] , 2);
   menubar.SetBarItems(&mbar[0] , 2);
   
   menubar.SetSubMenu(0 , &foptions);
   menubar.SetSubMenu(1 , &options);
   
   RelativeLayout rlayout;
   rlayout.Resize(3);
   
   rlayout.PlaceWidget(&menubar  , 0 , LayoutRectangle(0 , 0 , 1 , 0.05));
   rlayout.PlaceWidget(&foptions , 1 , LayoutRectangle(0 , 0.05 , 0.5 , 0.15));
   rlayout.PlaceWidget(&options  , 2 , LayoutRectangle(0.5 , 0.05 , 0.5 , 0.15));
   
   gui.SetRootLayout(&rlayout);
   
//   EagleInfo() << gui << std::endl << std::endl;
   
   /** RESOURCE_LIBRARY DEMO */

   ResourceLibrary* reslib = sys->GetResourceLibrary();
   reslib->SetWindow(win);
   RESOURCEID rid = BADRESOURCEID;
   ResourceBase* rb = 0;
   RESOURCE_TYPE rt = RT_UNKNOWN;

   TextResource* textres = 0;
   EagleImage* image = 0;
   EagleFont*  efont = 0;
   default_font_size = -64;
   ArchiveResource* arcres = 0;
   
   DialogManager* dman = sys->GetDialogManager();
   
   sys->GetSystemTimer()->Start();
   
   bool shown = false;
   
   bool quit = false;
   bool redraw = true;
   while (!quit) {
      if (redraw) {
         
         
         if (rb) {
            win->SetDrawingTarget(gui.BackgroundBitmap());
            win->Clear(EagleColor(0,0,0));
            switch (rt) {
            case RT_TEXTFILE :
               textres = dynamic_cast<TextResource*>(rb);
               win->DrawMultiLineTextString(font , textres->FileText() , 25 , 50 , EagleColor(255,255,255) , font->Height() , HALIGN_LEFT , VALIGN_TOP);
               break;
            case RT_IMAGE :
               image = dynamic_cast<EagleImage*>(rb);
               if (image) {
                  win->DrawImageFit(image , Rectangle(300,225,200,150));
                  win->DrawTextString(font , "Preview" , 300 , 200 , EagleColor(255,255,255) , HALIGN_LEFT , VALIGN_TOP);
               }
               break;
            case RT_FONT :
               efont = dynamic_cast<EagleFont*>(rb);
               if (efont) {
                  win->DrawTextString(efont , "Example Text" , 400 , 300 , EagleColor(255,255,255) , HALIGN_CENTER , VALIGN_CENTER);
               }
               break;
            case RT_ARCHIVE :
               if (!shown) {
                  arcres = dynamic_cast<ArchiveResource*>(rb);
                  std::shared_ptr<Folder> folder = arcres->GetContents();
                  folder.get()->PrintContents();
                  shown = true;
               }
               break;
            default:
               break;
            };
            gui.MakeAreaDirty(Rectangle(0 , 0 , gui.OuterArea().W() , gui.OuterArea().H()));
         }
         
         
         
         win->SetDrawingTarget(win->GetBackBuffer());
         win->Clear();
         gui.Display(win , 0 , 0);
         win->FlipDisplay();
         redraw = false;
      }
      while (!sys->UpToDate()) {
         EagleEvent e = sys->WaitForSystemEventAndUpdateState();
         
         if (e.type == EAGLE_EVENT_DISPLAY_RESIZE) {
            gui.SetWidgetArea(Rectangle(0 , 0 , e.window->Width() , e.window->Height()) , false);
            win->AcknowledgeResize();
         }
         
         if (e.type == EAGLE_EVENT_KEY_DOWN && e.keyboard.keycode == EAGLE_KEY_ESCAPE) {
            quit = true;
         }
         if (e.type == EAGLE_EVENT_DISPLAY_CLOSE) {
            quit = true;
         }
         if (e.type == EAGLE_EVENT_TIMER) {
            redraw = true;
         }
         else {
            if (e.type != EAGLE_EVENT_MOUSE_AXES) {
               EagleInfo() << EagleEventName(e.type) << std::endl;
            }
         }
         gui.HandleEvent(e);
         while (gui.HasMessages()) {
            WidgetMsg msg = gui.TakeNextMessage();
            EagleInfo() << Indenter() << msg << std::endl;
            if (msg == WidgetMsg(foptions.MItems()[0] , TOPIC_MENU , MENU_ITEM_ACTIVATED)) {
               /// file option 1 is load
               /** al_show_native_file_dialog blocks here*/
               std::vector<std::string> filenames = dman->ShowFileDialog("Pick a resource to load" , FilePath("") , true , false , true , false);
               if (filenames.size()) {
                  rt = reslib->GetResourceType(GetFileExt(filenames[0]));
                  if (rb) {
                     reslib->FreeResource(rid);
                     rid = BADRESOURCEID;
                     rb = 0;
                  }
                  FilePath fp = filenames[0];
                  EagleLog() << "Loading file " << fp.Path() << std::endl;
                  if (rt == RT_ARCHIVE) {
                     std::shared_ptr<ArchiveFile> afile = fsys->ReadArchive(fp);
                     Folder* fl = dynamic_cast<Folder*>(afile.get());
                     fl->PrintContentsAbsolute();
                  }
                  else if (rt != RT_UNKNOWN) {
                     std::shared_ptr<File> file = fsys->ReadFile(fp);
                     rid = reslib->LoadFileResource(file);
                     rb = reslib->LookupResourceByID(rid);
                  }
               }
               foptions.MItems()[0]->Deactivate();
               foptions.SetVisibleState(false);
               shown = false;
               redraw = true;
            }
            else if (msg == WidgetMsg(foptions.MItems()[1] , TOPIC_MENU , MENU_ITEM_ACTIVATED)) {
               /// file option 4 is quit
               quit = true;
            }
         }
      }
   }
   return 0;
}


