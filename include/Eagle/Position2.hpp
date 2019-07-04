



#ifndef Position2_HPP
#define Position2_HPP



template <class TYPE>
void Pos2T<TYPE>::DrawLineTo(EagleGraphicsContext* win , const Pos2T<TYPE>& p , EagleColor color) const {
   DrawLineTo(win , p , 1.0 , color);
}



template <class TYPE>
void Pos2T<TYPE>::DrawLineTo(EagleGraphicsContext* win , const Pos2T<TYPE>& p , double thickness , EagleColor color) const {
   win->DrawLine(tx , ty , p.X() , p.Y() , thickness , color);
}



#endif // Position2_HPP
