


#include "Eagle/ObjModel.hpp" 
#include "Eagle/FileWork.hpp"
#include <cstring>

#include "Eagle/Exception.hpp"
#include "Eagle/StringWork.hpp"
#include "Eagle/Lib.hpp"
#include "Eagle/System.hpp"
#include "Eagle/FileSystem.hpp"
#include "Eagle/Resources.hpp"
#include "Eagle/ResourceLib.hpp"

const std::string DEFAULT_MATERIAL = "DEFAULT_MATERIAL";



void MaterialFile::ClearMaterials() {
   for (std::map<std::string , Material*>::iterator it = materialmap.begin() ; it != materialmap.end() ; ++it) {
      if (it->second) {
         delete it->second;
      }
   }
   materialmap.clear();

}



void MaterialFile::ClearTextures() {
   texture_library.Clear();
}



void MaterialFile::Clear() {
   ClearMaterials();
   ClearTextures();
}



bool MaterialFile::ProcessFile() {
   Clear();

   materialmap[DEFAULT_MATERIAL] = new Material(DEFAULT_MATERIAL);

   Material* cmat = 0;/// Current material
   
   bool error = false;
   
   std::vector<std::string> lines = GetLines();
   
   for (unsigned int i = 0 ; i < lines.size() ; ++i) {
      const std::string& line = lines[i];
      if (!line.size())   {continue;}/// Empty lines
      if (line[0] == '#') {continue;}/// Comments
      
      if (strncmp(line.c_str() , "newmtl " , 7) == 0) {
         const char* name = line.c_str() + 7;
         std::map<std::string , Material*>::iterator it = materialmap.find(name);
         if (it != materialmap.end()) {
            cmat = it->second;
         }
         else {
            Material* newmat = new Material(name);
            cmat = newmat;
            materialmap[name] = newmat;
         }
      }
      else if (strncmp(line.c_str() , "map_Kd " , 7) == 0) {
         cmat->diffuse_texmap_fp = line.c_str() + 7;
         cmat->tid = texture_library.TID(cmat->diffuse_texmap_fp);
      }
      else if (strncmp(line.c_str() , "Ka " , 3) == 0) {
         if (!ScanVec3(line.c_str() + 3 , cmat->acol)) {
            error = true;
         }
      }
      else if (strncmp(line.c_str() , "Kd " , 3) == 0) {
         if (!ScanVec3(line.c_str() + 3 , cmat->dcol)) {
            error = true;
         }
      }
      else if (strncmp(line.c_str() , "Ks " , 3) == 0) {
         if (!ScanVec3(line.c_str() + 7 , cmat->scol)) {
            error = true;
         }
      }
      else if (strncmp(line.c_str() , "d " , 2) == 0) {
         double opacity = -1.0;
         if (1 == sscanf(line.c_str() + 2 , "%lf" , &opacity)) {
            cmat->alpha = opacity;
         }
         else {
            error = true;
         }
      }
      else if (strncmp(line.c_str() , "Tr " , 3) == 0) {
         double trans = 0.0;
         if (1 == sscanf(line.c_str() + 3 , "%lf" , &trans)) {
            cmat->alpha = 1.0 - trans;
         }
         else {
            error = true;
         }
      }
      else if (strncmp(line.c_str() , "illum " , 6) == 0) {
         if (1 != sscanf(line.c_str() + 6 , "%u" , &cmat->illumination_model)) {
            error = true;
         }
      }
      else {
         std::string name = GetFileName();
         printf("Unrecognized line in material file %s :\n%s\n" , name.c_str() , line.c_str());
         error = true;
      }
   }
   if (error) {
      return false;
   }
   return true;
}




void ObjectFile::ClearMaterialFiles() {
   std::map<std::string , MaterialFile*>::iterator it = matfiles.begin();
   while (it != matfiles.end()) {
      MaterialFile* mfile = it->second;
      if (mfile) {
         delete mfile;
      }
      ++it;
   }
   matfiles.clear();
}



void ObjectFile::ClearObjects() {
   std::map<std::string , Mesh*>::iterator it = objects.begin();
   while (it != objects.end()) {
      Mesh* mesh = it->second;
      if (mesh) {
         delete mesh;
      }
      ++it;
   }
   objects.clear();
}



void ObjectFile::Clear() {
   ClearMaterialFiles();
   ClearObjects();
}

