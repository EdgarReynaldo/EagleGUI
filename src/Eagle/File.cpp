
/**
 *
 *     _______       ___       ____      __       _______
 *    /\  ____\    /|   \     /  __\    /\ \     /\  ____\
 *    \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_
 *     \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\
 *      \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_
 *       \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *        \/______/|/_/  \/_/  \_\_____/    \/_____/ \/______/
 *
 *
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2021+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */




#include "Eagle/File.hpp"
#include "Eagle/StringWork.hpp"
#include "Eagle/Exception.hpp"

#include "Eagle/FileSystem.hpp"
#include "Eagle/System.hpp"
#include "Eagle/Lib.hpp"



/// -------------------------      FSInfo     -----------------------------


FSInfo::FSInfo() :
      fpath(""),
      fexists(false),
      fmode(0),
      tcreate(),
      tmodify(),
      taccess(),
      fsize(0ULL)
{}



FSInfo::FSInfo(FilePath path) :
   fpath(path),
   fexists(false),
   fmode(0),
   tcreate(0),
   tmodify(0),
   taccess(0),
   fsize(0ULL)
{
   *this = Eagle::EagleLibrary::System("Any")->GetFileSystem()->GetFileInfo(fpath);
}



FSInfo::FSInfo(FilePath path , bool exists , FSMode mode ,
               time_t creation_time , time_t modify_time , time_t access_time , unsigned long long size) :
      fpath(path),
      fexists(exists),
      fmode(mode),
      tcreate(creation_time),
      tmodify(modify_time),
      taccess(access_time),
      fsize(size)
{}



/// ----------------------------     Folder     ---------------------------------------------



void Folder::RegisterFile(std::shared_ptr<File> f) {
   EAGLE_ASSERT(f);
   f->SetParent(this);
   files[f->Info().Path()] = f;
}



void Folder::RegisterArchiveFile(std::shared_ptr<ArchiveFile> af) {
   EAGLE_ASSERT(af);
   af->SetParent(this);
   archives[af->File::Info().Path()] = af;
}



void Folder::RegisterSubFolder(std::shared_ptr<Folder> fl) {
   EAGLE_ASSERT(fl);
   fl->parent = this;
   subfolders[fl->Info().Path()] = fl;
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



Folder::~Folder() {
   archives.clear();
   subfolders.clear();
   files.clear();
   parent = 0;
   fname = "";
}



void Folder::PrintContents(Indenter indent) {
   EagleLog() << indent << fname << std::endl;
   ++indent;
   FILEMAP::iterator fit = files.begin();
   while (fit != files.end()) {
      File* fl = fit->second.get();
      EagleLog() << indent << fl->Name() << std::endl;
      ++fit;
   }
   SUBFOLDERMAP::iterator sfit = subfolders.begin();
   while (sfit != subfolders.end()) {
      Folder* fl = sfit->second.get();
      fl->PrintContents(indent);
      ++sfit;
   }
   ARCHIVEMAP::iterator afit = archives.begin();
   while (afit != archives.end()) {
      ArchiveFile* afl = afit->second.get();
      afl->Folder::PrintContents(indent);
      ++afit;
   }
}



void Folder::PrintContentsAbsolute() {
   EagleLog() << Path() << " : " << std::endl;
   FILEMAP::iterator fit = files.begin();
   while (fit != files.end()) {
      File* fl = fit->second.get();
      EagleLog() << fl->Path() << std::endl;
      ++fit;
   }
   SUBFOLDERMAP::iterator sfit = subfolders.begin();
   while (sfit != subfolders.end()) {
      Folder* fl = sfit->second.get();
      fl->PrintContentsAbsolute();
      ++sfit;
   }
   ARCHIVEMAP::iterator afit = archives.begin();
   while (afit != archives.end()) {
      ArchiveFile* afl = afit->second.get();
      afl->Folder::PrintContentsAbsolute();
      ++afit;
   }
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
   SetPathComponents(GetAbsolutePath(path));
}



std::string FilePath::Drive() {
   return drive + "/";/// DONT USE NativePathSeparator();
}



std::string FilePath::Folder() {
   std::string f = FilePath::Drive();
   for (unsigned int i = 0 ; i < folderpath.size() ; ++i) {
      f += folderpath[i] + "/";/// DONT USE NativePathSeparator();
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
      fext("")
{
   fname = GetFileName(info.Path());
   fext = GetFileExt(fname);
}



void File::SetParent(Folder* parent_folder) {
   parent = parent_folder;
}




