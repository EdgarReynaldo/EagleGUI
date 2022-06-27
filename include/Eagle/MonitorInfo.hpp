



#ifndef MonitorInfo_HPP
#define MonitorInfo_HPP


#include <vector>


struct MODE_INFO {
   int w,h,format,refresh;
   MODE_INFO() : w(0) , h(0) , format(0) , refresh(0) {}
};



struct MONITOR_INFO {
   int x,y,w,h;
   MONITOR_INFO() : x(0) , y(0) , w(0) , h(0) {}
};


enum GRAPHICS_DRIVER {
   GRAPHICS_NONE     = 0,
   GRAPHICS_SOFTWARE = 1,
   GRAPHICS_OPENGL   = 2,
   GRAPHICS_DIRECTX  = 4
};

struct ADAPTER_INFO {
   int adapter_num;
   MONITOR_INFO monitor_info;
   std::vector<MODE_INFO> modes;
   
   ADAPTER_INFO() : adapter_num(-1) , monitor_info() , modes() {}
   
};






class GraphicsAdapter {
protected:
   ADAPTER_INFO adapter_info;
public :
   GraphicsAdapter() : adapter_info() {}
   
   int Num() {return adapter_info.adapter_num;}
   MONITOR_INFO MonitorInfo() {return adapter_info.monitor_info;}
   std::vector<MODE_INFO> ModeInfo() {return adapter_info.modes;}
};

class GraphicsHardware {
protected :
   std::vector<GraphicsAdapter> adapters;

public :
   GraphicsHardware() : adapters() {}
   
   virtual bool EnumerateHardware(GRAPHICS_DRIVER driver)=0;
   
};

class GraphicsDriver {
protected :
   std::map<GRAPHICS_DRIVER , GraphicsHardware*> hardware;
public :
   virtual bool EnumerateDrivers()=0;
};

















class Allegro5GraphicsHardware : public GraphicsHardware {
   
protected :
   
public :
   Allegro5GraphicsHardware() : GraphicsHardware() {}
   
   virtual bool EnumerateHardware(GRAPHICS_DRIVER driver);
bool Allegro5GraphicsHardware::EnumerateHardware(GRAPHICS_DRIVER driver) {
   
   switch (driver) {
   case GRAPHICS_OPENGL :
      al_set_new_display_flag(ALLEGRO_OPENGL);
      break;
   case GRAPHICS_DIRECTX :
      al_set_new_display_flag(ALLEGRO_DIRECTX);
      break;
   default :
      throw EagleException(StringPrintF("Graphics driver #%i not supported.\n" , driver));
      break;
      
   };
   
   bool success = true;
   adapters.clear();
   adapters.resize(al_get_num_video_adapters());
   for (unsigned int i = 0 ; i < adapters.size() ; ++i) {
      ADAPTER_INFO info;
      info.adapter_num = i;
      MONITOR_INFO monitor_info;
      ALLEGRO_MONITOR_INFO al_info;
      al_get_monitor_info(i , &al_info);
      monitor_info.x = al_info.x1;
      monitor_info.y = al_info.y1;
      monitor_info.w = al_info.x2 - al_info.x1;
      monitor_info.h = al_info.y2 - al_info.y1;
      info.monitor_info = monitor_info;
      for (unsigned int i = 0 ; i < al_get_num_display_modes() ; ++i) {
         ALLEGRO_DISPLAY_MODE mode;
         al_get_display_mode(i , &mode);
         MODE_INFO minfo;
         minfo.w = mode.width;
         minfo.h = mode.height;
         minfo.format = mode.format;
         mnifo.refresh = mode.refresh_rate;
         info.modes.push_back(minfo);
      }
      adapters[i].adapter_info = info;
   }
   return success;
}
};


class Allegro5GraphicsDriver : public GraphicsDriver {
protected :
   
public :
   
   virtual bool EnumerateDrivers() override;
bool EnumerateDrivers() {
   
   hardware.clear();
   
   GRAPHICS_DRIVER drivers[2] = {GRAPHICS_OPENGL , GRAPHICS_DIRECTX};
   
   for (unsigned int i = 0 ; i < 2 ; ++i) {
      hardware[drivers[i]] = new Allegro5GraphicsHardware();
      hardware[drivers[i]]->EnumerateHardware(drivers[i]);
   }
}
};






#endif // MonitorInfo_HPP









