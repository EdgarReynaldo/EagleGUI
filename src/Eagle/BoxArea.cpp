
/**
 *
 *         _______       ___       ____      __       _______
 *        /\  ____\    /|   \     /  __\    /\ \     /\  ____\
 *        \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_
 *         \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\
 *          \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_
 *           \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *            \/______/|/_/  \/_/  \_\____/     \/_____/ \/______/
 *
 *
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2022+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file BoxArea.cpp
 * @brief Implementation file for basic box classes
 *
 */




#include "Eagle/BoxArea.hpp"

#include "Eagle/Indenter.hpp"




CELL_AREA GetCell(HCELL_AREA hcell , VCELL_AREA vcell) {
   return (CELL_AREA)(vcell*3 + hcell);
}



/// BOXAREA



BOXAREA::BOXAREA() :
   left(0),
   right(0),
   top(0),
   bottom(0)
{}



BOXAREA::BOXAREA(int side) :
      left(side),
      right(side),
      top(side),
      bottom(side)
{}



BOXAREA::BOXAREA(int hsize , int vsize) :
      left(hsize),
      right(hsize),
      top(vsize),
      bottom(vsize)
{}



BOXAREA::BOXAREA(unsigned int l , unsigned int r , unsigned int t , unsigned int b) :
      left(l),
      right(r),
      top(t),
      bottom(b)
{}



void BOXAREA::Set(unsigned int l , unsigned int r , unsigned int t , unsigned int b) {
   left = l;
   right = r;
   top = t;
   bottom = b;
}



std::ostream& BOXAREA::DescribeTo(std::ostream& os , Indenter indent) const {
   os << indent << "BOXAREA(" << left << "," << right << "," << top << "," << bottom << ")";
   return os;
}



std::ostream& operator<<(std::ostream& os , const BOXAREA& ba) {
   return ba.DescribeTo(os);
}

