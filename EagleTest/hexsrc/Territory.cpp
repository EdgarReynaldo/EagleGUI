



#include "Territory.hpp"

#include <set>

#include "Eagle.hpp"



/// ------------------------     Territory     -----------------------------



const unsigned int BADTID = (unsigned int)-1;



void Territory::CalcIncome() {
   income = 0;
   /// Simple
///   income = (int)blist.size() + 4*(int)tlist.size();
   /// Complex
   std::unordered_set<HexTile*>::iterator it = tlist.begin();
   while (it != tlist.end()) {
      HexTile* tile = *it;
      tile->CalcIncome();
      income += tile->TotalIncome();
      ++it;
   }
   it = blist.begin();
   while (it != blist.end()) {
      HexTile* tile = *it;
      tile->CalcIncome();
      income += tile->TotalIncome();
      ++it;
   }
}



void Territory::CollectIncome() {
   std::unordered_set<HexTile*>::iterator it = tlist.begin();
   while (it != tlist.end()) {
      HexTile* tile = *it;
      tile->CollectIncome();
      ++it;
   }
   it = blist.begin();
   while (it != blist.end()) {
      HexTile* tile = *it;
      tile->CollectIncome();
      ++it;
   }
}



void Territory::AddTile(HexTile* tile) {

   /// BAD tile
   if (!tile) {
      return;
   }
   
   /// Skip if already in territory
   if (Territory::Contains(tile)) {
      return;
   }

   /// Add the tile to our territory list
   tlist.insert(tile);

   /// Remove the tile from our border list
   if (blist.find(tile) != blist.end()) {
      blist.erase(tile);
   }
   
   /// Add the neighbors to our border if they are unowned
   std::vector<HexTile*>& nb = tile->neighbors;
   for (unsigned int i = 0 ; i < NUM_HEX_DIRECTIONS ; ++i) {
      HexTile* t = nb[i];
      if (!t) {continue;}
      if (t->owner == 0) {
         blist.insert(t);
      }
   }
}



void Territory::RemoveTile(HexTile* tile) {
   /// BAD tile
   if (!tile) {
      return;
   }
   
   if (!Territory::Contains(tile)) {
      return;/// We don't own this tile
   }
   
   /// Remove the tile from the territory
   std::unordered_set<HexTile*>::iterator it1 = tlist.find(tile);
   tlist.erase(it1);
   
   /// If we are bordered by any tiles owned by us, we are now part of the border
   std::vector<HexTile*>& nb = tile->neighbors;
   for (unsigned int i = 0 ; i < NUM_HEX_DIRECTIONS ; ++i) {
      HexTile* t = nb[i];
      if (!t) {continue;}
      if (t->owner == tile->owner) {
         blist.insert(tile);
         break;
      }
   }
}



void Territory::RemoveBorderTile(HexTile* tile) {
   std::unordered_set<HexTile*>::iterator bit = blist.find(tile);
   if (bit != blist.end()) {
      blist.erase(bit);
   }
}



bool Territory::Contains(HexTile* tile) {
   return tlist.find(tile) != tlist.end();
}



bool Territory::Borders(HexTile* tile) {
   return blist.find(tile) != blist.end();
}



/// ------------------------     TerritoryList     -----------------------------



void TerritoryList::CalcIncome() {
   income = 0;
   std::map<unsigned int , Territory*>::iterator it = tlist.begin();
   while (it != tlist.end()) {
      Territory* tty = it->second;
      tty->CalcIncome();
      income += tty->TotalIncome();
      ++it;
   }
   return;
}



void TerritoryList::CollectIncome() {
   for (std::map<unsigned int , Territory*>::iterator it = tlist.begin() ; it != tlist.end() ; ++it) {
      Territory* t = it->second;
      t->CollectIncome();
   }
}



unsigned int TerritoryList::GetNewTerritoryId() {
   unsigned int id = 0;
   while (tlist.find(id) != tlist.end()) {
      ++id;/// Already on list, skip id
   }
   return id;
}



unsigned int TerritoryList::AddTile(HexTile* tile) {
   if (!tile) {return BADTID;}
   
   const unsigned int TID = TerritoryList::Contains(tile);
   if (TID != BADTID) {
      /// Already on our territory list
      EAGLE_ASSERT(TID == BADTID);
      return BADTID;
   }

   /// First, find if this tile borders any tile in our territory
   std::map<unsigned int , Territory*>::iterator it = tlist.begin();
   std::unordered_set<unsigned int> tids;
   while (it != tlist.end()) {
      if (it->second->Borders(tile)) {
         tids.insert(it->first);
      }
      ++it;
   }
   
   /// Isolated territory
   if (tids.empty()) {
      Territory* newtty = new Territory;
      newtty->AddTile(tile);
      unsigned int newid = GetNewTerritoryId();
      tlist[newid] = newtty;
      return newid;
   }
   /// We only border one territory, simply add the tile to that territory
   else if (tids.size() == 1) {
      tlist[(*tids.begin())]->AddTile(tile);
      return *tids.begin();
   }

   /// We border more than one territory
   unsigned int tid = *tids.begin();
   Territory* base = tlist[tid];
   base->AddTile(tile);
   
   tids.erase(tids.begin());/// Ignore this territory, we have a pointer to base 

   while(tids.size() > 0) {
      /// For each adjoining territory
      Territory* t = tlist[*tids.begin()];
      t->blist.erase(tile);
      /// Manual merge of territories
      base->tlist.insert(t->tlist.begin() , t->tlist.end());
      base->blist.insert(t->blist.begin() , t->blist.end());
      t->tlist.clear();
      t->blist.clear();
      delete t;/// Delete extra territories
      tlist.erase(tlist.find(*tids.begin()));
      tids.erase(tids.begin());
   }

   return tid;
}
   


