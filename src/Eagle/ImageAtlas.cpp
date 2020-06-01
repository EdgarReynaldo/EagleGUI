




#include "Eagle/ImageAtlas.hpp"

#include "Eagle/GraphicsContext.hpp"

#include "Eagle/StringWork.hpp"


#include <algorithm>



AtlasBody::AtlasBody() : 
      win(0),
      master(0),
      freearea(),
      usedarea(),
      imagemap() 
{}



void AtlasBody::Free() {
   if (master && win) {
      for (std::map<std::string , EagleImage*>::iterator it = imagemap.begin() ; it != imagemap.end() ; ++it) {
         win->FreeImage(it->second);
      }
      imagemap.clear();
      win->FreeImage(master);
      master = 0;
      freearea.clear();
      usedarea.clear();
      win = 0;
   }
}



bool AtlasBody::Allocate(EagleGraphicsContext* window) {
   Free();
   EAGLE_ASSERT(window);
   win = window;
   if (!win) {return false;}
   Pos2I maxsize = win->GetMaxTextureSize();
   master = win->CreateImage(maxsize.X() , maxsize.Y() , VIDEO_IMAGE , "Atlas Master Image");
   if (master && master->Valid()) {
      win->PushDrawingTarget(master);
      win->Clear(EagleColor(0,0,0,0));
      win->PopDrawingTarget();
      freearea.push_back(Rectangle(0,0,maxsize.X() , maxsize.Y()));
   }
   return master;
}


static int CompRectangles(const Rectangle& r1 , const Rectangle& r2) {
   if (r1.Y() < r2.Y()) {
      return -1;
   }
   else if (r1.Y() > r2.Y()) {
      return 1;
   }
   if (r1.X() < r2.X()) {
      return -1;
   }
   else if (r1.X() > r2.X()) {
      return 1;
   }
   if (r1.Area() < r2.Area()) {
      return -1;
   }
   else if (r1.Area() > r2.Area()) {
      return 1;
   }
   return 0;
}



void AtlasBody::SortFreeArea() {
//   std::sort(freearea.begin() , freearea.end() , CompRectangles);
   freearea.sort(CompRectangles);
}



void AtlasBody::RemoveArea(Rectangle area) {///< This only works if the area is free, otherwise does nothing
   
   bool removed = false;
   std::list<Rectangle>::iterator it = freearea.begin();
   for (; it != freearea.end() ; ++it) {
      RECT_CONTAINS_TYPE type = DoRectanglesContain(area , *it);
      switch (type) {
      case CONTAINS_NONE :
         continue;
         break;
      case R2_CONTAINS_R1 :
         {
            std::list<Rectangle> left = SubtractRectangle(area , *it);
            usedarea.push_back(*it);
            it = freearea.erase(it);
            freearea.insert(freearea.end() , left.begin() , left.end());
            SortFreeArea();
            return;
         }
         break;
      case R1_CONTAINS_R2 :
         
         break;
      default :
         
         break;
      };
      
   }
}



EagleImage* AtlasBody::GetImage(std::string name) {
   std::map<std::string , EagleImage*>::iterator it = imagemap.find(name);
   if (it == imagemap.end()) {return (EagleImage*)0;}
   return it->second;
}



/// --------------------     Atlas     ------------------------------




Rectangle Atlas::FindFreeArea(unsigned int body_index , int w , int h) {
   std::list<Rectangle>& freearea = (bodies[body_index].get())->freearea;
   for (std::list<Rectangle>::iterator it = freearea.begin() ; it != freearea.end() ; ++it) {
      Rectangle r = *it;
      if (r.W() >= w && r.H() >= h) {
         return Rectangle(r.X() , r.Y() , w , h);
      }
   }
   return BADRECTANGLE;
}



Atlas::Atlas(EagleGraphicsContext* window) : 
      win(window),
      bodies()
{}



unsigned int Atlas::MakeNewBody() {
   bodies.push_back(std::shared_ptr<AtlasBody>(new AtlasBody()));
   AtlasBody* body = bodies.back().get();
   body->Allocate(win);
   return bodies.size() - 1;
}



const std::map<std::string , EagleImage*>& Atlas::GetImageMap(unsigned int atlas_num) {
   return bodies[atlas_num]->imagemap;
}


EagleImage* Atlas::ReserveArea(std::string name , EagleImage* copy_image , unsigned int body_index) {
   EAGLE_ASSERT(copy_image);
   EagleImage* sub = ReserveArea(name , copy_image->W() , copy_image->H() , body_index);
   if (sub && sub->Valid()) {
      win->PushDrawingTarget(sub);
      win->Clear(EagleColor(0,0,0,0));
      win->SetFullCopyBlender();
      win->Draw(copy_image , 0 , 0);
      win->RestoreLastBlendingState();
      win->PopDrawingTarget();
      
   }
   else if (sub && !sub->Valid()) {
      win->FreeImage(sub);
   }
   return sub;
}



EagleImage* Atlas::ReserveArea(std::string name , int w , int h , unsigned int body_index) {
   Rectangle rfree = FindFreeArea(body_index , w , h);
   if (rfree == BADRECTANGLE) {
      return (EagleImage*)0;
   }
   AtlasBody& body = *(bodies[body_index].get());
   std::map<std::string , EagleImage*>::iterator it = body.imagemap.find(name);
   if (it != body.imagemap.end()) {
      throw EagleException(StringPrintF("%s already in atlas. Cannnot replace!" , name.c_str()));
   }
   body.imagemap[name] = win->CreateSubImage(body.master , rfree.X() , rfree.Y() , w , h , name);
   EagleImage*& iref = body.imagemap[name];
   if (!iref || (iref && !iref->Valid())) {
      if (iref) {
         win->FreeImage(iref);
         return (EagleImage*)0;
      }
   }
   body.RemoveArea(rfree);
   return iref;
}



