

#include "Animation.hpp"
#include "Eagle.hpp"



Animation::Animation() :
      AnimationBase()
{}


void XYAnimation::OnSetAnimationPercent() {
   pos = posfunc(AnimationBase::GetAnimationPercent());
}


XYAnimation::XYAnimation() : 
      Animation(),
      posfunc(0),
      pos()
{
}



XYAnimation::XYAnimation(POS2D_FUNC func) : 
      Animation(),
      posfunc(0),
      pos()
{
   Create(func);
}



void XYAnimation::Create(POS2D_FUNC func) {
   posfunc = func;
   OnSetAnimationPercent();
}



void XYTextAnimation::OnSetAnimationPercent() {
   text_color = colfunc(AnimationBase::GetAnimationPercent());
   XYAnimation::OnSetAnimationPercent();
}



XYTextAnimation::XYTextAnimation() :
      XYAnimation(),
      text_font(0),
      text_string(""),
      text_color(),
      colfunc(0)
{
   
}


XYTextAnimation::XYTextAnimation(EagleFont* f , std::string s , COLOR_FUNC cfunc , POS2D_FUNC pfunc) :
      XYAnimation(),
      text_font(0),
      text_string(""),
      text_color(),
      colfunc(0)
{
   Create(f,s,cfunc,pfunc);
}





void XYTextAnimation::Create(EagleFont* f , std::string s , COLOR_FUNC cfunc , POS2D_FUNC pfunc) {
   text_font = f;
   text_string = s;
   colfunc = cfunc;
   posfunc = pfunc;
   OnSetAnimationPercent();
}



void XYTextAnimation::Draw(EagleGraphicsContext* win , int x , int y) {
   win->DrawMultiLineTextString(text_font , text_string , x + pos.X() , y + pos.Y() , text_color ,  text_font->Height()/2 , HALIGN_CENTER , VALIGN_CENTER);
}



void XYTextAnimation::OnLoopComplete() {
///   EagleInfo() << "Loop complete" << std::endl;
}



void XYTextAnimation::OnComplete() {
///   EagleInfo() << "Animation complete" << std::endl;
}



TextAnimation::TextAnimation() :
      XYTextAnimation(),
      tfunc(0),
      trans()
{}



TextAnimation::TextAnimation(EagleFont* f , std::string s , TRANS_FUNC transfunc , COLOR_FUNC cfunc , POS2D_FUNC pfunc) :
      XYTextAnimation(),
      tfunc(0),
      trans()
{
   Create(f , s , transfunc , cfunc , pfunc);
}



void TextAnimation::Create(EagleFont* f , std::string s , TRANS_FUNC transfunc , COLOR_FUNC cfunc , POS2D_FUNC pfunc) {
   tfunc = transfunc;
   XYTextAnimation::Create(f , s  , cfunc , pfunc);
   OnSetAnimationPercent();
}



void TextAnimation::OnSetAnimationPercent() {
   trans = tfunc(GetAnimationPercent());
   XYTextAnimation::OnSetAnimationPercent();
}



void TextAnimation::Draw(EagleGraphicsContext* win , int x , int y) {
   win->GetTransformer()->PushViewTransform(trans);
   XYTextAnimation::Draw(win,x,y);
   win->GetTransformer()->PopViewTransform();
}





