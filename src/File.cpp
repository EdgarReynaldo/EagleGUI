



#include "Eagle/File.hpp"
#include "Eagle/StringWork.hpp"
#include "Eagle/Exception.hpp"


#include "Eagle/FileSystem.hpp"


/// -------------------------      FSInfo     -----------------------------


FSInfo::FSInfo(FilePath path , FSMode mode , time_t creation_time , time_t modify_time , time_t access_time , unsigned long long size) :
      fpath(path),
      fmode(mode),
      tcreate(creation_time),
      tmodify(modify_time),
      taccess(access_time),
      fsize(size)
{}



/// ----------------------------     Folder     ---------------------------------------------



void Folder::RegisterFile(File* f) {
   files[f->Info().Path()] = std::shared_ptr<File>(f);
}



void Folder::RegisterSubFolder(Folder* f) {
   subfolders[f->Info().Path()] = std::shared_ptr<Folder>(f);
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



/// ---------------------------       FilePath       --------------------------------------------



void FilePath::SetPathComponents(const std::vector<std::string>& paths) {
   EAGLE_ASSERT(paths.size() >= 2);
   drive = paths[0];
   folderpath.clear();
   for (int i = 1 ; i < (int)paths.size() - 1 ; ++i) {
      folderpath.push_back(paths[i]);
   }
   file = paths[(int)paths.size() - 1];
}



FilePath::FilePath(std::string path) :
      drive(""),
      folderpath(),
      file("")
{
   SetPathComponents(ExplodePath(path));
}



std::string FilePath::Drive() {
   return drive + PathSeparator();
}



std::string FilePath::Folder() {
   std::string f = FilePath::Drive();
   for (unsigned int i = 0 ; i < folderpath.size() ; ++i) {
      f += folderpath[i] + PathSeparator();
   }
   return f;
}



std::string FilePath::Path() {
   return Folder() + File();
}



std::string FilePath::File() {
   return file;
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



