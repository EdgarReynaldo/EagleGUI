



#include "Eagle/Gui/Layout/SimpleTable.hpp"



int SimpleTable::RelColumnX(unsigned int column) {
   int c = 0;
   int x = 0;
   for (unsigned int i = 0 ; i < icolwidths.size() && c < (int)column; ++i) {
      x += icolwidths[i];
      ++c;
   }
   return x;
}



int SimpleTable::RelRowY(unsigned int row) {
   int r = 0;
   int y = 0;
   for (unsigned int i = 0 ; i < irowheights.size() && r < (int)row; ++i) {
      y += irowheights[i];
      ++r;
   }
   return y;
}



void SimpleTable::CalcCells() {
   float w = InnerArea().W();
   float h = InnerArea().H();
   icolwidths.resize(fcolwidths.size());
   irowheights.resize(frowheights.size());
   for (unsigned int i = 0 ; i < fcolwidths.size() ; ++i) {
      icolwidths[i] = (int)(w*fcolwidths[i]);
   }
   for (unsigned int i = 0 ; i < frowheights.size() ; ++i) {
      irowheights[i] = (int)(h*frowheights[i]);
   }
   RepositionAllChildren();
}



void SimpleTable::CalcRelativeSizes() {
   fcolwidths.resize(ncols);
   frowheights.resize(nrows);
   double w = InnerArea().W();
   double h = InnerArea().H();
   for (unsigned int i = 0 ; i < icolwidths.size() ; ++i) {
      fcolwidths[i] = (float)((double)icolwidths[i]/w);
   }
   for (unsigned int i = 0 ; i < irowheights.size() ; ++i) {
      frowheights[i] = (float)((double)irowheights[i]/h);
   }
}



SimpleTable::SimpleTable(std::string objname) :
      LayoutBase("SimpleTable" , objname),
      ncols(0),
      nrows(0),
      icolwidths(),
      irowheights(),
      padding(0)
{
   
}



Rectangle SimpleTable::RequestWidgetArea(int widget_slot , int newx , int newy , int newwidth , int newheight) {
   (void)newx;
   (void)newy;
   (void)newwidth;
   (void)newheight;
   int row = widget_slot / (int)ncols;
   int col = widget_slot % (int)ncols;
   int x = InnerArea().X();
   int y = InnerArea().Y();
//   int w = InnerArea().W();
//   int h = InnerArea().H();
   int cw = icolwidths[col];
   int ch = irowheights[row];
   int rx = x + RelColumnX(col);
   int ry = y + RelRowY(row);
   rx += padding;
   ry += padding;
   cw -= 2*padding;
   ch -= 2*padding;
   return Rectangle(rx,ry,cw,ch);
}



void SimpleTable::Resize(std::vector<unsigned int> colwidths , std::vector<unsigned int> rowheights) {
   /// What to do with overflow - remove widgets
   ClearWidgets();
   icolwidths = colwidths;
   irowheights = rowheights;
   ncols = icolwidths.size();
   nrows = irowheights.size();
   ReserveSlots((int)(ncols*nrows));

   CalcRelativeSizes();
}



void SimpleTable::Resize(std::vector<float> colwidths , std::vector<float> rowheights) {
   ClearWidgets();
   fcolwidths = colwidths;
   frowheights = rowheights;
   ncols = colwidths.size();
   nrows = rowheights.size();
   ReserveSlots((int)(ncols*nrows));
   CalcCells();
}





void SimpleTable::SetPadding(unsigned int pad) {
   padding = (int)pad;
   RepositionAllChildren();
}