void TerritoryList::RemoveTile(HexTile* tile) {
   if (!tile) {return;}
   
   unsigned int TTYID = TerritoryList::Contains(tile);
   if (TTYID == BADTID) {
      /// None of our territories contain this tile
      EAGLE_ASSERT(TTYID != BADTID);
      return;
   }
   
   
   /// We have a tile, and one of our territories contains it
   Territory* home = tlist[TTYID];
   /// Removing this from the territory may create up to 3 unique territories
   /// We know who we border from HexTile::border_tty
   
   unsigned int nnb = tile->neighbor_tty.size(); 
   if (nnb < 1) {
      /// Isolated, delete
      delete home;
      tlist.erase(tlist.find(TTYID));
      return;
   }
   home->RemoveTile(tile);
   if (nnb < 2) {
      /// Neighbors only one other tile
      return;
   }
   
   /// 2 or more neighbors, look for adjacent ones
   std::set<HEXDIRECTION> ntty;/// Sort the directions quick
   ntty.insert(tile->neighbor_tty.begin() , tile->neighbor_tty.end());
   
   std::unordered_set<HEXDIRECTION> uniq;

   if (ntty.size() > 4) {
      /// Surrounded by 5 or 6 neighboring territories, so they have to be connected
      uniq.insert(*ntty.begin());/// Pick any
   }
   else {
      /// There are 2,3,or 4 neighbors, which may or may not be connected (true)
      /// There must be 2,3,or 4 territories which are not connected (false)
      /// Ring buffer (of neighbors) 1 0 1 1 0 1 = <5,6> , <2,3>

      std::vector<bool> nbs = {0,0,0,0,0,0};
      
      for (std::set<HEXDIRECTION>::iterator it = ntty.begin() ; it != ntty.end() ; ++it) {
         nbs[(int)*it] = 1;
      }
      
      int N = 0;
      while (nbs[N%6]) {++N;}/// Skip true
      while (!nbs[N%6]) {++N;}/// Skip false
      /// First true
      for (int n = N ; n < N + 6 ; ) {
         uniq.insert((HEXDIRECTION)(n%6));
         while (nbs[n%6]) {++n;}
         while (!nbs[n%6]) {++n;}
      }
   }

   tile->owner = 0;
   tile->CalcBorders();
   for (unsigned int n = 0 ; n < NUM_HEX_DIRECTIONS ; ++n) {
      HexTile* nb = tile->neighbors[n];
      if (nb) {
         nb->CalcBorders();
      }
   }
   
   /// for each unique direction, create a new territory
      /// for each remaining direction, see if it is connected, if so, erase it from the list
   
   /// Now uniq holds a set of unique neighbors and their direction from here
   std::unordered_set<HEXDIRECTION>::iterator uniqit = uniq.begin();
   /// They might still be connected, so we have to iterate the territory
   while (uniqit != uniq.end()) {
      /// Create a new territory
      unsigned int newtid = GetNewTerritoryId();
      Territory* newtty = new Territory;
      tlist[newtid] = newtty;
      
      newtty->tlist = GetAdjoiningTiles(tile->neighbors[*uniqit]);
      newtty->blist = GetBorders(newtty->tlist);
      
      /// Check if the other territories connect
      std::unordered_set<HEXDIRECTION>::iterator uniqit2 = uniqit;
      ++uniqit2;
      while (uniqit2 != uniq.end()) {
         HexTile* nb = tile->neighbors[*uniqit2];
         if (newtty->Contains(nb)) {
            uniqit2 = uniq.erase(uniqit2);/// Skip this one, it's already on a territory
         }
         else {
            ++uniqit2;
         }
      }
      ++uniqit;
   } 
   

   /// Remove the original territory from the map
   delete home;
   tlist.erase(tlist.find(TTYID));
   
   return;
}



void TerritoryList::RemoveBorderTile(HexTile* tile) {
   for (std::map<unsigned int , Territory*>::iterator it = tlist.begin() ; it != tlist.end() ; ++it) {
      Territory* t = it->second;
      t->RemoveBorderTile(tile);
   }
}



unsigned int TerritoryList::Contains(HexTile* tile) {
   std::map<unsigned int , Territory*>::iterator it= tlist.begin();
   while (it != tlist.end()) {
      if (it->second->Contains(tile)) {
         return it->first;
      }
      ++it;
   }
   return BADTID;
}



std::unordered_set<unsigned int> TerritoryList::Borders(HexTile* tile) {
   std::unordered_set<unsigned int> bset;
   std::map<unsigned int , Territory*>::iterator it = tlist.begin();
   while (it != tlist.end()) {
      if (it->second->Borders(tile)) {
         bset.insert(it->first);
      }
      ++it;
   }
   return bset;
}



unsigned int TerritoryList::GetTID(HexTile* tile) {
   for (std::map<unsigned int , Territory*>::iterator it = tlist.begin() ; it != tlist.end() ; ++it) {
      if (it->second->Contains(tile)) {return it->first;}
   }
   return BADTID;
}




