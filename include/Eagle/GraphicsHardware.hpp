



#ifndef GraphicsHardware_HPP
#define GraphicsHardware_HPP


#include <vector>
#include <map>

struct MODE_INFO {
   int w,h,format,refresh;
   MODE_INFO() : w(-1) , h(-1) , format(-1) , refresh(-1) {}
};



struct MONITOR_INFO {
   int x,y,w,h;
   
   MONITOR_INFO() : x(-1) , y(-1) , w(-1) , h(-1) {}
};



struct ADAPTER_INFO {
   int adapter_num;
   MONITOR_INFO monitor_info;
   std::vector<MODE_INFO> modes;
   
   ADAPTER_INFO() : adapter_num(-1) , monitor_info() , modes() {}
   
};


enum GRAPHICS_DRIVER {
   GRAPHICS_NONE     = 0,
   GRAPHICS_SOFTWARE = 1,
   GRAPHICS_OPENGL   = 2,
   GRAPHICS_DIRECTX  = 3
};

bool CompareModes(const MODE_INFO& m1 , const MODE_INFO& m2);///< Returns false if m1 comes first. Descending sort by area, width, and refresh rate


/**! @class GraphicsHardware
 *   @brief Simple abstract base class to track graphics adapters and modes and sort them
 */

class GraphicsHardware {
protected :
   std::map<GRAPHICS_DRIVER , std::vector<ADAPTER_INFO> > adaptermap;

public :
   GraphicsHardware() : adaptermap() {}
   
   virtual void EnumerateHardware()=0;
   
   std::vector<ADAPTER_INFO> GetAdapters(GRAPHICS_DRIVER driver);

   int NumAdapters(GRAPHICS_DRIVER driver);

   MONITOR_INFO GetMonitor(GRAPHICS_DRIVER driver , int adapter_num);

   std::vector<MODE_INFO> GetModes(GRAPHICS_DRIVER driver , int adapter_num);



};






#endif // GraphicsHardware_HPP









