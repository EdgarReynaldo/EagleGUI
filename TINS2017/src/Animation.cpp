

#include "Animation.hpp"
#include "Eagle.hpp"



Animation::Animation() :
      AnimationBase()
{}


void XYAnimation::OnSetAnimationPercent() {
   pos = posfunc(AnimationBase::GetAnimationPercent());
}


XYAnimation::XYAnimation(POS2D_FUNC func) : 
      Animation(),
      posfunc(func),
      pos(posfunc(0.0))
{}


void XYTextAnimation::OnSetAnimationPercent() {
   text_color = colfunc(AnimationBase::GetAnimationPercent());
   XYAnimation::OnSetAnimationPercent();
}



XYTextAnimation::XYTextAnimation(EagleFont* f , std::string s , COLOR_FUNC cfunc , POS2D_FUNC pfunc) :
      XYAnimation(pfunc),
      text_font(f),
      text_string(s),
      text_color(cfunc(0.0)),
      colfunc(cfunc)
{}


void XYTextAnimation::Draw(EagleGraphicsContext* win , int x , int y) {
   win->DrawMultiLineTextString(text_font , text_string , x + pos.X() , y + pos.Y() , text_color ,  text_font->Height()/2 , HALIGN_CENTER , VALIGN_CENTER);
}



void XYTextAnimation::OnLoopComplete() {
///   EagleInfo() << "Loop complete" << std::endl;
}



void XYTextAnimation::OnComplete() {
///   EagleInfo() << "Animation complete" << std::endl;
}



TextAnimation::TextAnimation(EagleFont* f , std::string s , TRANS_FUNC transfunc , COLOR_FUNC cfunc , POS2D_FUNC pfunc) :
      XYTextAnimation(f,s,cfunc,pfunc),
      tfunc(transfunc),
      trans(transfunc(0.0))
{}



void TextAnimation::OnSetAnimationPercent() {
   trans = tfunc(GetAnimationPercent());
   XYTextAnimation::OnSetAnimationPercent();
}



void TextAnimation::Draw(EagleGraphicsContext* win , int x , int y) {
   win->GetTransformer()->PushViewTransform(trans);
   XYTextAnimation::Draw(win,x,y);
   win->GetTransformer()->PopViewTransform();
}





