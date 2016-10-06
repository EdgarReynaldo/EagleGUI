



#include "Eagle/Gui/Button/TextButton.hpp"


#include "Eagle/Gui/WidgetDrawFuncs.hpp"

/**

int TextButton::PrivateHandleEvent(EagleEvent e) {
   int ret = DIALOG_OKAY;
   ret |= our_text->PrivateHandleEvent(e);
   if (!(ret & DIALOG_INPUT_USED)) {
      if (our_button == dynamic_cast<BasicButton*>(this)) {
         ret |= BasicButton::PrivateHandleEvent(e);
      }
      else {
         ret |= our_button->PrivateHandleEvent(e);
      }
   }
}



int TextButton::PrivateCheckInputs() {
   int ret = DIALOG_OKAY;
   ret |= our_text->PrivateCheckInputs();
   if (!(ret & DIALOG_INPUT_USED)) {
      if (our_button == dynamic_cast<BasicButton*>(this)) {
         ret |= BasicButton::PrivateCheckInputs();
      }
      else {
         ret |= our_button->PrivateCheckInputs();
      }
   }
}



void TextButton::PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos) {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      BasicButton::PrivateDisplay(win,xpos,ypos);
   }
   else {
      our_button->PrivateDisplay(win,xpos,ypos);
   }
   our_text->PrivateDisplay(win,xpos,ypos);
}



int TextButton::PrivateUpdate(double tsec) {
   int ret = DIALOG_OKAY;
   ret |= our_text->PrivateUpdate(tsec);
   if (!(ret & DIALOG_INPUT_USED)) {
      if (our_button == dynamic_cast<BasicButton*>(this)) {
         ret |= BasicButton::PrivateUpdate(tsec);
      }
      else {
         ret |= our_button->PrivateUpdate(tsec);
      }
   }
}



/// WidgetBase
void TextButton::QueueUserMessage(const WidgetMsg& wmsg) {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      BasicButton::QueueUserMessage(wmsg);  
   }
   else {
      our_button->QueueUserMessage(wmsg);
   }
}



/// Setters
void TextButton::SetParent(WidgetBase* parent) {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      BasicButton::SetParent(parent);
   }
   else {
      our_button->SetParent(parent);
   }
}



void TextButton::SetOwnerLayout(Layout* l) {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      BasicButton::SetOwnerLayout(l);
   }
   else {
      our_button->SetOwnerLayout(l);
   }
}



void TextButton::SetBackgroundPainter(BackgroundPainter* painter) {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      BasicButton::SetBackgroundPainter(painter);
   }
   else {
      our_button->SetBackgroundPainter(painter);
   }
}



void TextButton::SetFocusPainter(FocusPainter* painter) {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      BasicButton::SetFocusPainter(painter);
   }
   else {
      our_button->SetFocusPainter(painter);
   }
}



void TextButton::SetBackgroundDrawType(BG_DRAW_TYPE draw_type) {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      BasicButton::SetBackgroundDrawType(draw_type);
   }
   else {
      our_button->SetBackgroundDrawType(draw_type);
   }
}



void TextButton::SetFocusDrawType(FOCUS_DRAW_TYPE draw_type) {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      BasicButton::SetFocusDrawType(draw_type);
   }
   else {
      our_button->SetFocusDrawType(draw_type);
   }
}



void TextButton::SetColorset(const WidgetColorset& colors , bool set_descendants_colors) {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      BasicButton::SetColorset(colors , set_descendants_colors);
   }
   else {
      our_button->SetColorset(colors , set_descendants_colors);
   }
}



void TextButton::SetPrivateColorset(const WidgetColorset& colors) {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      BasicButton::SetPrivateColorset(colors);
   }
   else {
      our_button->SetPrivateColorset(colors);
   }
}



void TextButton::UseColorset(bool use_public_colorset) {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      BasicButton::UseColorset(use_public_colorset);
   }
   else {
      our_button->UseColorset(use_public_colorset);
   }
}



void TextButton::UsePrivateColorset(bool use_priv_colorset) {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      BasicButton::UsePrivateColorset(use_priv_colorset);
   }
   else {
      our_button->UsePrivateColorset(use_priv_colorset);
   }
}



void TextButton::SetFlagStates(UINT FLAGS , bool state) {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      BasicButton::SetFlagStates(FLAGS , state);
   }
   else {
      our_button->SetFlagStates(FLAGS , state);
   }
}



void TextButton::SetEnabledState      (bool state) {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      BasicButton::SetEnabledState(state);
   }
   else {
      our_button->SetEnabledState(state);
   }
}



void TextButton::SetVisibilityState   (bool state) {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      BasicButton::SetVisibilityState(state);
   }
   else {
      our_button->SetVisibilityState(state);
   }
}



void TextButton::SetHoverState        (bool state) {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      BasicButton::SetHoverState(state);
   }
   else {
      our_button->SetHoverState(state);
   }
}



void TextButton::SetFocusState        (bool state) {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      BasicButton::SetFocusState(state);
   }
   else {
      our_button->SetFocusState(state);
   }
}



void TextButton::SetMoveableState     (bool state) {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      BasicButton::SetMoveableState(state);
   }
   else {
      our_button->SetMoveableState(state);
   }
}



void TextButton::SetResizeableState   (bool state) {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      BasicButton::SetResizeableState(state);
   }
   else {
      our_button->SetResizableState(state);
   }
}



void TextButton::SetNeedsRedrawState  (bool state) {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      BasicButton::SetNeedsRedrawState(state);
   }
   else {
      our_button->SetNeedsRedrawState(state);
   }
}



void TextButton::SetNeedsBgRedrawState(bool state) {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      BasicButton::SetNeedsBgRedrawState(state);
   }
   else {
      our_button->SetNeedsBgRedrawState(state);
   }
}



void TextButton::SetAllowCloseState   (bool state) {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      BasicButton::SetAllowCloseState(state);
   }
   else {
      our_button->SetAllowCloseState(state);
   }
}



void TextButton::SetAllowOverlapState (bool state) {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      BasicButton::SetAllowOverlapState(state);
   }
   else {
      our_button->SetAllowOverlapState(state);
   }
}



void TextButton::SetRedrawFlag() {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      BasicButton::SetRedrawFlag();
   }
   else {
      our_button->SetRedrawFlag();
   }
}



void TextButton::SetBgRedrawFlag() {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      BasicButton::SetBgRedrawFlag();
   }
   else {
      our_button->SetBgRedrawFlag();
   }
}



void TextButton::ClearRedrawFlag() {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      BasicButton::ClearRedrawFlag();
   }
   else {
      our_button->ClearRedrawFlag();
   }
}



void TextButton::SetRedrawAllFlag() {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      BasicButton::SetRedrawAllFlag();
   }
   else {
      our_button->SetRedrawAllFlag();
   }
}



void TextButton::ShowWidget() {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      BasicButton::ShowWidget();
   }
   else {
      our_button->ShowWidget();
   }
}



void TextButton::HideWidget() {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      BasicButton::HideWidget();
   }
   else {
      our_button->HideWidget();
   }
}



void TextButton::ToggleWidgetVisibility() {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      BasicButton::ToggleWidgetVisibility();
   }
   else {
      our_button->ToggleWidgetVisibility();
   }
}



bool TextButton::AcceptsFocus() {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      return BasicButton::AcceptsFocus();
   }
   else {
      return our_button->AcceptsFocus();
   }
}



bool TextButton::IsMouseOver(int realmsx , int realmsy) const {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      return BasicButton::IsMouseOver(realmsx , realmsy);
   }
   else {
      return our_button->IsMouseOver(realmsx , realmsy);
   }
}



bool TextButton::GiveWidgetFocus(WidgetBase* widget , bool notify_parent) {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      return BasicButton::GiveWidgetFocus(widget , notify_parent);
   }
   else {
      return our_button->GiveWidgetFocus(widget , notify_parent);
   }
}



void TextButton::SetBgImage(EagleImage* img , MARGIN_HCELL hcell, MARGIN_VCELL vcell) {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      BasicButton::SetBgImage(img , hcell , vcell);
   }
   else {
      our_button->SetBgImage(img , hcell , vcell);
   }
}



void TextButton::SetBgImages(EagleImage* imgs[3][3]) {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      BasicButton::SetBgImages(imgs);
   }
   else {
      our_button->SetBgImages(imgs);
   }
}



void TextButton::SetImagesHaveAlpha(bool have_alpha) {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      BasicButton::SetImagesHaveAlpha(have_alpha);
   }
   else {
      our_button->SetImagesHaveAlpha(have_alpha);
   }
}



void TextButton::SetWidgetArea(int xpos , int ypos , int width , int height , bool notify_layout) {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      BasicButton::SetWidgetArea(xpos,ypos,width,height,notify_layout);
   }
   else {
      our_button->SetWidgetArea(xpos,ypos,width,height,notify_layout);
   }
}



void TextButton::SetMarginsExpandFromInner(int left , int right , int top , int bottom) {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      BasicButton::SetMarginsExpandFromInner(left,right,top,bottom);
   }
   else {
      our_button->SetMarginsExpandFromInner(left,right,top,bottom);
   }
}



void TextButton::SetMarginsContractFromOuter(int left , int right , int top , int bottom) {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      BasicButton::SetMarginsContractFromOuter(left,right,top,bottom);
   }
   else {
      our_button->SetMarginsContractFromOuter(left,right,top,bottom);
   }
}



void TextButton::SetMinInnerWidth(int w) {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      BasicButton::SetMinInnerWidth(w);
   }
   else {
      our_button->SetMinInnerWidth(w);
   }
}



void TextButton::SetMinInnerHeight(int h) {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      BasicButton::SetMinInnerHeight(h);
   }
   else {
      our_button->SetMinInnerHeight(h);
   }
}



void TextButton::SetMinInnerDimensions(int w , int h) {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      BasicButton::SetMinInnerDimensions(w,h);
   }
   else {
      our_button->SetMinInnerDimensions(w,h);
   }
}



void TextButton::SetDisplayPriority(int priority) {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      BasicButton::SetDisplayPriority(priority);
   }
   else {
      our_button->SetDisplayPriority(priority);
   }
}



int TextButton::AbsMinWidth() const {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      return BasicButton::AbsMinWidth();
   }
   else {
      return our_button->AbsMinWidth();
   }
}



int TextButton::AbsMinHeight() const {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      return BasicButton::AbsMinHeight();
   }
   else {
      return our_button->AbsMinHeight();
   }
}



bool TextButton::HasGui() {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      return BasicButton::HasGui();
   }
   else {
      return our_button->HasGui();
   }
}



WidgetHandler* TextButton::GetGui() {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      return BasicButton::GetGui();
   }
   else {
      return our_button->GetGui();
   }
}




WidgetHandler* TextButton::NearestParentGui() {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      return BasicButton::NearestParentGui();
   }
   else {
      return our_button->NearestParentGui();
   }
}



WidgetBase* TextButton::Root() {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      return BasicButton::Root();
   }
   else {
      return our_button->Root();
   }
}



WidgetHandler* TextButton::RootGui() {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      return BasicButton::RootGui();
   }
   else {
      return our_button->RootGui();
   }
}



int TextButton::AbsParentX() const {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      return BasicButton::AbsParentX();
   }
   else {
      return our_button->AbsParentX();
   }
}



int TextButton::AbsParentY() const {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      return BasicButton::AbsParentY();
   }
   else {
      return our_button->AbsParentY();
   }
}



Pos2d TextButton::GetParentOffset() const {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      return BasicButton::GetParentOffset();
   }
   else {
      return our_button->GetParentOffset();
   }
}



WidgetBase* TextButton::Parent() const {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      return BasicButton::Parent();
   }
   else {
      return our_button->Parent();
   }
}



WidgetColorset& TextButton::WCols() {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      return BasicButton::WCols();
   }
   else {
      return our_button->WCols();
   }
}



const WidgetColorset& TextButton::WCols() const {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      return BasicButton::WCols();
   }
   else {
      return our_button->WCols();
   }
}



WidgetArea TextButton::Area() const {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      return BasicButton::Area();
   }
   else {
      return our_button->Area();
   }
}



Rectangle TextButton::OuterArea() const {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      return BasicButton::OuterArea();
   }
   else {
      return our_button->OuterArea();
   }
}



Rectangle TextButton::InnerArea() const {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      return BasicButton::InnerArea();
   }
   else {
      return our_button->InnerArea();
   }
}



int TextButton::MinWidth() const {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      return BasicButton::MinWidth();
   }
   else {
      return our_button->MinWidth();
   }
}



int TextButton::MinHeight() const {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      return BasicButton::MinHeight();
   }
   else {
      return our_button->MinHeight();
   }
}



int TextButton::MinInnerWidth() const {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      return BasicButton::MinInnerWidth();
   }
   else {
      return our_button->MinInnerWidth();
   }
}



int TextButton::MinInnerHeight() const {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      return BasicButton::MinInnerHeight();
   }
   else {
      return our_button->MinInnerHeight();
   }
}



UINT TextButton::Flags() const {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      return BasicButton::Flags();
   }
   else {
      return our_button->Flags();
   }
}



int TextButton::DisplayPriority() const {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      return BasicButton::DisplayPriority();
   }
   else {
      return our_button->DisplayPriority();
   }
}



std::string TextButton::GetWidgetClassName() {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      return "TextButton::BasicButton";
   }
   else {
      return our_button->GetWidgetClassName();
   }
}



std::ostream& TextButton::DescribeTo(std::ostream& os , Indenter indent) const {
   if (our_button == dynamic_cast<BasicButton*>(this)) {
      return BasicButton::DescribeTo(os,indent);
   }
   else {
      return our_button->DescribeTo(os,indent);
   }
}

//*/




