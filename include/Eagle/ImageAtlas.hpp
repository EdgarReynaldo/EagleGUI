
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
 *    Copyright 2009-2021+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file ImageAtlas.hpp
 * @brief An interface for working with image atlases
 *
 * TODO : Atlas WIP
 */

#ifndef ImageAtlas_HPP
#define ImageAtlas_HPP


#include <list>
#include <map>
#include <string>

#include "Eagle/Area.hpp"
#include "Eagle/Image.hpp"



class AtlasBody {
private :
   friend class Atlas;
   
   EagleGraphicsContext* win;
   EagleImage* master;
   std::list<Rectangle> freearea;
   std::list<Rectangle> usedarea;
   std::map<std::string , EagleImage*> imagemap;
   
   AtlasBody();
   
   
   void Free();
   bool Allocate(EagleGraphicsContext* window);

   void SortFreeArea();
   void RemoveArea(Rectangle area);///< This only works if the area is free, otherwise does nothing

public :
   ~AtlasBody() {Free();}
   
   EagleImage* GetImage(std::string name);
   
   
};



class Atlas {
   
   EagleGraphicsContext* win;
   std::vector<std::shared_ptr<AtlasBody> > bodies;
   
   virtual Rectangle FindFreeArea(unsigned int body_index , int w , int h);
public :
   
   Atlas(EagleGraphicsContext* window);
   virtual ~Atlas() {}
   
   unsigned int MakeNewBody();
   
   const std::map<std::string , EagleImage*>& GetImageMap(unsigned int atlas_num);

   EagleImage* ReserveArea(std::string name , EagleImage* copy_image , unsigned int body_index);
   EagleImage* ReserveArea(std::string name , int w , int h , unsigned int body_index);

};





#endif // ImageAtlas_HPP


















