


#ifndef AnimationData_HPP
#define AnimationData_HPP

#include "Eagle/AnimationBase.hpp"


class EagleGraphicsContext;


class Animation : public AnimationBase {

protected :
   virtual void OnSetAnimationPercent()=0;

public :
   
   Animation();
   
   virtual void Draw(EagleGraphicsContext* win , int x , int y)=0;

};


#include "Eagle/Position.hpp"


typedef Pos2d(*POS2D_FUNC)(double);



class XYAnimation : public Animation {

protected :
   POS2D_FUNC posfunc;
   
   Pos2d pos;
   
   void OnSetAnimationPercent();

public :
   XYAnimation(POS2D_FUNC func);
   
   virtual void Draw(EagleGraphicsContext* win , int x , int y)=0;
};

#include <string>

class EagleFont;


typedef EagleColor(*COLOR_FUNC)(double);

class XYTextAnimation : public XYAnimation {

protected :
   EagleFont* text_font;
   std::string text_string;
   EagleColor text_color;
   
   COLOR_FUNC colfunc;

   virtual void OnSetAnimationPercent();

public :
   
   XYTextAnimation(EagleFont* f , std::string s , COLOR_FUNC cfunc , POS2D_FUNC pfunc);

   virtual void Draw(EagleGraphicsContext* win , int x , int y);

   void OnLoopComplete();
   void OnComplete();
};

#include "Eagle/Transforms.hpp"

typedef Transform (*TRANS_FUNC) (double);

class TextAnimation : public XYTextAnimation {
   
protected :
   TRANS_FUNC tfunc;
   
   Transform trans;
   
   virtual void OnSetAnimationPercent();
public :
   
   TextAnimation(EagleFont* f , std::string s , TRANS_FUNC transfunc , COLOR_FUNC cfunc , POS2D_FUNC pfunc);

   virtual void Draw(EagleGraphicsContext* win , int x , int y);



   
};



#endif // AnimationData_HPP