struct ObjVertex {
public :
   ObjVertex(int vindex);
   ObjVertex(int vindex , int vnindex);
   ObjVertex(int vindex , int vtindex , int vnindex);
   int v;
   int vt;
   int vn;
};



ObjVertex::ObjVertex(int vindex) :
      v(vindex),
      vt(BAD_INDEX),
      vn(BAD_INDEX)
{}



ObjVertex::ObjVertex(int vindex , int vnindex) :
      v(vindex),
      vt(BAD_INDEX),
      vn(vnindex)
{}



ObjVertex::ObjVertex(int vindex , int vtindex , int vnindex) :
      v(vindex),
      vt(vtindex),
      vn(vnindex)
{}



bool ObjectFile::ProcessPolygon(std::string line , Mesh* cmesh , Material* cmat) {
   
///   EAGLE_ASSERT(cmat);
   EAGLE_ASSERT(cmesh);
   
   bool error = false;
   bool ret = true;
   
   std::vector<std::string> vertstrs;
   std::vector<ObjVertex> vertdata;
   int idx = 0;
   int ncharsread = 0;
   
   while (1) {
      std::string word;
      word.reserve(64);
      if (1 == sscanf(line.c_str() + idx , "%s%n" , &word[0] , &ncharsread)) {
         vertstrs.push_back(word);
         idx += ncharsread;
      }
      else {
         break;
      }
   }
   if (vertstrs.size() >= 3) {
      for (unsigned int i = 0 ; i < vertstrs.size() ; ++i) {
         error = false;
         std::vector<std::string> subwords = SplitByDelimiterString(vertstrs[i] , "/");
         int vindex  = BAD_INDEX;
         int vtindex = BAD_INDEX;
         int vnindex = BAD_INDEX;
         if (subwords.size() == 3) {
            /// We have v, vt, and vn and vt is optional
            /// v//vn
            /// v/vt/vn
            if (subwords.size() == 3) {
               if (3 != sscanf(vertstrs[i].c_str() , "%d/%d/%d" , &vindex , &vtindex , &vnindex)) {
                  error = true;
               }
            }
            else if (subwords[1][0] == '\0') {
               if (2 != sscanf(vertstrs[i].c_str() , "%d//%d" , &vindex , &vnindex)) {
                  error = true;
               }
            }
            else {
               error = true;
            }
         }
         else if (subwords.size() == 2) {
            /// We have v, and vt
            if (2 != sscanf(vertstrs[i].c_str() , "%d/%d" , &vindex , &vtindex)) {
               error = true;
            }
         }
         else if (subwords.size() == 1) {
            /// We just have v
            if (1 != sscanf(vertstrs[i].c_str() , "%d" , &vindex)) {
               error = true;
            }
         }
         else {
            error = true;
         }
         
         ret = ret && !error;
         
         if (!error) {
            if (vindex != BAD_INDEX && vindex > 0) {--vindex;}
            if (vtindex != BAD_INDEX && vtindex > 0) {--vtindex;}
            if (vnindex != BAD_INDEX && vnindex > 0) {--vnindex;}
            vertdata.push_back(ObjVertex(vindex , vtindex , vnindex));
         }
      }
   }
   else {
      error = true;
   }
   ret = ret && !error;
   
   
   
   if (ret) {
      ObjVertex vd0 = vertdata[0];
      for (unsigned int i = 1 ; i < vertdata.size() - 1 ; ++i) {
         ObjVertex vd1 = vertdata[i];
         ObjVertex vd2 = vertdata[i+1];
         if (cmat) {
            /// Uses a material
            if (cmat->tid != BAD_TEXID) {
               /// Textured face
               cmesh->AddTexturedTriFace(vd0.v , vd1.v , vd2.v , vd0.vt , vd1.vt , vd2.vt , vd0.vn , vd1.vn , vd2.vn);
            }
            else {
               /// Flat face
               cmesh->AddTriFace(vd0.v , vd1.v , vd2.v , vd0.vn , vd1.vn , vd2.vn);
            }
         }
      }
   }
   
   
   return ret;
}

   

