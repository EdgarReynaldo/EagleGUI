
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
 * @file Mesh.cpp
 * @brief Implementation of the mesh class
 * 
 * The various objects all store unchanging indexes and are easy to access for display using the graphics driver
 */


#include "Eagle/Mesh.hpp"
#include "Eagle/GraphicsContext.hpp"



const int BAD_INDEX = INT_MAX;

const unsigned int INDEX_NONE = (unsigned int)-1;

const unsigned int BAD_VERTEX = (unsigned int)INDEX_NONE;
const unsigned int BAD_TEXTURE = (unsigned int)INDEX_NONE;
const unsigned int BAD_NORMAL = (unsigned int)INDEX_NONE;



Edge::Edge(unsigned int from , unsigned int to) :
      vfrom(from),
      vto(to)
{}




TriFace::TriFace(unsigned int i1 , unsigned int i2 , unsigned int i3) :
      v1(i1),
      v2(i2),
      v3(i3),
      tv1(BAD_TEXTURE),
      tv2(BAD_TEXTURE),
      tv3(BAD_TEXTURE),
      n1(BAD_NORMAL),
      n2(BAD_NORMAL),
      n3(BAD_NORMAL)
{}



TriFace::TriFace(unsigned int i1  , unsigned int i2  , unsigned int i3  ,
                 unsigned int ti1 , unsigned int ti2 , unsigned int ti3 ,
                 unsigned int ni1 , unsigned int ni2 , unsigned int ni3) :
      v1(i1),
      v2(i2),
      v3(i3),
      tv1(ti1),
      tv2(ti2),
      tv3(ti3),
      n1(ni1),
      n2(ni2),
      n3(ni3)
{}



bool TriFace::Textured() const {
   return ((tv1 != BAD_TEXTURE) && (tv2 != BAD_TEXTURE) && (tv3 != BAD_TEXTURE));
}



bool TriFace::Normaled() const {
   return ((n1 != BAD_NORMAL) && (n2 != BAD_NORMAL) && (n3 != BAD_NORMAL));
}



unsigned int Mesh::UnsignedVIndex(int index) {
   if (index == BAD_INDEX) {return BAD_VERTEX;}
   if (index < 0) {
      index = (int)vertices.size() - index;
   }
   unsigned int uidx = index;
   EAGLE_ASSERT(uidx <= vertices.size());
   return uidx;
}



unsigned int Mesh::UnsignedTIndex(int index) {
   if (index == BAD_INDEX) {return BAD_TEXTURE;}
   if (index < 0) {
      index = (int)texverts.size() - index;
   }
   unsigned int uidx = index;
   EAGLE_ASSERT(uidx <= texverts.size());
   return uidx;
}



unsigned int Mesh::UnsignedNIndex(int index) {
   if (index == BAD_INDEX) {return BAD_NORMAL;}
   if (index < 0) {
      index = (int)normals.size() - index;
   }
   unsigned int uidx = index;
   EAGLE_ASSERT(uidx <= normals.size());
   return uidx;
}




Transform Mesh::SetupTransform(EagleGraphicsContext* win , const SpatialInfo& info , Vec3 scale) const {
   Transform old = win->GetTransformer()->GetViewTransform();
   Transform view = win->GetTransformer()->GetIdentityTransform();
   Transform  temp = win->GetTransformer()->GetIdentityTransform();
   
   const Vec3& c = info.pos;
   const Orient& o = info.orient;
   const Vec3& theta = o.Theta();

   
   double ux = UP.x;
   double uy = UP.y;
   double uz = UP.z;
   double rx = RIGHT.x;
   double ry = RIGHT.y;
   double rz = RIGHT.z;
   double fx = FORWARD.x;
   double fy = FORWARD.y;
   double fz = FORWARD.z;

   view.GetIdentityMatrix();
   
   temp.GetIdentityMatrix();
   temp.Rotate(theta.yaw , ux , uy , uz);
   temp.ApplyTransformation(&fx , &fy , &fz);
   temp.ApplyTransformation(&rx , &ry , &rz);
   
   view *= temp;
   
   temp.GetIdentityMatrix();
   temp.Rotate(theta.pitch , rx , ry , rz);
   temp.ApplyTransformation(&fx , &fy , &fz);
   temp.ApplyTransformation(&ux , &uy , &uz);
   
   view *= temp;
   
   temp.GetIdentityMatrix();
   temp.Rotate(theta.roll , fx , fy , fz);
   temp.ApplyTransformation(&rx , &ry , &rz);
   temp.ApplyTransformation(&ux , &uy , &uz);
   
   view *= temp;
   
   view.Scale(scale.x , scale.y , scale.z);
   view.Translate(c.x , c.y , c.z);
   
   view *= old;
   
   win->SetViewTransform(view);
   
   return old;
}


unsigned int Mesh::AddVertex(VERTEX v) {
   vertices.push_back(v);
   return vertices.size() - 1;
}



unsigned int Mesh::AddTexVertex(TEXTEX t) {
   texverts.push_back(t);
   return texverts.size() - 1;
}



