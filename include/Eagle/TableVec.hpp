
/**
 *
 *         _______       ___       ____      __       _______
 *        /\  ____\    /|   \     /  __\    /\ \     /\  ____\
 *        \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_
 *         \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\
 *          \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_
 *           \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *            \/______/|/_/  \/_/  \_\_____/    \/_____/ \/______/
 *
 *
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2021+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file TableVec.hpp
 * @brief A 2D vector with table capabilities, such as insertion and deletion of columns and rows and resizability
 */

#ifndef TableVec_HPP
#define TableVec_HPP



#include <vector>



template <class TYPE>
class TableVec2D {
   unsigned int ncols;
   unsigned int nrows;
   unsigned int nsize;
   std::vector<std::vector<TYPE> > table;

public :

   TableVec2D(unsigned int numcols , unsigned int numrows , TYPE t = TYPE()) :
         ncols(numcols),
         nrows(numrows),
         nsize(numcols*numrows),
         table(numrows , std::vector<TYPE>(numcols , t))
   {}

   void Resize(unsigned int numcols , unsigned int numrows , TYPE t = TYPE());
   
   void InsertColumn(std::vector<TYPE> colvec , unsigned int new_col_num);
   void InsertRow(std::vector<TYPE> rowvec , unsigned int new_row_num);
   
   void DeleteColumn(unsigned int old_col_num);
   void DeleteRow(unsigned int old_row_num);
   
   std::vector<TYPE>& operator[](unsigned int row) {
      return table[row];
   }
   
   unsigned int NRows() {return nrows;}
   unsigned int NCols() {return ncols;}
   unsigned int Size() {return nsize;}
};



template <class TYPE>
void TableVec2D<TYPE>::Resize(unsigned int numcols , unsigned int numrows , TYPE t) {
   int dcols = numcols - ncols;
   int drows = numrows - nrows;
   
   if (dcols == 0 && drows == 0) {return;}/// Nothing to do
   
   /// Empty table
   if (ncols == 0 && nrows == 0) {
      table.resize(drows , std::vector<TYPE>(dcols , t));
      ncols = dcols;
      nrows = drows;
      nsize = ncols*nrows;
      return;
   }
   
   /// Clear the table
   if (numcols == 0 || numrows == 0) {
      table.clear();
      ncols = nrows = nsize = 0;
      return;
   }
   
   /// Add/remove columns first
   for (unsigned int i = 0 ; i < table.size() ; ++i) {
      table[i].resize(numcols , t);
   }
   
   /// Add/remove rows second
   table.resize(numrows , std::vector<TYPE>(ncols , t));
   
   ncols = numcols;
   nrows = numrows;
   nsize = ncols*nrows;
}




template <class TYPE>
void TableVec2D<TYPE>::InsertColumn(std::vector<TYPE> colvec , unsigned int new_col_num) {
   unsigned int i = 0;
   for (unsigned int row = 0 ; row < table.size() ; ++row) {
      unsigned int col = 0;
      for (typename std::vector<TYPE>::iterator it = table[row].begin() ; it != table[row].end() ; ++it) {
         if (col == new_col_num) {
            table[row].insert(it , colvec[i]);
         }
         ++col;
      }
      ++i;
   }
}



template <class TYPE>
void TableVec2D<TYPE>::InsertRow(std::vector<TYPE> rowvec , unsigned int new_row_num) {
   unsigned int row = 0;
   for (typename std::vector<std::vector<TYPE> >::iterator it = table.begin() ; it != table.end() ; ++it) {
      if (row == new_row_num) {
         table.insert(it , rowvec);
         break;
      }
      ++row;
   }
}



template <class TYPE>
void TableVec2D<TYPE>::DeleteColumn(unsigned int old_col_num) {
   for (unsigned int row = 0 ; row < table.size() ; ++row) {
      unsigned int col = 0;
      for (typename std::vector<TYPE>::iterator it = table[row].begin() ; it != table[row].end() ; ++it) {
         if (col == old_col_num) {
            table[row].erase(it);
            break;
         }
         ++col;
      }
   }
}


template <class TYPE>
void TableVec2D<TYPE>::DeleteRow(unsigned int old_row_num) {
   unsigned int row = 0;
   for (typename std::vector<std::vector<TYPE> >::iterator it = table.begin() ; it != table.end() ; ++it) {
      if (row == old_row_num) {
         table.erase(it);
         break;
      }
      ++row;
   }
}



#endif // TableVec_HPP




