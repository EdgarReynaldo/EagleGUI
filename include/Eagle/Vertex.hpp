
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
 * @file Vertex.hpp
 * @brief A class to store vertices in
 * 
 *
 */


#ifndef Vertex_HPP
#define Vertex_HPP


#include "Vec2.hpp"
#include "Vec3.hpp"
#include "Eagle/Color.hpp"



class Vertex {

public :
   
   Vertex(Vec3 p);
   Vertex(Vec3 p , EagleColor c);
   Vertex(Vec3 p , Vec3 n , EagleColor c);

   
   Vec3 pos;
   Vec3 nml;
   Vec2 uv;
//   std::vector<unsigned int> edge_list;
   EagleColor col;
};



typedef Vertex VERTEX;



#endif // Vertex_HPP



