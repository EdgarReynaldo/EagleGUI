

#include "Eagle/Vertex.hpp"


Vertex::Vertex(Vec3 p) :
      pos(p),
      nml(),
//         edge_list(),
      col(255,255,255,255) 
{}
Vertex::Vertex(Vec3 p , EagleColor c) :
      pos(p),
      nml(),
//         edge_list(),
      col(c)
{}
Vertex::Vertex(Vec3 p , Vec3 n , EagleColor c) :
      pos(p),
      nml(n),
//         edge_list(),
      col(c)
{}




