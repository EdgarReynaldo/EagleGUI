



#include "Territory.hpp"

#include <set>

#include "Eagle.hpp"



/// ------------------------     Territory     -----------------------------



const unsigned int BADTID = (unsigned int)-1;



void Territory::AddTile(HexTile* tile) {

   /// BAD tile
   if (!tile) {
      return;
   }
   
   /// Skip if already in territory
   if (Contains(tile)) {
      return;
   }

   /// Add the tile to our territory list
   tlist.insert(tile);

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
   
   if (!Contains(tile)) {
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



bool Territory::Contains(HexTile* tile) {
   return tlist.find(tile) != tlist.end();
}



bool Territory::Borders(HexTile* tile) {
   return blist.find(tile) != blist.end();
}



/// ------------------------     TerritoryList     -----------------------------



unsigned int TerritoryList::GetNewTerritoryId() {
   unsigned int id = 0;
   if (tlist.find(id) != tlist.end()) {
      /// Already on list, skip id
      ++id;
   }
   return id;
}



unsigned int TerritoryList::AddTile(HexTile* tile) {
   if (!tile) {return BADTID;}
   
   if (Contains(tile)) {
      /// Already on our territory list
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
   base->blist.erase(tile);/// Remove this tile from the border of the territory
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
      tids.erase(tids.begin());
   }
   return tid;
}
   


void TerritoryList::RemoveTile(HexTile* tile) {
   if (!tile) {return;}
   
   unsigned int TTYID = Contains(tile);
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
   if (nnb < 2) {
      /// Neighbors only one other tile
      home->RemoveTile(tile);
      return;
   }
   
   /// 2 or more neighbors, look for adjacent ones
   int i = 0;
   std::set<HEXDIRECTION> ntty;/// Sort the directions quick
   ntty.insert(tile->neighbor_tty.begin() , tile->neighbor_tty.end());

   std::set<HEXDIRECTION>::iterator it = ntty.begin();
   std::unordered_set<HEXDIRECTION> uniq;
   bool wasconnected = false;
   bool connected = false;
   while (it != ntty.end()) {
      std::set<HEXDIRECTION>::iterator it2 = it;
      ++it2;/// Get next
      ++i;
      if (i == (int)ntty.size()) {
         it2 = ntty.begin();/// Wrap around to beginning of set
      }
      int n1 = *it;
      int n2 = *it;
      int dn = n2 - n1;/// compare
      if (dn < 0) {dn += 6;}/// modulo
      wasconnected = connected;
      if (dn == 1 || dn == 5) {/// 1 away or 5 away (1 away)
         /// These are next to each other, ignore
         connected = true;
      }
      else {
         connected = false;
      }
      if (connected && !wasconnected) {
         uniq.insert(*it);
      }
      ++it;
   }

   /// Now uniq holds a set of unique neighbors and their direction from here
   std::unordered_set<HEXDIRECTION>::iterator uniqit = uniq.begin();
   /// They might still be connected, so we have to iterate the territory
   while (uniqit != uniq.end()) {
      unsigned int newtid = GetNewTerritoryId();
      Territory* newtty = new Territory;
      tlist[newtid] = newtty;
      
      newtty->tlist = GetAdjoiningTiles(tile->neighbors[*uniqit]);
      newtty->blist = GetBorders(newtty->tlist);
      
      /// Check the remaining tiles for already being included in this territory
      std::unordered_set<HEXDIRECTION>::iterator uniqit2 = uniqit;
      ++uniqit2;
      while (uniqit2 != uniq.end()) {
         if (newtty->tlist.find(tile->neighbors[*uniqit2]) != newtty->tlist.end()) {
            /// This one is already on a territory, skip it
            ++uniqit;
         }
         ++uniqit2;
      }
      if (uniqit == uniq.end()) {break;}
      ++uniqit;
   }

   /// Remove the original territory from the map
   delete home;
   tlist.erase(tlist.find(TTYID));
   
   return;
}



bool TerritoryList::Contains(HexTile* tile) {
   std::map<unsigned int , Territory*>::iterator it= tlist.begin();
   while (it != tlist.end()) {
      if (it->second->Contains(tile)) {
         return true;
      }
      ++it;
   }
   return false;
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




