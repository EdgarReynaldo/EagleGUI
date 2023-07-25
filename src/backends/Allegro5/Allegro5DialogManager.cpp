
/**
 *
 *         _______       ___       ____      __       _______
 *        /\  ____\    /|   \     /  __\    /\ \     /\  ____\
 *        \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_
 *         \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\
 *          \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_
 *           \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *            \/______/|/_/  \/_/  \_\____/     \/_____/ \/______/
 *
 *
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2023+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file Allegro5DialogManager.cpp
 * @brief Allegro 5 implementation of the dialog manager
 * 
 */



#include "Eagle/backends/Allegro5/Allegro5DialogManager.hpp"
#include "allegro5/allegro.h"
#include "allegro5/allegro_native_dialog.h"



std::vector<std::string> Allegro5DialogManager::ShowFileDialog(std::string title , FilePath initial_path , 
                                                  bool search_for_files , bool select_multiple , bool must_exist , bool save) {
   std::string path = initial_path.Path();
   int mode = 0;
   if (!search_for_files) {mode |= ALLEGRO_FILECHOOSER_FOLDER;}
   if (select_multiple && !save) {mode |= ALLEGRO_FILECHOOSER_MULTIPLE;}
   if (must_exist) {mode |= ALLEGRO_FILECHOOSER_FILE_MUST_EXIST;}
   if (save) {mode |= ALLEGRO_FILECHOOSER_SAVE;}
   mode |= ALLEGRO_FILECHOOSER_SHOW_HIDDEN;
   ALLEGRO_FILECHOOSER* fpick = al_create_native_file_dialog(path.c_str() , title.c_str() , "*" , mode);
   al_show_native_file_dialog(0 , fpick);
   std::vector<std::string> strs;
   for (int i = 0 ; i < al_get_native_file_dialog_count(fpick) ; ++i) {
      std::string s = al_get_native_file_dialog_path(fpick , i);
      strs.push_back(s);
   }
   al_destroy_native_file_dialog(fpick);
   fpick = 0;
   return strs;
}



void Allegro5DialogManager::ShowMessageBox(std::string message, std::string oktext) {
   al_show_native_message_box(0 , message.c_str() , "" , oktext.c_str() , "Ok" , ALLEGRO_MESSAGEBOX_OK_CANCEL);
}



int Allegro5DialogManager::ShowYesNo(std::string message , std::string yestext , std::string notext) {
   std::string s = "Select " + yestext + " to " + message + " or " + notext + " to cancel.";
   return al_show_native_message_box(0 , message.c_str() , s.c_str() , "" , "YES|NO" , ALLEGRO_MESSAGEBOX_YES_NO);
}