bool ObjectFile::ProcessObjectFile() {
   
   MaterialFile* cmatlib = 0;/// Current material library
   Material* cmat = 0;/// Current material
   Mesh* cobjmesh = 0;/// Current object mesh
   
   
   bool error = false;
   int ecount = 0;
   
   EagleSystem* sys = Eagle::EagleLibrary::System("Any");
   EAGLE_ASSERT(sys);
   FileSystem* fsys = sys->GetFileSystem();
   ResourceLibrary* reslib = sys->GetResourceLibrary();
   EAGLE_ASSERT(fsys);
   EAGLE_ASSERT(reslib);
   
   std::vector<std::string> lines = GetLines();
   
   for (unsigned int i = 0 ; i < lines.size() ; ++i) {
      const std::string& line = lines[i];
      if (!line.size())   {continue;}/// Empty lines
      if (line[0] == '#') {continue;}/// Comments
      
      if (strncmp(line.c_str() , "mtllib " , 7) == 0) {
         std::string mfstr = line.c_str() + 7;
         std::shared_ptr<File> spfile(fsys->ReadFile(FilePath(mfstr)));
         std::map<std::string , MaterialFile*>::iterator it = matfiles.find(mfstr);
         if (it == matfiles.end()) {
            RESOURCEID rid = reslib->LoadFileResource(spfile.get());
            MaterialFile* mf = dynamic_cast<MaterialFile*>(reslib->LookupResourceByID(rid));
            if (!mf->LoadFile(mfstr)) {
               error = true;
               printf("Failed to load material file '%s'\n" , mfstr.c_str());
               delete mf;
            }
            else {
               matfiles[mfstr] = mf;
               cmatlib = mf;
            }
         }
      }
      else if (strncmp(line.c_str() , "usemtl " , 7) == 0) {
         std::string mtl = line.c_str() + 7;
         EAGLE_ASSERT(cmatlib);
         cmat = cmatlib->GetMaterial(mtl);
         EAGLE_ASSERT(cmat);
      }
      else if (strncmp(line.c_str() , "o " , 2) == 0) {
         /// New object
         std::string objname = line.c_str() + 2;
         Mesh* objmesh = new Mesh();
         objects[objname] = objmesh;
         cobjmesh = objmesh;
      }
      else if (strncmp(line.c_str() , "v " , 2) == 0) {
         /// vertice = v x y z
         Vec3 xyz;
         if (!ScanVec3(line.c_str() + 2 , xyz)) {
            error = true;
         }
         else {
            cobjmesh->AddVertex(xyz);
         }
      }
      else if (strncmp(line.c_str() , "vn " , 3) == 0) {
         /// vertice normal = vn x y z
         Vec3 nml;
         if (!ScanVec3(line.c_str() + 3 , nml)) {
            error = true;
         }
         else {
            cobjmesh->AddNormal(nml);
         }
      }
      else if (strncmp(line.c_str() , "vt " , 3) == 0) {
         /// texture vertice = vt u v
         Vec2 uv;
         if (2 != sscanf(line.c_str() + 3 , "%lf%lf" , &uv.u , &uv.v)) {
            error = true;
         }
         else {
            cobjmesh->AddTexVertex(TextureVertex((cmat?cmat->tid:BAD_TEXID) , uv));
         }
      }
      else if (strncmp(line.c_str() , "f " , 2) == 0) {
         /// Triangular face
         if (!ProcessPolygon(line.c_str() + 2 , cobjmesh , cmat?cmat:(cmatlib?cmatlib->GetMaterial(DEFAULT_MATERIAL):0))) {
            error = true;
         }
      }
      else if (strncmp(line.c_str() , "l " , 2) == 0) {
         /// Poly line = l idx1 idx2 idx3 ...
         std::string polystr = line.c_str() + 2;
         unsigned int idx = 0;
         unsigned int idx1 = (unsigned int)-1;
         unsigned int idx2 = (unsigned int)-1;
         int ncharsread = 0;
         while (2 == sscanf(line.c_str() + idx , "%u%n%u" , &idx1 , &ncharsread , &idx2)) {
            if (idx1 > 0) {idx1--;}
            if (idx2 > 0) {idx2--;}
            cobjmesh->AddEdge(idx1 , idx2);
            idx += (unsigned int)ncharsread;
         }
      }
      
      
      
      
      if (error) {
         std::string name = GetFileName();
         printf("Problem with line %d of file %s here :\n\t%s\n" , (int)i + 1 , name.c_str() , line.c_str());
         ++ecount;
         error = false;
      }
   }
   return ecount == 0;
}





