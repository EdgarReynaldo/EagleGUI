



#ifndef Face_HPP
#define Face_HPP

#include "Eagle.hpp"
#include "Eagle/GraphicsContext.hpp"

#include "GL/gl.h"


enum ROOM_FACE {
   ROOM_ABOVE = 0,
   ROOM_BELOW = 1,
   ROOM_NORTH = 2,
   ROOM_SOUTH = 3,
   ROOM_EAST  = 4,
   ROOM_WEST  = 5,
   NUM_ROOM_FACES = 6
};

enum FACE_TYPE {
   FACE_UPDOWN = 0,
   FACE_NORTHSOUTH = 1,
   FACE_EASTWEST = 2,
   NUM_FACE_TYPES = 3
};

FACE_TYPE GetFaceType(ROOM_FACE rf);

enum ROOM_DIRECTION {
   ROOM_POSITIVE = 0,
   ROOM_NEGATIVE = 1,
   ROOM_NUM_DIRECTIONS = 2
};

ROOM_DIRECTION GetRoomDirection(ROOM_FACE rf);

enum FACE_CORNER {
   FC_UPPERLEFT = 0,
   FC_LOWERLEFT = 1,
   FC_LOWERRIGHT = 2,
   FC_UPPERRIGHT = 3,
   NUM_FACE_CORNERS = 4
};

struct Vec3D;

class Room;

class Face {
   Room* rooms[ROOM_NUM_DIRECTIONS];
   Vec3* v[NUM_FACE_CORNERS];

   GLuint texidpos;/// Texture id for the POSITIVE direction
   GLuint texidneg;/// Texture id for the NEGATIVE direction

   int kweight;
   
   bool open;
   
public :
   Face();
   
   void Reset();

   void SetRoom(ROOM_DIRECTION dir , Room* room);
   void SetVertex(FACE_CORNER corner , Vec3* vtx);
   void SetWeight(int w);
   
   bool Open() {return open;}
   int Weight() {return kweight;}
   
   Room* GetRoom(ROOM_DIRECTION dir);
   
   void Display(GLuint tex);
   void SetOpen(bool face_open) {open = face_open;}
};



#endif // Face_HPP
