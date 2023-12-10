


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
   
   ResourceLibrary* reslib = sys->GetResourceLibrary();
   reslib->SetWindow(win);

   RESOURCEID rid = BADRESOURCEID;
   ResourceBase* rb = 0;
   RESOURCE_TYPE rt = RT_UNKNOWN;

   ArchiveResource arcres(win);
   if (!arcres.LoadFromFile(FilePath("Data/Fonts/Fonts.7z"))) {
      EagleLog() << "Failed to load archive resource." << std::endl;
   }
   
//   std::shared_ptr<ArchiveFile> sparcfile = fsys->ReadArchive(FilePath("Data/Fonts/alex-brush.zip"));
//   sparcfile->PrintContentsAbsolute();
   





/**
   DialogManager* dman = sys->GetDialogManager();
   
   bool quit = false;
   
   while (!quit) {
      do {
         EagleEvent e = sys->WaitForSystemEventAndUpdateState();
         
         if (e.type == EAGLE_EVENT_KEY_DOWN && e.keyboard.keycode == EAGLE_KEY_ESCAPE) {
            quit = true;
         }
         if (e.type == EAGLE_EVENT_DISPLAY_CLOSE) {
            quit = true;
         }
         if (e.type == EAGLE_EVENT_TIMER) {
            std::vector<std::string> filenames = dman->ShowFileDialog("Pick an archive file to load: " , FilePath("") , true , false , true , false);
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
               else {
                  EagleLog() << "Not an archive file." << std::endl;
               }
            }
         }
      } while (!sys->UpToDate());
   }
*/

   return 0;
}
