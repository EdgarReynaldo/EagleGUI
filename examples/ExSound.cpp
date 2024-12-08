



#include "Eagle.hpp"
#include "Eagle/backends/Allegro5Backend.hpp"




class TickerTape {
public:
   std::string text;
   EagleFont* font;
   int lx,rx,cy;
   double tcurrent;
   double ttotal;

   TickerTape(std::string txt , EagleFont* fnt);


   void UpdateTime(double dt);
   void Display(EagleGraphicsContext* win);

};


int sw = 800;
int sh = 600;


int main(int argc , char** argv) {

   (void)argc;
   (void)argv;



   Allegro5System* sys = GetAllegro5System();
   EAGLE_ASSERT(sys);


   int state = 0;

   if (EAGLE_FULL_SETUP != (state = sys->Initialize(EAGLE_FULL_SETUP))) {
      EagleWarn() << "Failed to initialize some subsystem. Continuiing." << std::endl;
      EagleWarn() << "Failed states : " << PrintFailedEagleInitStates(EAGLE_FULL_SETUP , state) << std::endl;
   }


   EagleGraphicsContext* window = sys->CreateGraphicsContext("Soundboard" , sw , sh , EAGLE_OPENGL | EAGLE_WINDOWED , -1 , -1);
   EAGLE_ASSERT(window && window->Valid());
   window->Clear();
   window->FlipDisplay();


   EagleFont* font   = window->GetFont("Data/Fonts/Verdana.ttf" , -10 , 0 , VIDEO_IMAGE);
   EagleFont* font20 = window->GetFont("Data/Fonts/Verdana.ttf" , -20 , 0 , VIDEO_IMAGE);
   EagleFont* font40 = window->GetFont("Data/Fonts/Verdana.ttf" , -40 , 0 , VIDEO_IMAGE);
   EAGLE_ASSERT(font);
   EAGLE_ASSERT(font20);



   Allegro5SoundManager soundman;
   bool soundup = soundman.Initialize();
   if (!soundup) {
      EagleCritical() << "Could not initialize sound manager." << std::endl;
      return -1;
   }

   soundman.ReserveInstances(10);
   EAGLE_ASSERT(soundman.InstanceCount() == 10);


   EagleImage* checkbox_down = window->LoadImageFromFile("Data/Images/Checkbox_Checked.png");
   EagleImage* checkbox_up = window->LoadImageFromFile("Data/Images/Checkbox_Unchecked.png");

   EagleImage* play_button = window->LoadImageFromFile("Data/Images/PlayButton.png");
   EagleImage* pause_button = window->LoadImageFromFile("Data/Images/PauseButton.png");



   WidgetHandler gui(window , "WidgetHandler" , "Main GUI");
   gui.SetupBuffer(sw , sh , window);
   gui.SetWidgetArea(Rectangle(0 , 0 , sw , sh));


   SIMPLE_MENU_ITEM fmenu[] = {{SPRING_BTN , false , (EagleImage*)0 , (EagleImage*)0 , font20 , "Load SF&X Folder"  , input_key_press(EAGLE_KEY_X) , "X"},
                               {SPRING_BTN , false , (EagleImage*)0 , (EagleImage*)0 , font20 , "Load BG Music &Stream"  , input_key_press(EAGLE_KEY_S) , "S"},
                               {SPRING_BTN , false , (EagleImage*)0 , (EagleImage*)0 , font20 , "&Quit" , input_key_press(EAGLE_KEY_Q) , "Q"}};


   SIMPLE_MENU_ITEM omenu[] = {{TOGGLE_BTN , true , checkbox_down , checkbox_up , font20 , "Mute All Sound" , input_key_press(EAGLE_KEY_F1) , "F1"},
                               {TOGGLE_BTN , true , checkbox_down , checkbox_up , font20 , "Mute Music" , input_key_press(EAGLE_KEY_F2) , "F2"},
                               {TOGGLE_BTN , true , checkbox_down , checkbox_up , font20 , "Mute Effects" , input_key_press(EAGLE_KEY_F3) , "F3"}};

   SIMPLE_MENU_BAR_ITEM mbar[] = {{font40 , "&File"    , input_key_press(EAGLE_KEY_F)},
                                  {font40 , "&Options" , input_key_press(EAGLE_KEY_O)}};

   ClassicMenu options;
   ClassicMenu foptions;
   ClassicMenuBar menubar;

   options.SetItems(&omenu[0] , 3);
   foptions.SetItems(&fmenu[0] , 3);
   menubar.SetBarItems(&mbar[0] , 2);

   menubar.SetSubMenu(0 , &foptions);
   menubar.SetSubMenu(1 , &options);

   RelativeLayout rlayout;
   rlayout.Resize(4);

   rlayout.PlaceWidget(&menubar  , 0 , LayoutRectangle(0 , 0 , 1 , 0.05));
   rlayout.PlaceWidget(&foptions , 1 , LayoutRectangle(0 , 0.05 , 0.5 , 0.15));
   rlayout.PlaceWidget(&options  , 2 , LayoutRectangle(0.5 , 0.05 , 0.5 , 0.15));

   FlowLayout flow;
   flow.SetFlowDirection(FLOW_FAVOR_VERTICAL);
   flow.SetAlignment(HALIGN_LEFT , VALIGN_CENTER);
   flow.SetBoxSpacing(BOX_SPACE_EVEN);
   flow.SetAnchorPosition(FBOX_ANCHOR_NW);

   std::vector<std::shared_ptr<TextIconButton> > sample_btns;

   rlayout.PlaceWidget(&flow , 3 , LayoutRectangle(0 , 0.05 , 1.0 , 0.95));

   gui.SetRootLayout(&rlayout);

   int fnum = 0;
   std::vector<EagleSoundSample*> soundboard;
   std::vector<EagleSoundInstance*> instances;

   EagleSoundStream* stream = soundman.CreateSoundStream(FilePath("Data/Audio/GrandDarkWaltzAllegro.mp3"));
   soundman.ReadyBGStream(stream);

   soundman.SetMasterMixerPlaying(true);
   soundman.SetSampleMixerPlaying(true);
   soundman.SetBGStreamPlaying(true);

   bool masteron = true;
   bool musicon = true;
   bool sampleson = true;


   TickerTape tickertape("Grand Dark Waltz Allegro" , font40);

   bool quit = false;
   bool redraw = true;

   sys->GetSystemTimer()->Start();


   while (!quit) {
      if (redraw) {
         window->Clear();
         gui.Display(window , 0 , 0);
         tickertape.Display(window);
         window->FlipDisplay();
         redraw = false;
      }
      do {
         EagleEvent e = sys->WaitForSystemEventAndUpdateState();
         if (e.type == EAGLE_EVENT_TIMER) {
            tickertape.UpdateTime(e.timer.eagle_timer_source->SPT());
            gui.Update(e.timer.eagle_timer_source->SPT());
            redraw = true;
         }
         if (e.type == EAGLE_EVENT_DISPLAY_CLOSE) {
            quit = true;
         }
         if (e.type == EAGLE_EVENT_KEY_DOWN) {
            if (e.keyboard.keycode == EAGLE_KEY_ESCAPE) {
               quit = true;
            }
         }

         gui.HandleEvent(e);
         while (gui.HasMessages()) {
            WidgetMsg wmsg = gui.TakeNextMessage();
            for (int i = 0 ; i < fnum ; ++i) {
               if (wmsg.From() == sample_btns[i].get()) {
                  /// Sample link activated, play sample instance
                  soundman.SetInstancePlaying(instances[i] , !sample_btns[i].get()->Up());
               }
            }

            if (wmsg == WidgetMsg(options.MItems()[0] , TOPIC_MENU , MENU_ITEM_TOGGLED)) {
               masteron = !masteron;
               soundman.SetMasterMixerPlaying(masteron);
            }
            else if (wmsg == WidgetMsg(options.MItems()[1] , TOPIC_MENU , MENU_ITEM_TOGGLED)) {
               musicon = !musicon;
               soundman.SetBGStreamPlaying(musicon);
            }
            else if (wmsg == WidgetMsg(options.MItems()[2] , TOPIC_MENU , MENU_ITEM_TOGGLED)) {
               sampleson = !sampleson;
               soundman.SetSampleMixerPlaying(sampleson);
            }



            if (wmsg == WidgetMsg(foptions.MItems()[2] , TOPIC_MENU , MENU_ITEM_ACTIVATED)) {
               /// file option 2 is quit
               quit = true;
            }

///   virtual std::vector<std::string> ShowFileDialog(std::string title , FilePath initial_path ,
///                                                   bool search_for_files , bool select_multiple , bool must_exist , bool save)=0;
            else if (wmsg == WidgetMsg(foptions.MItems()[0] , TOPIC_MENU , MENU_ITEM_ACTIVATED)) {
               foptions.CloseMe();
               /// Load sfx folder
               sys->GetSystemTimer()->Stop();
               std::vector<std::string> foldername = sys->GetDialogManager()->ShowFileDialog("Select sfx folder" , FilePath() , false , false , true , false);
               if (foldername.size() == 1) {
                  std::shared_ptr<Folder> spfolder = sys->GetFileSystem()->ReadFolder(FilePath(foldername[0]) , false);
                  Folder* pfolder = spfolder.get();
                  if (pfolder) {
                     fnum = 0;
                     Folder::FILEMAP sfxmap = pfolder->Files();
                     if (sfxmap.empty()) {
                        sys->GetDialogManager()->ShowMessageBox("Directory is empty" , "OK");
                     }
                     else {
                        soundman.ClearSoundSamples();
                        soundboard.clear();
                        sample_btns.clear();
                        flow.ClearWidgets();
                        for (Folder::FILEMAP::iterator it = sfxmap.begin() ; it != sfxmap.end() ; ++it) {
                           EagleSoundSample* sample = soundman.CreateSoundSample(it->second.get()->Path());
                           if (!sample) {continue;}
                           soundboard.push_back(sample);
                           instances.push_back(soundman.CreateSoundInstance(sample));
                           soundman.SetInstancePlaying(instances.back() , false);
                           TextIconButton* pbtn = new TextIconButton(font20 , it->second.get()->Name() , "Sample button" , it->second.get()->Name());
                           pbtn->SetPreferredSize(sw/2 , 25);
                           pbtn->SetActionType(TOGGLE_BTN);
                           sample_btns.push_back(std::shared_ptr<TextIconButton>(pbtn));
                           flow.AddWidget(pbtn);
                           pbtn->SetImages(play_button , pause_button , play_button , pause_button);
                           ++fnum;
                        }
                     }
                     if (!fnum) {
                        sys->GetDialogManager()->ShowMessageBox("No sound files loaded." , "OK");
                     }
                  }
               }
               sys->GetSystemTimer()->Start();
            }
            else if (wmsg == WidgetMsg(foptions.MItems()[1] , TOPIC_MENU , MENU_ITEM_ACTIVATED)) {
               foptions.CloseMe();
               /// Load bg music
               sys->GetSystemTimer()->Stop();
               bool success = false;
               std::vector<std::string> files = sys->GetDialogManager()->ShowFileDialog("Select BG Music" , FilePath() , true , false , true , false);
               if (files.size() == 1) {
                  std::shared_ptr<File> file = sys->GetFileSystem()->ReadFile(files[0]);
                  File* pfile = file.get();
                  if (pfile) {
                     soundman.ClearBGStream();
                     stream = soundman.CreateSoundStream(pfile->Path());
                     soundman.ReadyBGStream(stream);
                     tickertape.text = pfile->Path();
                     tickertape.tcurrent = 0.0;
                     success = true;
                  }
               }
               if (!success) {
                  sys->GetDialogManager()->ShowMessageBox("Failed to load bg music stream" , "OK");
               }
               sys->GetSystemTimer()->Start();
            }
         }
      } while (!sys->UpToDate());
   }

   return 0;
}





TickerTape::TickerTape(std::string txt , EagleFont* fnt) :
      text(txt),
      font(fnt),
      lx(0),
      rx(sw),
      cy(sh),
      tcurrent(0.0),
      ttotal(8.0)
{
}



void TickerTape::UpdateTime(double dt) {
   tcurrent += dt;
   if (tcurrent < 0.0) {
      tcurrent += -(int)tcurrent + 1;
   }
   else if (tcurrent > ttotal) {
      tcurrent = tcurrent - floor(tcurrent);
   }
}



void TickerTape::Display(EagleGraphicsContext* win) {
   double pct = fmod(tcurrent,ttotal);
   double xpos = Interpolate(rx + font->Width(text)/2 , lx - font->Width(text)/2 , pct);
   double ypos = cy;
   win->DrawTextString(font , text , xpos , ypos , EagleColor(255,255,255) , HALIGN_CENTER , VALIGN_BOTTOM);
}






















