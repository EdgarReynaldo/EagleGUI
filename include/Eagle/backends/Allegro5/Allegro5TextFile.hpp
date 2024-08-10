
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
 *    Copyright 2009-2024+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file Allegro5TextFile.hpp
 * @brief Interface for text files. May be contained within archives given proper mounting.
 * Not sure if saving to an archive is supported or not...
 */

#ifndef Allegro5TextFile_HPP
#define Allegro5TextFile_HPP



#include "Eagle/TextFile.hpp"

class Allegro5TextFile : public TextFile {
   

public :
   Allegro5TextFile();

   virtual bool LoadFile(std::string txtfilepath);
   virtual bool SaveFile(std::string filepath , const char* newline);

};



#endif // Allegro5TextFile_HPP































