
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
 * @file 
 * @brief
 * 
 * 
 * 
 */

#ifndef SimpleTableLayout_HPP
#define SimpleTableLayout_HPP



#include "Eagle/Gui/Layout/Layout.hpp"



class SimpleTable : public LayoutBase {   
   unsigned int ncols;
   unsigned int nrows;
   std::vector<unsigned int> icolwidths;
   std::vector<unsigned int> irowheights;
   std::vector<float> fcolwidths;
   std::vector<float> frowheights;
   int padding;
   
   int RelColumnX(unsigned int column);
   int RelRowY(unsigned int row);
   
   void CalcCells();
   void CalcRelativeSizes();
   
   virtual void OnAreaChanged();///< Takes care of automatic resizing of contents



public :
   
   SimpleTable(std::string objname = "Nemo");
   
      
   virtual Rectangle RequestWidgetArea(int widget_slot , int newx , int newy , int newwidth , int newheight);
   
   void Resize(std::vector<unsigned int> colwidths , std::vector<unsigned int> rowheights);
   void Resize(std::vector<float> colwidths , std::vector<float> rowheights);
   
   void SetPadding(unsigned int pad);


   /// TODO : Insert / Delete  Row / Column
};



#endif // SimpleTableLayout_HPP



