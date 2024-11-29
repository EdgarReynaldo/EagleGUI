
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
 * @file ObjModel.hpp
 * @brief Interface class to store material and object files for use with blender
 * 
 *
 */




#ifndef ObjModel_HPP
#define ObjModel_HPP


#include <string>
#include <vector>
#include <map>

#include "Eagle/Resources.hpp"
#include "Eagle/FileWork.hpp"
#include "Eagle/Vec3.hpp"
#include "Eagle/Mesh.hpp"



extern const std::string DEFAULT_MATERIAL;

class Material {
   
protected :
   std::string matname;/// Material name
   
public :
   
///   Material(std::string name);
   Material(std::string name) : 
         matname(name),
         acol(1,1,1),
         dcol(1,1,1),
         scol(1,1,1),
         alpha(1.0),
         shine(0.0),
         illumination_model((unsigned int)-1),
         diffuse_texmap_fp(""),
         tid()
   {}
   
   Vec3 acol;/// Ka, ambient light color
   Vec3 dcol;/// Kd, diffuse light color
   Vec3 scol;/// Ks, specular light color
   double alpha;/// d, or 1.0 - Tr, d is opacity, Tr is translucency, alpha value
   double shine;/// Ns, shininess
   unsigned int illumination_model;/// illum, illumination model
   std::string diffuse_texmap_fp;/// map_Kd, diffuse texture map
   TexID tid;
};

class MaterialFile {

protected :
   std::map<std::string , Material*> materialmap;

   TexLib texture_library;
   


   void ClearMaterials();
   void ClearTextures();
   void Clear();

   bool ProcessFile();

public :
   virtual std::vector<std::string> GetLines()=0;
   virtual std::string GetFileName()=0;
   
//   MaterialFile();
   MaterialFile() :
         materialmap(),
         texture_library()
   {}
   virtual ~MaterialFile() {Clear();}
   
   virtual bool LoadFile(std::string filepath)=0;
   //bool SaveFile(std::string filepath , const char* newline);

 //  Material* GetMaterial(std::string mat);
   Material* GetMaterial(std::string mat) {
      if (materialmap.find(mat) == materialmap.end()) {
         return (Material*)0;
      }
      return materialmap[mat];
   }
///   const std::map<std::string , Material*>& MatMap() {return materialmap;}
};



class ObjectFile {

protected :
   std::map<std::string , MaterialFile*> matfiles;
   
   std::map<std::string , Mesh*> objects;
   

   void ClearMaterialFiles();
   void ClearObjects();
   void Clear();

   bool ProcessPolygon(std::string line , Mesh* cmesh , Material* cmat);
   bool ProcessObjectFile();
   
public :
   virtual std::vector<std::string> GetLines()=0;
   virtual std::string GetFileName()=0;
   
   ObjectFile() :
         matfiles(),
         objects()
   {}
   virtual ~ObjectFile() {Clear();}
   
   virtual bool LoadFile(std::string filepath)=0;
   //bool SaveFile(std::string filepath , const char* newline);
};






#endif // ObjModel_HPP


