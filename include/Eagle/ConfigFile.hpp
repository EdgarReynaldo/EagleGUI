
/**
 *
 *         _______       ___       ____      __       _______
 *        /\  ____\    /|   \     /  __\    /\ \     /\  ____\
 *        \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_
 *         \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\
 *          \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_
 *           \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *            \/______/|/_/  \/_/  \_\_____/    \/_____/ \/______/
 *
 *
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2019+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file ConfigFile.hpp
 * @brief A simple interface for working with textual config files
 * 
 * 
 * 
 */

#ifndef ConfigFile_HPP
#define ConfigFile_HPP



#include "Eagle/MemFile.hpp"



#include <vector>
#include <string>


/**! @class ConfigLine
 *   @brief The ConfigLine class is used to store a line from a config file
 */

class ConfigLine {
   bool comment;///< True if this is a comment
   bool spacer;///< True if this is a spacer
   std::string line;///< The value of the line
   std::string key;///< The value of the key
   std::string value;///< The value associated with the key
   
   void ParseLine();///< Member function to parse the stored line into key and value

public :
   ConfigLine();///< Empty constructor
   
   ConfigLine(std::string ln);///< Line constructor

   void SetLine(std::string ln);///< Sets the current line's value

   void SetKeyAndValue(std::string k , std::string v);///< Set the key and value for this line
   void SetKey(std::string k);///< Set the key
   void SetValue(std::string v);///< Set the value associated with this key

   std::string Key() const {return key;}///< Get the key string
   std::string& Value() {return value;}///< Get a reference to the value string
   const std::string& Value() const {return value;}///< Get a const reference to the value string
   std::string Line();///< Get a copy of the current line
   
   bool IsComment() {return comment;}///< Returns true if this is a comment line
   bool IsSpacer() {return spacer;}///< Returns true if this is a spacer line
   bool IsKeyValuePair() {return !comment && !spacer;}///< Returns true if there is a key and value associated with this line
};


/**! @class ConfigSection
 *   @brief A simple class to store a section of a config file
 */

class ConfigSection {

   std::vector<ConfigLine*> clines;///< Vector of config lines



   ///< Get a non-const iterator to the line associated with 'key'
   std::vector<ConfigLine*>::iterator GetConfigIterator(std::string key);

   ///< Get a const iterator to the line associated with 'key'
   std::vector<ConfigLine*>::const_iterator GetConfigIteratorConst(std::string key) const;


public :
   
   ConfigSection();///< Empty constructor
   
   ConfigLine* FindConfig(std::string key);///< Get a pointer to the config line corresponding to this key, may be null if empty
   const ConfigLine* FindConfigConst(std::string key) const ;///< const @ref FindConfig 

   ConfigLine* GetConfigByKey(std::string key);///< Like @ref FindConfig, but will create a new line if none with the key exists

   void SetKeyValuePair(std::string key , std::string value);///< Set the key and value
   void RemoveLineByKey(std::string key);///< Remove the line associated with key
   
   std::string& operator[](std::string key);///< Retrieves the string associated with key
   const std::string& operator[] (std::string key) const ;///< const @ref operator[]
   
   void AddSpacer();///< Add a spacer line
   void AddComment(std::string comment);///< Add a comment line
   void AddConfigLine(std::string line);///< Add a config line by string
   void AddConfigLine(std::string key , std::string value);///< Add a config line by key and value
   
   std::string GetConfigLine(int index) const ;///< Gets the string value of the config line at index
   unsigned int NConfigLines() const;///< Returns the number of config lines stored in this section
   
   std::vector<std::string> GetKeys() const;///< Get a list of keys in use
};


/**! @class ConfigFile
 *   @brief A simple class to represent and store and work with a config file
 */

class ConfigFile {

protected :
   /*! @typedef SECTIONMAP
    *  @brief Typedef to make it easier to work with a map of config sections keyed by string
    */
   typedef std::map<std::string , ConfigSection> SECTIONMAP;
   /*! @typedef SMIT
    *  @brief Typedef for working with @ref SECTIONMAP iterators
    */
   typedef SECTIONMAP::iterator SMIT;
   


   std::string contents;///< The entire contents of the file in a string
   SECTIONMAP sectionmap;///< The sections mapped by string

   void UpdateContents();///< Updates the @ref ConfigFile::contents string
   
public :
   ConfigFile();///< Empty constructor
   
   void Clear();///< Clear this ConfigFile's string @ref contents
      
   bool LoadFromFile(const char* path);///< Load config from a file, path may be relative or absolute
   
   bool SaveToFile(const char* path);///< Save config to a file
   
   void Absorb(const ConfigFile& c);///< Absorb another ConfigFile object, combining this and that
   
   ConfigSection* FindSection(std::string section);///< Lookup a section by name, may be null
   
   ConfigSection& operator[] (std::string section);///< Get a reference to a config section, if none exists, and empty one is created
   const ConfigSection& operator[] (std::string section) const;///< Like @ref operator[] but will throw if the section does not exist
};



#endif // ConfigFile_HPP



