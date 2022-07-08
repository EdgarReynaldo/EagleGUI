



#include "Eagle/GraphicsHardware.hpp"



bool CompareModes(const MODE_INFO& m1 , const MODE_INFO& m2) {
   int area1 = m1.w*m1.h;
   int area2 = m2.w*m2.h;
   if (area1 < area2) {
      return false;
   } else if (m1.w < m2.w) {
      return false;
   } else if (m1.refresh < m2.refresh) {
      return false;
   }
   return true;
}



std::vector<ADAPTER_INFO> GraphicsHardware::GetAdapters(GRAPHICS_DRIVER driver) {
   std::vector<ADAPTER_INFO> info;
   switch(driver) {
   case GRAPHICS_OPENGL :
      info = adaptermap[GRAPHICS_OPENGL];
      break;
   case GRAPHICS_DIRECTX :
      info = adaptermap[GRAPHICS_DIRECTX];
      break;
   default :
      break;
   };
   return info;
}



int GraphicsHardware::NumAdapters(GRAPHICS_DRIVER driver) {
   std::map<GRAPHICS_DRIVER , std::vector<ADAPTER_INFO> >::iterator it = adaptermap.find(driver);
   if (it != adaptermap.end()) {
      return it->second.size();
   }
   return 0;
}



MONITOR_INFO GraphicsHardware::GetMonitor(GRAPHICS_DRIVER driver , int adapter_num) {
   MONITOR_INFO minfo;
   if (adapter_num < NumAdapters(driver)) {
      return GetAdapters(driver)[adapter_num].monitor_info;
   }
   return minfo;
}



std::vector<MODE_INFO> GraphicsHardware::GetModes(GRAPHICS_DRIVER driver , int adapter_num) {
   std::vector<MODE_INFO> modes;
   if (adapter_num < NumAdapters(driver)) {
      return GetAdapters(driver)[adapter_num].modes;
   }
   return modes;
}
