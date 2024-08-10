



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



