
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
 * @file TextFile.hpp
 * @brief Simple text file simplicator
 */

 #include <string>
 #include <vector>
 
class TextFile {
   
protected :
   std::string path;
   std::string contents;/// Full text including newlines
   std::vector<std::string> lines;/// Each line with newline removed
   
public :
//   TextFile();
   TextFile() :
         path(),
         contents(),
         lines()
   {}
   virtual ~TextFile() {}
   std::string Path() {return path;}
   std::string Contents() {return contents;}
   std::vector<std::string> Lines() {return lines;}
   
   virtual bool LoadFile(std::string txtfilepath)=0;
   virtual bool SaveFile(std::string filepath , const char* newline)=0;
};
