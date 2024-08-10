

#include "Eagle/UnitCube.hpp"

#include "Eagle/Image.hpp"






void UnitCube::CreateMesh() {
   
   /// A cube has 8 vertices
   EagleColor white(255,255,255,255);
   
   const unsigned int LBB = cube.AddVertex(VERTEX(Vec3(-0.5 , -0.5 ,  0.5) , white)); 
   const unsigned int LBF = cube.AddVertex(VERTEX(Vec3(-0.5 , -0.5 , -0.5) , white)); 
   const unsigned int LTF = cube.AddVertex(VERTEX(Vec3(-0.5 ,  0.5 , -0.5) , white)); 
   const unsigned int LTB = cube.AddVertex(VERTEX(Vec3(-0.5 ,  0.5 ,  0.5) , white)); 
   const unsigned int RBB = cube.AddVertex(VERTEX(Vec3( 0.5 , -0.5 ,  0.5) , white)); 
   const unsigned int RBF = cube.AddVertex(VERTEX(Vec3( 0.5 , -0.5 , -0.5) , white)); 
   const unsigned int RTF = cube.AddVertex(VERTEX(Vec3( 0.5 ,  0.5 , -0.5) , white)); 
   const unsigned int RTB = cube.AddVertex(VERTEX(Vec3( 0.5 ,  0.5 ,  0.5) , white)); 
   
   /// A cube has 12 edges

   /// left edges
   cube.AddEdge(LBB , LBF);
   cube.AddEdge(LBF , LTF);
   cube.AddEdge(LTF , LTB);
   cube.AddEdge(LTB , LBB);
   
   /// Right edges
   cube.AddEdge(RBB , RBF);
   cube.AddEdge(RBF , RTF);
   cube.AddEdge(RTF , RTB);
   cube.AddEdge(RTB , RBB);

   /// Cross edges
   cube.AddEdge(LBB , RBB);
   cube.AddEdge(LBF , RBF);
   cube.AddEdge(LTF , RTF);
   cube.AddEdge(LTB , RTB);
   
   /// A cube has six faces
   
   /// Left side
   cube.AddFlatQuadFace(LTB , LBB , LBF , LTF);
   
   /// Right side
   cube.AddFlatQuadFace(RTF , RBF , RBB , RTB);
   
   /// Front side
   cube.AddFlatQuadFace(LTF , LBF , RBF , RTF);

   /// Back side
   cube.AddFlatQuadFace(RTB , RBB , LBB , LTB);
   
   /// Top side
   cube.AddFlatQuadFace(LTB , LTF , RTF , RTB);
   
   /// Bottom side
   cube.AddFlatQuadFace(LBF , LBB , RBB , RBF);
}



UnitCube::UnitCube() :
      cube()
{
   CreateMesh();
}