unsigned int Mesh::AddNormal(NORMAL n) {
   normals.push_back(n);
   return normals.size() - 1;
}



unsigned int Mesh::AddEdge(int vfrom , int vto) {
   edges.push_back(EDGE(UnsignedVIndex(vfrom) , UnsignedVIndex(vto)));
   return edges.size() - 1;
}



unsigned int Mesh::AddTriFace(int v1 , int v2 , int v3 , int n1 , int n2 , int n3) {
   faces.push_back(TRIFACE(UnsignedVIndex(v1) , UnsignedVIndex(v2) , UnsignedVIndex(v3),
                           BAD_TEXTURE , BAD_TEXTURE , BAD_TEXTURE,
                           UnsignedNIndex(n1) , UnsignedNIndex(n2) , UnsignedNIndex(n3)));
   return faces.size() - 1;
}



/// Returns the index of the first of the four faces added by this function, also adds a new vertice
unsigned int Mesh::AddQuadFace(int vtl , int vbl , int vbr , int vtr , int ntl , int nbl , int nbr , int ntr) {
   
   /// A quad is made up of two or more triangles - we'll use 4 based on the center, so it creates a pyramid
   const unsigned int tl = UnsignedVIndex(vtl);
   const unsigned int bl = UnsignedVIndex(vbl);
   const unsigned int br = UnsignedVIndex(vbr);
   const unsigned int tr = UnsignedVIndex(vtr);

   const unsigned int tln = UnsignedNIndex(ntl);
   const unsigned int bln = UnsignedNIndex(nbl);
   const unsigned int brn = UnsignedNIndex(nbr);
   const unsigned int trn = UnsignedNIndex(ntr);

   VERTEX center(MidPoint(MidPoint(GetVertex(tl).pos , GetVertex(tr).pos) , MidPoint(GetVertex(bl).pos , GetVertex(br).pos)));
   const unsigned int c = AddVertex(center);

   if ((tln != BAD_NORMAL) && (bln != BAD_NORMAL) && (brn != BAD_NORMAL) && (trn != BAD_NORMAL)) {
      NORMAL ncenter(MidPoint(MidPoint(GetNormal(tln) , GetNormal(trn)) , MidPoint(GetNormal(bln) , GetNormal(brn))));
      const unsigned int nc = AddNormal(ncenter);

      const unsigned int f1 = AddTriFace(c,tl,bl,nc,ntl,nbl);
      const unsigned int f2 = AddTriFace(c,bl,br,nc,nbl,nbr);
      const unsigned int f3 = AddTriFace(c,br,tr,nc,nbr,ntr);
      const unsigned int f4 = AddTriFace(c,tr,tl,nc,ntr,ntl);
      
      (void)f2;
      (void)f3;
      (void)f4;

      return f1;
   }
   const unsigned int f1 = AddTriFace(c,tl,bl);
   const unsigned int f2 = AddTriFace(c,bl,br);
   const unsigned int f3 = AddTriFace(c,br,tr);
   const unsigned int f4 = AddTriFace(c,tr,tl);
   
   (void)f2;
   (void)f3;
   (void)f4;
   
   return f1;
}



/// Returns the index of the first of the two faces added by this function, also adds a new vertice
unsigned int Mesh::AddFlatQuadFace(int vtl , int vbl , int vbr , int vtr , int ntl , int nbl , int nbr , int ntr) {
   
   /// A quad is made up of two or more triangles - we'll use 4 based on the center, so it creates a pyramid
   const unsigned int tl = UnsignedVIndex(vtl);
   const unsigned int bl = UnsignedVIndex(vbl);
   const unsigned int br = UnsignedVIndex(vbr);
   const unsigned int tr = UnsignedVIndex(vtr);

   const unsigned int tln = UnsignedNIndex(ntl);
   const unsigned int bln = UnsignedNIndex(nbl);
   const unsigned int brn = UnsignedNIndex(nbr);
   const unsigned int trn = UnsignedNIndex(ntr);
   
   if ((tln != BAD_NORMAL) && (bln != BAD_NORMAL) && (brn != BAD_NORMAL) && (trn != BAD_NORMAL)) {
      const unsigned int f1 = AddTriFace(tl,bl,br,tln,bln,brn);
      const unsigned int f2 = AddTriFace(br,tr,tl,brn,trn,tln);

      (void)f2;

      return f1;
   }

   const unsigned int f1 = AddTriFace(tl,bl,br);
   const unsigned int f2 = AddTriFace(br,tr,tl);
   
   (void)f2;
   
   return f1;
}

unsigned int Mesh::AddTexturedTriFace(int v1  , int v2  , int v3 ,
                                      int tv1 , int tv2 , int tv3,
                                      int n1  , int n2  , int n3  ) {
   
   faces.push_back(TRIFACE(UnsignedVIndex(v1)  , UnsignedVIndex(v2)  , UnsignedVIndex(v3)  ,
                           UnsignedTIndex(tv1) , UnsignedTIndex(tv2) , UnsignedTIndex(tv3) ,
                           UnsignedNIndex(n1)  , UnsignedNIndex(n2)  , UnsignedNIndex(n3)));
   return faces.size() - 1;
}



