



#ifndef Territory_HPP
#define Territory_HPP




#include "Hexagon.hpp"

#include <unordered_set>


extern const unsigned int BADTID;



class Territory {
   int owner;
   
   std::unordered_set<HexTile*> tlist;/// Territory list
   std::unordered_set<HexTile*> blist;/// Border list
   
   friend class TerritoryList;
   friend class HexGrid;
   
public :

//   Territory(HexTile* tile);
   Territory() :
         owner(0),
         tlist(),
         blist()
   {}

   void AddTile(HexTile* tile);
   void RemoveTile(HexTile* tile);
   
   bool Contains(HexTile* tile);
   bool Borders(HexTile* tile);
};


class TerritoryList {
//   int owner;
   std::map<unsigned int , Territory*> tlist;/// <territory id , Territory>



   unsigned int GetNewTerritoryId();///< Return first free tid

   friend class HexGrid;
   friend class HexGame;
   friend class Player;
   
public :
      

//   TerritoryList();
   TerritoryList() :
         tlist()
   {}
      
   unsigned int AddTile(HexTile* tile);/// Returns territory id
   void RemoveTile(HexTile* tile);

   bool Contains(HexTile* tile);
   std::unordered_set<unsigned int> Borders(HexTile* tile);

   unsigned int GetTID(HexTile* tile);
};




#endif // Territory_HPP
