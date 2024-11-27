

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
 * @file UnitCube.hpp
 * @brief A class to store a model of a unit cube and a class to store a textured unit cube
 */




#ifndef UnitCube_HPP
#define UnitCube_HPP


#include "Vec3.hpp"


#include "Mesh.hpp"



class UnitCube {
protected :
   Mesh cube;

   void CreateMesh();
   
public :
   UnitCube();
   virtual ~UnitCube() {}
   
   inline const Mesh& GetMesh() const {return cube;}
   
};


enum CUBE_FACE {
   CUBE_FACE_LEFT   = 0,
   CUBE_FACE_RIGHT  = 1,
   CUBE_FACE_TOP    = 2,
   CUBE_FACE_BOTTOM = 3,
   CUBE_FACE_BACK   = 4,
   CUBE_FACE_FRONT  = 5,
   NUM_CUBE_FACES   = 6
};

enum FACE_CORNER {
   FACETL = 0,
   FACEBL = 1,
   FACEBR = 2,
   FACETR = 3,
   NUM_FACE_CORNERS = 4
};



class EagleImage;

class TexturedUnitCube : public UnitCube {
protected:
   Mesh cubeinside;
   Mesh cubeoutside;

   EagleImage* textures[NUM_CUBE_FACES];

   void CreateTexturedMesh();

   void Clear();
public :
   
   TexturedUnitCube();
   
   void Texture(EagleImage* tex[NUM_CUBE_FACES]);
   
   const Mesh& Inside() {return cubeinside;}
   const Mesh& Outside() {return cubeoutside;}
};


#endif // UnitCube_HPP
