



#ifndef Map_HPP
#define Map_HPP


extern const int TILE_WIDTH;
extern const int TILE_HEIGHT;




class TileBase {

protected :
   
   EagleColor tile_color;
   EagleImage* tile_image;

   int type;
   
   int ntw;
   int ntt;
   
public :
   
   TileBase(EagleColor color , EagleImage* image , int type);
   TileBase(EagleColor color , EagleImage* image , int type) :
         tile_color(color),
         tile_image(image),
         tile_type(type),
         ntw(1),
         nth(1)
   {
      EAGLE_ASSERT(tile_image && tile_image->Valid());
      ntw = tile_image->W()/TILE_WIDTH;
      ntt = tile_image->H()/TILE_HEIGHT;
   }
   

   int CX(int xpos) {return xpos + (ntw*TILE_WIDTH)/2;}
   int CY(int ypos) {return ypos + (ntt*TILE_HEIGHT)/2;}
   
};

enum TILE_BASE_LAYER_TYPE {
   SNOWY = 0,
   GRASS = 1,
   DIRT = 2,
   FLOOR = 3,
   STONE = 4,
   BRICK = 5,
   HFENCE = 6,
   VFENCE = 7
};



map<int , Tile*> tile_type_map;


class TileMap {
   
   
};



#endif // Map_HPP

