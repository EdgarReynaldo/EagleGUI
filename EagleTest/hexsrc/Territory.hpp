



#ifndef Territory_HPP
#define Territory_HPP




#include "Hexagon.hpp"

#include <unordered_set>


extern const unsigned int BADTID;



class Territory {
   int owner;
   
   std::unordered_set<HexTile*> tlist;/// Territory list
   std::unordered_set<HexTile*> blist;/// Border list
   
   int income;
   
   friend class TerritoryList;
   friend class HexGrid;
   
   void CalcIncome();
   
   void CollectIncome();
public :

//   Territory(HexTile* tile);
   Territory() :
         owner(0),
         tlist(),
         blist(),
         income(0)
   {}

   void AddTile(HexTile* tile);
   void RemoveTile(HexTile* tile);
   
   void RemoveBorderTile(HexTile* tile);
   
   bool Contains(HexTile* tile);
   bool Borders(HexTile* tile);
   
   int TotalIncome() {return income;}
};


class TerritoryList {
//   int owner;
   std::map<unsigned int , Territory*> tlist;/// <territory id , Territory>

   int income;



   unsigned int GetNewTerritoryId();///< Return first free tid

   friend class HexGrid;
   friend class HexGame;
   friend class Player;
   
   void CalcIncome();
   
   void CollectIncome();
   
   
public :
      

//   TerritoryList();
   TerritoryList() :
         tlist(),
         income(0)
   {}
      
   unsigned int AddTile(HexTile* tile);/// Returns territory id
   void RemoveTile(HexTile* tile);

   void RemoveBorderTile(HexTile* tile);
   
   unsigned int Contains(HexTile* tile);
   std::unordered_set<unsigned int> Borders(HexTile* tile);

   unsigned int GetTID(HexTile* tile);
   int TotalIncome() {return income;}
};




#endif // Territory_HPP