unsigned int Mesh::AddTexturedQuadFace(int vtl , int vbl , int vbr , int vtr,
                                       int ttl , int tbl , int tbr , int ttr,
                                       int ntl , int nbl , int nbr , int ntr) {
   
   
   
   /// A quad is made up of two or more triangles - we'll use 4 based on the center, so it creates a pyramid
   const unsigned int tl = UnsignedVIndex(vtl);
   const unsigned int bl = UnsignedVIndex(vbl);
   const unsigned int br = UnsignedVIndex(vbr);
   const unsigned int tr = UnsignedVIndex(vtr);

   VERTEX center(MidPoint(MidPoint(GetVertex(tl).pos , GetVertex(tr).pos) , MidPoint(GetVertex(bl).pos , GetVertex(br).pos)));
   
   const unsigned int c = AddVertex(center);

   const unsigned int textl = UnsignedTIndex(ttl);
   const unsigned int texbl = UnsignedTIndex(tbl);
   const unsigned int texbr = UnsignedTIndex(tbr);
   const unsigned int textr = UnsignedTIndex(ttr);

   Vec2 texcenter(MidPoint(MidPoint(GetTexVertex(textl).uv , GetTexVertex(textr).uv) , MidPoint(GetTexVertex(texbl).uv , GetTexVertex(texbr).uv)));

   const unsigned int ctex = AddTexVertex(TEXTEX(GetTexVertex(textl).tid.pimg , texcenter));
   
   const unsigned int tln = UnsignedNIndex(ntl);
   const unsigned int bln = UnsignedNIndex(nbl);
   const unsigned int brn = UnsignedNIndex(nbr);
   const unsigned int trn = UnsignedNIndex(ntr);
   
   if ((tln != BAD_NORMAL) && (bln != BAD_NORMAL) && (brn != BAD_NORMAL) && (trn != BAD_NORMAL)) {
      NORMAL ncenter = MidPoint(MidPoint(GetNormal(tln) , GetNormal(trn)) , MidPoint(GetNormal(bln) , GetNormal(brn)));
      unsigned int cn = AddNormal(ncenter);

      const unsigned int f1 = AddTexturedTriFace(c,tl,bl , ctex , textl , texbl , cn , tln , bln);
      const unsigned int f2 = AddTexturedTriFace(c,bl,br , ctex , texbl , texbr , cn , bln , brn);
      const unsigned int f3 = AddTexturedTriFace(c,br,tr , ctex , texbr , textr , cn , brn , trn);
      const unsigned int f4 = AddTexturedTriFace(c,tr,tl , ctex , textr , textl , cn , trn , tln);
      
      (void)f2;
      (void)f3;
      (void)f4;
      
      return f1;
   }
   
   
   const unsigned int f1 = AddTexturedTriFace(c,tl,bl , ctex , textl , texbl);
   const unsigned int f2 = AddTexturedTriFace(c,bl,br , ctex , texbl , texbr);
   const unsigned int f3 = AddTexturedTriFace(c,br,tr , ctex , texbr , textr);
   const unsigned int f4 = AddTexturedTriFace(c,tr,tl , ctex , textr , textl);
   
   (void)f2;
   (void)f3;
   (void)f4;
   
   return f1;
}



unsigned int Mesh::AddTexturedFlatQuadFace(int vtl , int vbl , int vbr , int vtr,
                                           int ttl , int tbl , int tbr , int ttr,
                                           int ntl , int nbl , int nbr , int ntr) {
   
   const unsigned int tl = UnsignedVIndex(vtl);
   const unsigned int bl = UnsignedVIndex(vbl);
   const unsigned int br = UnsignedVIndex(vbr);
   const unsigned int tr = UnsignedVIndex(vtr);

   const unsigned int textl = UnsignedTIndex(ttl);
   const unsigned int texbl = UnsignedTIndex(tbl);
   const unsigned int texbr = UnsignedTIndex(tbr);
   const unsigned int textr = UnsignedTIndex(ttr);

   const unsigned int tln = UnsignedNIndex(ntl);
   const unsigned int bln = UnsignedNIndex(nbl);
   const unsigned int brn = UnsignedNIndex(nbr);
   const unsigned int trn = UnsignedNIndex(ntr);
   
   if ((tln != BAD_NORMAL) && (tln != BAD_NORMAL) && (tln != BAD_NORMAL) && (tln != BAD_NORMAL)) {
         
      const unsigned int f1 = AddTexturedTriFace(tl,bl,br , textl , texbl , texbr , tln , bln , brn);
      const unsigned int f2 = AddTexturedTriFace(br,tr,tl , texbr , textr , textl , brn , trn , tln);
      
      (void)f2;
      
      return f1;
   }
   
   const unsigned int f1 = AddTexturedTriFace(tl,bl,br , textl , texbl , texbr);
   const unsigned int f2 = AddTexturedTriFace(br,tr,tl , texbr , textr , textl);
   
   (void)f2;
   
   return f1;
}




void Mesh::ClearAll() {
   vertices.clear();
   texverts.clear();
   edges.clear();
   faces.clear();
}

