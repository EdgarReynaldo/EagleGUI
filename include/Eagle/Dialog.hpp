
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
 * @file Dialog.hpp
 * @brief Drivers for dialog routines
 * 
 *
 */

#ifndef Dialog_HPP
#define Dialog_HPP



#include <vector>
#include <string>

#include "Eagle/File.hpp"


class DialogManager {
   
protected :
   
   
public :
   
   
   
   virtual std::vector<std::string> ShowFileDialog(std::string title , FilePath initial_path , 
                                                     bool search_for_files , bool select_multiple , bool must_exist , bool save)=0;
   
   virtual void ShowMessageBox(std::string message, std::string oktext)=0;
   
   virtual int ShowYesNo(std::string message , std::string yestext , std::string notext)=0;
};



#endif // Dialog_HPP





