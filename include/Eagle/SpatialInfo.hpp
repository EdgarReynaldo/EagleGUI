
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
 * @file SpatialInfo.hpp
 * @brief Interface for SpatialInfo class
 * 
 *
 */




#ifndef SpatialInfo_HPP
#define SpatialInfo_HPP



#include "Vec3.hpp"
#include "Orient.hpp"



class SpatialInfo {
   
public :
   Vec3 pos;
   Orient orient;
   
   SpatialInfo() : pos() , orient() {}
   SpatialInfo(Vec3 p , Orient o) : pos(p) , orient(o) {}
};



extern SpatialInfo START;


#endif // SpatialInfo_HPP
