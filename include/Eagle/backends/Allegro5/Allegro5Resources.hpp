



#ifndef Allegro5Resources_HPP
#define Allegro5Resources_HPP

#include "Eagle/Resources.hpp"



class Allegro5BinStream;

class Allegro5BinaryResource : public BinaryResource {

protected :
   Allegro5BinStream* a5binstream;

   bool LoadFromArgs(std::vector<std::string> args) override;

public :
   Allegro5BinaryResource();
   virtual ~Allegro5BinaryResource();

   BinStream* GetBinStream() override;
};


class Allegro5TextResource : public TextResource {
protected :
   virtual bool LoadFromArgs(std::vector<std::string> args) override;
public :
   Allegro5TextResource() : TextResource() {}
};
#endif // Allegro5Resources_HPP





