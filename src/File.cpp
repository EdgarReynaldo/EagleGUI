



#include "Eagle/File.hpp"
#include "Eagle/StringWork.hpp"
#include "Eagle/Exception.hpp"


/// -------------------------      FSInfo     -----------------------------


FSInfo::FSInfo(std::string path , FSMode mode , time_t creation_time , time_t modify_time , time_t access_time , unsigned long long size) :
      fpath(path),
      fmode(mode),
      tcreate(creation_time),
      tmodify(modify_time),
      taccess(access_time),
      fsize(size)
{}



/// ----------------------------     Folder     ---------------------------------------------



void Folder::RegisterFile(File* f) {
   files[f->Info().Path()] = f;
}



void Folder::RegisterSubFolder(Folder* f) {
   subfolders[f->Info().Path()] = f;
}



Folder::Folder(FSInfo info) :
      parent(0),
      finfo(info),
      fname(""),
      files(),
      subfolders()
{
   fname = GetFileName(info.Path());
}



/// ---------------------------       File       --------------------------------------------



File::File(FSInfo info) :
      parent(0),
      finfo(info),
      fname(""),
      fext(""),
      fmem(),
      file_open(false)
{
   fname = GetFileName(info.Path());
   fext = GetFileExt(fname);
}



bool File::ReadFileIntoMemory() {
   FILE* file = fopen(finfo.Path().c_str() , "rb");
   if (!file) {return false;}
   
   fmem = std::vector<unsigned char>(finfo.Size() , '0');
   
   if (finfo.Size() != fread(&fmem[0] , sizeof(unsigned char) , finfo.Size() , file)) {
      throw EagleException("File::ReadFileIntoMemory - failed to read full file size!\n");
   }
   
   fclose(file);
   return true;
}



bool File::WriteFileToDisk() {
   FILE* file = fopen(finfo.Path().c_str() , "wb");
   if (!file) {
      throw EagleException(StringPrintF("File::WriteFileToDisk - failed to open file '%s' for writing!\n" , finfo.Path().c_str()));
   }
   if (fmem.size() != fwrite(&fmem[0] , sizeof(unsigned char) , fmem.size() , file)) {
      throw EagleException("File::WriteFileToDisk - failed to write entire buffer to file.\n");
   }
   fclose(file);
   
   return true;
}



/// ----------------------------      FileSystem     ----------------------------------------



FileSystem::FSCREATOR FileSystem::CreateFunc = 0;
FileSystem::FSDESTRUCTOR FileSystem::DestroyFunc = FileSystem::DestroyFileSystem;
FileSystem* FileSystem::fsys = 0;



void FileSystem::DestroyFileSystem() {
   if (fsys) {
      delete fsys;
   }
   fsys = 0;
}




void FileSystem::RegisterFile(Folder* parent , File* file) {
   EAGLE_ASSERT(parent);
   EAGLE_ASSERT(file);
   parent->RegisterFile(file);
}



void FileSystem::RegisterSubFolder(Folder* parent , Folder* sub) {
   EAGLE_ASSERT(parent);
   EAGLE_ASSERT(sub);
   parent->RegisterSubFolder(sub);
}



FileSystem* FileSystem::Instance() {
   if (fsys) {
      return fsys;
   }
   else {
      if (!CreateFunc) {
         throw EagleException("FileSystem::Instance - no filesystem creation function registered!\n");
      }
      fsys = CreateFunc();
      if (fsys) {
         atexit(DestroyFileSystem);
      }
   }
   return 0;
}



std::string GetFileName(std::string path) {
   return FileSystem::Instance()->GetFileName(path);
}



std::string GetFileExt(std::string name) {
   return FileSystem::Instance()->GetFileExt(name);
}



FSInfo GetFileInfo(std::string path) {
   return FileSystem::Instance()->GetFileInfo(path);
}







