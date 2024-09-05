



#include "Face.hpp"


#include "Eagle/Vec3.hpp"



FACE_TYPE GetFaceType(ROOM_FACE rf) {
   return (FACE_TYPE)((int)rf/2);
}



ROOM_DIRECTION GetRoomDirection(ROOM_FACE rf) {
   return (ROOM_DIRECTION)((int)rf % 2);
}



/// --------------------     Face      ----------------------------



Face::Face() :
      rooms(),
      v(),
      texidpos((unsigned int)-1),
      texidneg((unsigned int)-1),
      kweight(0),
      open(false)
{
   Reset();
}



void Face::Reset() {
   memset(rooms , 0 , sizeof(Room*)*ROOM_NUM_DIRECTIONS);
   memset(v , 0 , sizeof(Vec3*)*NUM_FACE_CORNERS);
   texidpos = texidneg = (unsigned int)-1;
   kweight = 0;
   open = false;
}



void Face::SetRoom(ROOM_DIRECTION dir , Room* room) {
   rooms[dir] = room;
}



void Face::SetVertex(FACE_CORNER corner , Vec3* vtx) {
   v[corner] = vtx;
}



void Face::SetWeight(int w) {
   kweight = w;
}



void Face::Display(GLuint tex) {
   Vec3 v3;
   glBindTexture(0 , tex);
   glBegin(GL_TRIANGLE_FAN);
      glColor3ub(255,255,255);
      
      v3 = *v[FC_UPPERLEFT];
      glTexCoord2f(0.0f , 0.0f);
      glVertex3d(v3.x , v3.y , v3.z);

      v3 = *v[FC_LOWERLEFT];
      glTexCoord2f(0.0f , 1.0f);
      glVertex3d(v3.x , v3.y , v3.z);

      v3 = *v[FC_LOWERRIGHT];
      glTexCoord2f(1.0f , 1.0f);
      glVertex3d(v3.x , v3.y , v3.z);

      v3 = *v[FC_UPPERRIGHT];
      glTexCoord2f(0.0f , 1.0f);
      glVertex3d(v3.x , v3.y , v3.z);
      
   glEnd();
}
   


Room* Face::GetRoom(ROOM_DIRECTION dir) {
   return rooms[dir];
}
