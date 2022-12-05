



#include "Eagle/Platform.hpp"

#include "Eagle/backends/Allegro5/Allegro5GraphicsHardware.hpp"
#include "allegro5/allegro.h"
#include "Eagle/Indenter.hpp"
#include "Eagle/Logging.hpp"

#include <algorithm>



Allegro5GraphicsHardware::Allegro5GraphicsHardware() :
      GraphicsHardware()
{
   EnumerateHardware();
}



#ifdef EAGLE_WIN32
   #include "allegro5/allegro_direct3d.h"
#endif


void Allegro5GraphicsHardware::EnumerateHardware() {
   
   adaptermap.clear();
   
#ifdef EAGLE_WIN32
   const int NUM_DRIVERS = 2;
   GRAPHICS_DRIVER drivers[NUM_DRIVERS] = {GRAPHICS_OPENGL , GRAPHICS_DIRECTX};
   int alflags[NUM_DRIVERS] = {ALLEGRO_OPENGL , ALLEGRO_DIRECT3D};
   std::string driverstr[NUM_DRIVERS] = {"OpenGL" , "DirectX"};
#else
   const int NUM_DRIVERS = 1;
   GRAPHICS_DRIVER drivers[NUM_DRIVERS] = {GRAPHICS_OPENGL};
   int alflags[NUM_DRIVERS] = {ALLEGRO_OPENGL};
   std::string driverstr[NUM_DRIVERS] = {"OpenGL"};
#endif // #ifdef EAGLE_WIN32
      
   for (unsigned int j = 0 ; j < NUM_DRIVERS ; ++j) {
      al_set_new_display_flags(alflags[j]);

      std::vector<ADAPTER_INFO> adapters;
      adapters.resize(al_get_num_video_adapters());
      EagleInfo() << "Allegro5GraphicsHardware reports " << adapters.size() << " adapters with driver " << driverstr[j] << "." << std::endl;
      Indenter indent(1,3);
      for (unsigned int i = 0 ; i < adapters.size() ; ++i) {
         ADAPTER_INFO info;
         info.adapter_num = i;
         EagleInfo() << indent << "Checking monitor on adapter " << i << " : ";
         al_set_new_display_adapter(i);
         MONITOR_INFO monitor_info;
         ALLEGRO_MONITOR_INFO al_info;
         al_get_monitor_info(i , &al_info);
         monitor_info.x = al_info.x1;
         monitor_info.y = al_info.y1;
         monitor_info.w = al_info.x2 - al_info.x1;
         monitor_info.h = al_info.y2 - al_info.y1;
         info.monitor_info = monitor_info;
         EagleInfo() << "X Y W H " << monitor_info.x << " " << monitor_info.y << " " << monitor_info.w << " " << monitor_info.h << std::endl;
         for (int m = 0 ; m < al_get_num_display_modes() ; ++m) {
            ALLEGRO_DISPLAY_MODE mode;
            al_get_display_mode(m , &mode);
            MODE_INFO minfo;
            minfo.w = mode.width;
            minfo.h = mode.height;
            minfo.format = mode.format;
            minfo.refresh = mode.refresh_rate;
            info.modes.push_back(minfo);
         }
         std::sort(info.modes.begin() , info.modes.end() , CompareModes);
         adapters[i] = info;
      }
      adaptermap[drivers[j]] = adapters;
      
      al_set_new_display_flags(0);
      al_set_new_display_adapter(ALLEGRO_DEFAULT_DISPLAY_ADAPTER);
   }
}
