



#ifndef Allegro5ObjModel_HPP
#define Allegro5ObjModel_HPP

#include "Eagle/ObjModel.hpp"
#include "Eagle/backends/Allegro5/Allegro5ObjModel.hpp"
#include "Eagle/backends/Allegro5/Allegro5Resources.hpp"

class Allegro5MaterialFile : public Allegro5TextResource , public MaterialFile {

protected :
   
public :
   std::vector<std::string> GetLines() override;

   std::string GetFileName() override;

   Allegro5MaterialFile() : Allegro5TextResource(), MaterialFile() {}

   bool LoadFile(std::string filepath) override;
};



class Allegro5ObjectFile : public Allegro5TextResource , public ObjectFile {

protected :

public :
   std::vector<std::string> GetLines() override;

   std::string GetFileName() override;

   Allegro5ObjectFile() : Allegro5TextResource(), ObjectFile() {}

   bool LoadFile(std::string filepath) override;
};



#endif // Allegro5ObjModel_HPP

