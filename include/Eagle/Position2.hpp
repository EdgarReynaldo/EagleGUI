
/**
 *
 *     _______       ___       ____      __       _______
 *    /\  ____\    /|   \     /  __\    /\ \     /\  ____\
 *    \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_
 *     \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\
 *      \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_
 *       \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *        \/______/|/_/  \/_/  \_\_____/    \/_____/ \/______/
 *
 *
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2019+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */




template <class TYPE>
void Pos2T<TYPE>::DrawLineTo(EagleGraphicsContext* win , const Pos2T<TYPE>& p , EagleColor color) const {
   DrawLineTo(win , p , 1.0 , color);
}



template <class TYPE>
void Pos2T<TYPE>::DrawLineTo(EagleGraphicsContext* win , const Pos2T<TYPE>& p , double thickness , EagleColor color) const {
   win->DrawLine(tx , ty , p.X() , p.Y() , thickness , color);
}



