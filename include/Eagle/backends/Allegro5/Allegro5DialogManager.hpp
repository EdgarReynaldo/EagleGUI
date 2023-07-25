
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
 * @file Allegro5DialogManager.hpp
 * @brief Allegro 5 implementation of the dialog manager
 * 
 *       Has routines for getting file and folder names and displaying simple messages to the user.
 */


#ifndef Allegro5DialogManager_HPP
#define Allegro5DialogManager_HPP


//#include <vector>
//#include <string>
#include "Eagle/Dialog.hpp"


class Allegro5DialogManager : public DialogManager {
   
public :
   
   virtual std::vector<std::string> ShowFileDialog(std::string title , FilePath initial_path , 
                                                     bool search_for_files , bool select_multiple , bool must_exist , bool save);
   
   virtual void ShowMessageBox(std::string message, std::string oktext);
   
   virtual int ShowYesNo(std::string message , std::string yestext , std::string notext);
};


#endif // Allegro5DialogManager_HPP