void TexturedUnitCube::CreateTexturedMesh() {

   /// A cube has 8 vertices
   EagleColor white(255,255,255,255);

   const Vec3 lbb(-0.5 , -0.5 , -0.5);
   const Vec3 lbf(-0.5 , -0.5 ,  0.5);
   const Vec3 ltf(-0.5 ,  0.5 ,  0.5);
   const Vec3 ltb(-0.5 ,  0.5 , -0.5);
   const Vec3 rbb( 0.5 , -0.5 , -0.5);
   const Vec3 rbf( 0.5 , -0.5 ,  0.5);
   const Vec3 rtf( 0.5 ,  0.5 ,  0.5);
   const Vec3 rtb( 0.5 ,  0.5 , -0.5);

   /// L or R, T or B, B or F
   const unsigned int LBB1 = cubeoutside.AddVertex(VERTEX(lbb , white)); 
   const unsigned int LBF1 = cubeoutside.AddVertex(VERTEX(lbf , white)); 
   const unsigned int LTF1 = cubeoutside.AddVertex(VERTEX(ltf , white)); 
   const unsigned int LTB1 = cubeoutside.AddVertex(VERTEX(ltb , white)); 
   const unsigned int RBB1 = cubeoutside.AddVertex(VERTEX(rbb , white)); 
   const unsigned int RBF1 = cubeoutside.AddVertex(VERTEX(rbf , white)); 
   const unsigned int RTF1 = cubeoutside.AddVertex(VERTEX(rtf , white)); 
   const unsigned int RTB1 = cubeoutside.AddVertex(VERTEX(rtb , white)); 

   const unsigned int LBB2 = cubeinside.AddVertex(VERTEX(lbb , white)); 
   const unsigned int LBF2 = cubeinside.AddVertex(VERTEX(lbf , white)); 
   const unsigned int LTF2 = cubeinside.AddVertex(VERTEX(ltf , white)); 
   const unsigned int LTB2 = cubeinside.AddVertex(VERTEX(ltb , white)); 
   const unsigned int RBB2 = cubeinside.AddVertex(VERTEX(rbb , white)); 
   const unsigned int RBF2 = cubeinside.AddVertex(VERTEX(rbf , white)); 
   const unsigned int RTF2 = cubeinside.AddVertex(VERTEX(rtf , white)); 
   const unsigned int RTB2 = cubeinside.AddVertex(VERTEX(rtb , white)); 

   
   const Vec2 corners[NUM_FACE_CORNERS] = {
      Vec2(0.0 , 1.0),
      Vec2(0.0 , 0.0),
      Vec2(1.0 , 0.0),
      Vec2(1.0 , 1.0)
   };
   
   const TEXTEX texcoords[NUM_CUBE_FACES][NUM_FACE_CORNERS] = {
      TEXTEX(textures[CUBE_FACE_LEFT] , corners[FACETL]),
      TEXTEX(textures[CUBE_FACE_LEFT] , corners[FACEBL]),
      TEXTEX(textures[CUBE_FACE_LEFT] , corners[FACEBR]),
      TEXTEX(textures[CUBE_FACE_LEFT] , corners[FACETR]),

      TEXTEX(textures[CUBE_FACE_RIGHT] , corners[FACETL]),
      TEXTEX(textures[CUBE_FACE_RIGHT] , corners[FACEBL]),
      TEXTEX(textures[CUBE_FACE_RIGHT] , corners[FACEBR]),
      TEXTEX(textures[CUBE_FACE_RIGHT] , corners[FACETR]),

      TEXTEX(textures[CUBE_FACE_TOP] , corners[FACETL]),
      TEXTEX(textures[CUBE_FACE_TOP] , corners[FACEBL]),
      TEXTEX(textures[CUBE_FACE_TOP] , corners[FACEBR]),
      TEXTEX(textures[CUBE_FACE_TOP] , corners[FACETR]),

      TEXTEX(textures[CUBE_FACE_BOTTOM] , corners[FACETL]),
      TEXTEX(textures[CUBE_FACE_BOTTOM] , corners[FACEBL]),
      TEXTEX(textures[CUBE_FACE_BOTTOM] , corners[FACEBR]),
      TEXTEX(textures[CUBE_FACE_BOTTOM] , corners[FACETR]),

      TEXTEX(textures[CUBE_FACE_BACK] , corners[FACETL]),
      TEXTEX(textures[CUBE_FACE_BACK] , corners[FACEBL]),
      TEXTEX(textures[CUBE_FACE_BACK] , corners[FACEBR]),
      TEXTEX(textures[CUBE_FACE_BACK] , corners[FACETR]),

      TEXTEX(textures[CUBE_FACE_FRONT] , corners[FACETL]),
      TEXTEX(textures[CUBE_FACE_FRONT] , corners[FACEBL]),
      TEXTEX(textures[CUBE_FACE_FRONT] , corners[FACEBR]),
      TEXTEX(textures[CUBE_FACE_FRONT] , corners[FACETR]),
   };
   
   unsigned int texindex[NUM_CUBE_FACES][NUM_FACE_CORNERS];
   for (unsigned int f = 0 ; f < NUM_CUBE_FACES ; ++f) {
      for (unsigned int c = 0 ; c < NUM_FACE_CORNERS ; ++c) {
         texindex[f][c] = cubeoutside.AddTexVertex(texcoords[f][c]);
         cubeinside.AddTexVertex(texcoords[f][c]);
      }
   }
   
   /// A cube has 12 edges

   /// left edges
   cubeoutside.AddEdge(LBB1 , LBF1);
   cubeoutside.AddEdge(LBF1 , LTF1);
   cubeoutside.AddEdge(LTF1 , LTB1);
   cubeoutside.AddEdge(LTB1 , LBB1);
   
   /// Right edges
   cubeoutside.AddEdge(RBB1 , RBF1);
   cubeoutside.AddEdge(RBF1 , RTF1);
   cubeoutside.AddEdge(RTF1 , RTB1);
   cubeoutside.AddEdge(RTB1 , RBB1);

   /// Cross edges
   cubeoutside.AddEdge(LBB1 , RBB1);
   cubeoutside.AddEdge(LBF1 , RBF1);
   cubeoutside.AddEdge(LTF1 , RTF1);
   cubeoutside.AddEdge(LTB1 , RTB1);
   
   /// A cube has six faces
   
   /// Left side
   cubeoutside.AddTexturedFlatQuadFace(LTB1 , LBB1 , LBF1 , LTF1 , 
                                       texindex[CUBE_FACE_LEFT][FACETL] ,
                                       texindex[CUBE_FACE_LEFT][FACEBL] ,
                                       texindex[CUBE_FACE_LEFT][FACEBR] ,
                                       texindex[CUBE_FACE_LEFT][FACETR]);
   
   /// Right side
   cubeoutside.AddTexturedFlatQuadFace(RTF1 , RBF1 , RBB1 , RTB1 ,
                                       texindex[CUBE_FACE_RIGHT][FACETL] ,
                                       texindex[CUBE_FACE_RIGHT][FACEBL] ,
                                       texindex[CUBE_FACE_RIGHT][FACEBR] ,
                                       texindex[CUBE_FACE_RIGHT][FACETR]);
                                       
   /// Front side
   cubeoutside.AddTexturedFlatQuadFace(LTF1 , LBF1 , RBF1 , RTF1 ,
                                       texindex[CUBE_FACE_FRONT][FACETL] ,
                                       texindex[CUBE_FACE_FRONT][FACEBL] ,
                                       texindex[CUBE_FACE_FRONT][FACEBR] ,
                                       texindex[CUBE_FACE_FRONT][FACETR]);

   /// Back side
   cubeoutside.AddTexturedFlatQuadFace(RTB1 , RBB1 , LBB1 , LTB1 ,
                                       texindex[CUBE_FACE_BACK][FACETL] ,
                                       texindex[CUBE_FACE_BACK][FACEBL] ,
                                       texindex[CUBE_FACE_BACK][FACEBR] ,
                                       texindex[CUBE_FACE_BACK][FACETR]);
   
   /// Top side
///   cubeoutside.AddTexturedFlatQuadFace(RTF1 , RTB1 , LTB1 , LTF1 , 
   cubeoutside.AddTexturedFlatQuadFace(LTB1 , LTF1 , RTF1 , RTB1 ,
                                       texindex[CUBE_FACE_TOP][FACETL] ,
                                       texindex[CUBE_FACE_TOP][FACEBL] ,
                                       texindex[CUBE_FACE_TOP][FACEBR] ,
                                       texindex[CUBE_FACE_TOP][FACETR]);
   
   /// Bottom side
   cubeoutside.AddTexturedFlatQuadFace(LBF1 , LBB1 , RBB1 , RBF1 ,
                                       texindex[CUBE_FACE_BOTTOM][FACETL] ,
                                       texindex[CUBE_FACE_BOTTOM][FACEBL] ,
                                       texindex[CUBE_FACE_BOTTOM][FACEBR] ,
                                       texindex[CUBE_FACE_BOTTOM][FACETR]);

   /// Inside of the cube, for skyboxes
   /// A cube has six faces
   
   /// Left side
   cubeinside.AddTexturedFlatQuadFace(LTF2 , LBF2 , LBB2 , LTB2 , 
                                       texindex[CUBE_FACE_LEFT][FACETL] ,
                                       texindex[CUBE_FACE_LEFT][FACEBL] ,
                                       texindex[CUBE_FACE_LEFT][FACEBR] ,
                                       texindex[CUBE_FACE_LEFT][FACETR]);
   
   /// Right side
   cubeinside.AddTexturedFlatQuadFace(RTB2 , RBB2 , RBF2 , RTF2 ,
                                       texindex[CUBE_FACE_RIGHT][FACETL] ,
                                       texindex[CUBE_FACE_RIGHT][FACEBL] ,
                                       texindex[CUBE_FACE_RIGHT][FACEBR] ,
                                       texindex[CUBE_FACE_RIGHT][FACETR]);
                                       
   /// Front side
   cubeinside.AddTexturedFlatQuadFace(RTF2 , RBF2 , LBF2 , LTF2 ,
                                       texindex[CUBE_FACE_FRONT][FACETL] ,
                                       texindex[CUBE_FACE_FRONT][FACEBL] ,
                                       texindex[CUBE_FACE_FRONT][FACEBR] ,
                                       texindex[CUBE_FACE_FRONT][FACETR]);

   /// Back side
   cubeinside.AddTexturedFlatQuadFace(LTB2 , LBB2 , RBB2 , RTB2 ,
                                       texindex[CUBE_FACE_BACK][FACETL] ,
                                       texindex[CUBE_FACE_BACK][FACEBL] ,
                                       texindex[CUBE_FACE_BACK][FACEBR] ,
                                       texindex[CUBE_FACE_BACK][FACETR]);
   
   /// Top side
///   cubeinside.AddTexturedFlatQuadFace(RTB2 , RTF2 , LTF2 , LTB2 ,
   cubeinside.AddTexturedFlatQuadFace(LTF2 , LTB2 , RTB2 , RTF2 , 
                                       texindex[CUBE_FACE_TOP][FACETL] ,
                                       texindex[CUBE_FACE_TOP][FACEBL] ,
                                       texindex[CUBE_FACE_TOP][FACEBR] ,
                                       texindex[CUBE_FACE_TOP][FACETR]);
   
   /// Bottom side
///   cubeinside.AddTexturedFlatQuadFace(RBF2 , RBB2 , LBB2 , LBF2 ,
   cubeinside.AddTexturedFlatQuadFace(LBB2 , LBF2 , RBF2 , RBB2 ,
                                       texindex[CUBE_FACE_BOTTOM][FACETL] ,
                                       texindex[CUBE_FACE_BOTTOM][FACEBL] ,
                                       texindex[CUBE_FACE_BOTTOM][FACEBR] ,
                                       texindex[CUBE_FACE_BOTTOM][FACETR]);


}



void TexturedUnitCube::Clear() {
   cubeinside.ClearAll();
   cubeoutside.ClearAll();
}



TexturedUnitCube::TexturedUnitCube() :
      UnitCube(),
      cubeinside(),
      cubeoutside(),
      textures()
{
   for (unsigned int i = 0 ; i < NUM_CUBE_FACES ; ++i) {textures[i] = 0;}
}



void TexturedUnitCube::Texture(EagleImage* tex[NUM_CUBE_FACES]) {
   Clear();
   for (unsigned int i = 0 ; i < NUM_CUBE_FACES ; ++i) {
      textures[i] = tex[i];
   }
   CreateTexturedMesh();
}








