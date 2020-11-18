



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
   
public :
   
   SimpleTable(std::string objname = "Nemo");
   
      
   virtual Rectangle RequestWidgetArea(int widget_slot , int newx , int newy , int newwidth , int newheight);
   
   void Resize(std::vector<unsigned int> colwidths , std::vector<unsigned int> rowheights);
   void Resize(std::vector<float> colwidths , std::vector<float> rowheights);
   
   void SetPadding(unsigned int pad);


   /// TODO : Insert / Delete  Row / Column
};



#endif // SimpleTableLayout_HPP



