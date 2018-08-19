


#include "Eagle5/backends/Allegro5backend.hpp"

#include "Game.hpp"


int main(int argc , char** argv) {

   SendOutputToFile("Squire_log.txt" , "Squire game log\n" , false);

   EagleLog() << "Hello test log?" << std::endl;


   Game g;
   
   g.Run();

   return 0;
}

