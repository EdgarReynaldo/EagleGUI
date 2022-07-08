



#ifndef Allegro5GraphicsHardware_HPP
#define Allegro5GraphicsHardware_HPP

#include "Eagle/GraphicsHardware.hpp"



class Allegro5GraphicsHardware : public GraphicsHardware {
   
public :
   Allegro5GraphicsHardware();
   
   virtual void EnumerateHardware() override;
};


#endif // Allegro5GraphicsHardware_HPP
