



#ifndef Allegro5Resources_HPP
#define Allegro5Resources_HPP

#include "Eagle/Resources.hpp"



class Allegro5TextResource : public TextResource {
protected :
   virtual bool LoadFromArgs(std::vector<std::string> args) override;

};
#endif // Allegro5Resources_HPP





