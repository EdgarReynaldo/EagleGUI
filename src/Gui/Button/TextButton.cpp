



#include "Eagle/Gui/Button/TextButton.hpp"


#include "Eagle/Gui/WidgetDrawFuncs.hpp"

#include "Eagle/StringWork.hpp"



void TextButton::UseButton(BasicButton* button_to_use) {
   if (!button_to_use) {
      button_to_use = this;
   }
   real_button = button_to_use;
   text_decorator.DecorateWidget(real_button);
}



void TextButton::UseButtonLayout(Layout* button_layout) {
   text_decorator.UseLayout(button_layout);
}




/// TextButton functions forwarded from BasicButton base



void TextButton::SetButtonType(BUTTON_ACTION_TYPE type) {
   if (real_button == this) {
      BasicButton::SetButtonType(type);
      return;
   }
   real_button->SetButtonType(type);
}



void TextButton::SetHoverState (bool state) {
   if (real_button == this) {
      BasicButton::SetHoverState(state);
      return;
   }
   real_button->SetHoverState(state);
}



void TextButton::SetSpringDuration(double duration) {
   if (real_button == this) {
      BasicButton::SetSpringDuration(duration);
      return;
   }
   real_button->SetSpringDuration(duration);
}



void TextButton::SetButtonUpState(bool button_up) {
   if (real_button == this) {
      BasicButton::SetButtonUpState(button_up);
      return;
   }
   real_button->SetButtonUpState(button_up);
}



void TextButton::ToggleButton() {
   if (real_button == this) {
      BasicButton::ToggleButton();
      return;
   }
   real_button->ToggleButton();
}



void TextButton::SetClickArea(AreaBase* new_click_area , bool delete_when_done) {
   if (real_button == this) {
      BasicButton::SetClickArea(new_click_area , delete_when_done);
      return;
   }
   real_button->SetClickArea(new_click_area , delete_when_done);
}



void TextButton::SetButtonState(bool hover , bool up) {
   if (real_button == this) {
      BasicButton::SetButtonState(hover , up);
      return;
   }
   real_button->SetButtonState(hover , up);
}



void TextButton::SetInputGroup(InputGroup ig) {
   if (real_button == this) {
      BasicButton::SetInputGroup(ig);
      return;
   }
   real_button->SetInputGroup(ig);
}



bool TextButton::JustActivated() {
   if (real_button == this) {
      return BasicButton::JustActivated();
   }
   return real_button->JustActivated();
}



void TextButton::EnableHoverMessage(bool enabled) {
   if (real_button == this) {
      BasicButton::EnableHoverMessage(enabled);
      return;
   }
   real_button->EnableHoverMessage(enabled);
}



InputGroup TextButton::InputKey() {
   if (real_button == this) {
      return BasicButton::InputKey();
   }
   return real_button->InputKey();
}



BUTTON_STATE TextButton::ButtonState() {
   if (real_button == this) {
      return BasicButton::ButtonState();
   }
   return real_button->ButtonState();
}



bool TextButton::Up() {
   if (real_button == this) {
      return BasicButton::Up();
   }
   return real_button->Up();
}



bool TextButton::Hover() {
   if (real_button == this) {
      return BasicButton::Hover();
   }
   return real_button->Hover();
}



BUTTON_ACTION_TYPE TextButton::ActionType() {
   if (real_button == this) {
      return BasicButton::ActionType();
   }
   return real_button->ActionType();
}



/// TextButton functions forwarded from TextDecorator



void TextButton::UseTextLayout(Layout* text_layout) {
   text_decorator.UseTextLayout(text_layout);
}



void TextButton::CenterText(bool center_the_text) {
   text_decorator.CenterText(center_the_text);
}



void TextButton::ReCenterText() {
   text_decorator.ReCenterText();
}



void TextButton::UseTextWidget(BasicText* text_widget) {
   text_decorator.UseTextWidget(text_widget);
}



BasicText* TextButton::GetTextWidget() {
   return text_decorator.GetTextWidget();
}



void TextButton::Realign(HALIGNMENT hal , VALIGNMENT val , int hpad , int vpad) {
   GetTextWidget()->Realign(hal , val , hpad , vpad);
}



void TextButton::SetText(std::string text) {
   GetTextWidget()->SetText(text);
}



void TextButton::SetFont(EagleFont* font) {
   GetTextWidget()->SetFont(font);
}



/// TextButton functions forwarded from WidgetBase



int TextButton::PrivateHandleEvent(EagleEvent e) {
   if (real_button == this) {
      return BasicButton::PrivateHandleEvent(e);
   }
   return real_button->HandleEvent(e);
}



/** PrivateCheckInputs will be called by HandleEvent, no need to override
int TextButton::PrivateCheckInputs() {
   if (real_button == this) {
      return BasicButton::PrivateCheckInputs();
   }
   return real_button->PrivateCheckInputs();
}
//*/


void TextButton::PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos) {
   if (real_button == this) {
      BasicButton::PrivateDisplay(win,xpos,ypos);
      return;
   }
   real_button->Display(win,xpos,ypos);
}



int TextButton::PrivateUpdate(double tsec) {
   if (real_button == this) {
      return BasicButton::PrivateUpdate(tsec);
   }
   return real_button->Update(tsec);
}



void TextButton::QueueUserMessage(const WidgetMsg& wmsg) {
   if (real_button == this) {
      BasicButton::QueueUserMessage(wmsg);
      return;
   }
   real_button->QueueUserMessage(wmsg);
}



void TextButton::SetWidgetArea(int xpos , int ypos , int width , int height , bool notify_layout) {
   if (real_button == this) {
      BasicButton::SetWidgetArea(xpos,ypos,width,height,notify_layout);
      return;
   }
   real_button->SetWidgetArea(xpos,ypos,width,height,notify_layout);
}



void TextButton::SetMarginsExpandFromInner(int left , int right , int top , int bottom) {
   if (real_button == this) {
      BasicButton::SetMarginsExpandFromInner(left,right,top,bottom);
      return;
   }
   real_button->SetMarginsExpandFromInner(left,right,top,bottom);
}



void TextButton::SetMarginsContractFromOuter(int left , int right , int top , int bottom) {
   if (real_button == this) {
      BasicButton::SetMarginsContractFromOuter(left,right,top,bottom);
      return;
   }
   real_button->SetMarginsContractFromOuter(left,right,top,bottom);
}



void TextButton::SetParent(WidgetBase* parent) {
   if (real_button == this) {
      BasicButton::SetParent(parent);
      return;
   }
   real_button->SetParent(parent);
}



void TextButton::SetOwnerLayout(Layout* l) {
   if (real_button == this) {
      BasicButton::SetOwnerLayout(l);
      return;
   }
   real_button->SetOwnerLayout(l);
}



void TextButton::SetBackgroundPainter(BackgroundPainter* painter) {
   if (real_button == this) {
      BasicButton::SetBackgroundPainter(painter);
      return;
   }
   real_button->SetBackgroundPainter(painter);
}



void TextButton::SetFocusPainter(FocusPainter* painter) {
   if (real_button == this) {
      BasicButton::SetFocusPainter(painter);
      return;
   }
   real_button->SetFocusPainter(painter);
}



void TextButton::SetBackgroundDrawType(BG_DRAW_TYPE draw_type) {
   if (real_button == this) {
      BasicButton::SetBackgroundDrawType(draw_type);
      return;
   }
   real_button->SetBackgroundDrawType(draw_type);
}



void TextButton::SetFocusDrawType(FOCUS_DRAW_TYPE draw_type) {
   if (real_button == this) {
      BasicButton::SetFocusDrawType(draw_type);
      return;
   }
   real_button->SetFocusDrawType(draw_type);
}



void TextButton::SetColorset(const WidgetColorset& colors , bool set_descendants_colors) {
   if (real_button == this) {
      BasicButton::SetColorset(colors , set_descendants_colors);
      return;
   }
   real_button->SetColorset(colors , set_descendants_colors);
}



void TextButton::SetPrivateColorset(const WidgetColorset& colors) {
   if (real_button == this) {
      BasicButton::SetPrivateColorset(colors);
      return;
   }
   real_button->SetPrivateColorset(colors);
}



void TextButton::UseColorset(bool use_public_colorset) {
   if (real_button == this) {
      BasicButton::UseColorset(use_public_colorset);
      return;
   }
   real_button->UseColorset(use_public_colorset);
}



void TextButton::UsePrivateColorset(bool use_priv_colorset) {
   if (real_button == this) {
      BasicButton::UsePrivateColorset(use_priv_colorset);
      return;
   }
   real_button->UsePrivateColorset(use_priv_colorset);
}



void TextButton::SetFlagStates(UINT FLAGS , bool state) {
   if (real_button == this) {
      BasicButton::SetFlagStates(FLAGS , state);
      return;
   }
   real_button->SetFlagStates(FLAGS , state);
}



void TextButton::SetEnabledState(bool state) {
   if (real_button == this) {
      BasicButton::SetEnabledState(state);
      return;
   }
   real_button->SetEnabledState(state);
}



void TextButton::SetVisibilityState(bool state) {
   if (real_button == this) {
      BasicButton::SetVisibilityState(state);
      return;
   }
   real_button->SetVisibilityState(state);
}



void TextButton::SetFocusState(bool state) {
   if (real_button == this) {
      BasicButton::SetFocusState(state);
      return;
   }
   real_button->SetFocusState(state);
}



void TextButton::SetMoveableState(bool state) {
   if (real_button == this) {
      BasicButton::SetMoveableState(state);
      return;
   }
   real_button->SetMoveableState(state);
}



void TextButton::SetResizeableState(bool state) {
   if (real_button == this) {
      BasicButton::SetResizeableState(state);
      return;
   }
   real_button->SetResizeableState(state);
}



void TextButton::SetNeedsRedrawState(bool state) {
   if (real_button == this) {
      BasicButton::SetNeedsRedrawState(state);
      return;
   }
   real_button->SetNeedsRedrawState(state);
}



void TextButton::SetNeedsBgRedrawState(bool state) {
   if (real_button == this) {
      BasicButton::SetNeedsBgRedrawState(state);
      return;
   }
   real_button->SetNeedsBgRedrawState(state);
}



void TextButton::SetAllowCloseState(bool state) {
   if (real_button == this) {
      BasicButton::SetAllowCloseState(state);
      return;
   }
   real_button->SetAllowCloseState(state);
}



void TextButton::SetAllowOverlapState(bool state) {
   if (real_button == this) {
      BasicButton::SetAllowOverlapState(state);
      return;
   }
   real_button->SetAllowOverlapState(state);
}



/** We don't need to override these functions, they function properly on their own

virtual void SetRedrawFlag();/// Shortcut to SetNeedsRedrawState(true)
virtual void SetBgRedrawFlag();/// Shortcut to SetNeedsBgRedrawState(true)
virtual void ClearRedrawFlag();/// Shortcut to SetNeedsRedrawState(false) and SetNeedsBgRedrawState(false)

virtual void SetRedrawAllFlag();/// To tell parent widget handlers to redraw all widgets

virtual void ShowWidget();/// Makes the widget enabled and visible
virtual void HideWidget();/// Makes the widget disabled and invisible
virtual void ToggleWidgetVisibility();/// Toggles the enabled and visible state of the widget

//*/



bool TextButton::AcceptsFocus() {
   if (real_button == this) {
      return BasicButton::AcceptsFocus();
   }
   return real_button->AcceptsFocus();
}



bool TextButton::IsMouseOver(int realmsx , int realmsy) const {
   if (real_button == this) {
      return BasicButton::IsMouseOver(realmsx , realmsy);
   }
   return real_button->IsMouseOver(realmsx , realmsy);
}



bool TextButton::GiveWidgetFocus(WidgetBase* widget , bool notify_parent) {
   if (real_button == this) {
      return BasicButton::GiveWidgetFocus(widget , notify_parent);
   }
   return real_button->GiveWidgetFocus(widget , notify_parent);
}



void TextButton::SetBgImage(EagleImage* img , MARGIN_HCELL hcell, MARGIN_VCELL vcell) {
   if (real_button == this) {
      BasicButton::SetBgImage(img , hcell , vcell);
      return;
   }
   real_button->SetBgImage(img , hcell , vcell);
}



void TextButton::SetBgImages(EagleImage* imgs[3][3]) {
   if (real_button == this) {
      BasicButton::SetBgImages(imgs);
      return;
   }
   real_button->SetBgImages(imgs);
}



void TextButton::SetImagesHaveAlpha(bool have_alpha) {
   if (real_button == this) {
      BasicButton::SetImagesHaveAlpha(have_alpha);
      return;
   }
   real_button->SetImagesHaveAlpha(have_alpha);
}



void TextButton::SetMinInnerWidth(int w) {
   if (real_button == this) {
      BasicButton::SetMinInnerWidth(w);
      return;
   }
   real_button->SetMinInnerWidth(w);
}



void TextButton::SetMinInnerHeight(int h) {
   if (real_button == this) {
      BasicButton::SetMinInnerHeight(h);
      return;
   }
   real_button->SetMinInnerHeight(h);
}



void TextButton::SetMinInnerDimensions(int w , int h) {
   if (real_button == this) {
      BasicButton::SetMinInnerDimensions(w,h);
      return;
   }
   real_button->SetMinInnerDimensions(w,h);
}



void TextButton::SetDisplayPriority(int priority) {
   if (real_button == this) {
      BasicButton::SetDisplayPriority(priority);
      return;
   }
   real_button->SetDisplayPriority(priority);
}



int TextButton::AbsMinWidth() const {
   if (real_button == this) {
      return BasicButton::AbsMinWidth();
   }
   return real_button->AbsMinWidth();
}



int TextButton::AbsMinHeight() const {
   if (real_button == this) {
      return BasicButton::AbsMinHeight();
   }
   return real_button->AbsMinHeight();
}



bool TextButton::HasGui() {
   if (real_button == this) {
      return BasicButton::HasGui();
   }
   return real_button->HasGui();
}



WidgetHandler* TextButton::GetGui() {
   if (real_button == this) {
      return BasicButton::GetGui();
   }
   return real_button->GetGui();
}



WidgetHandler* TextButton::NearestParentGui() {
   if (real_button == this) {
      return BasicButton::NearestParentGui();
   }
   return real_button->NearestParentGui();
}



WidgetBase* TextButton::Root() {
   if (real_button == this) {
      return BasicButton::Root();
   }
   return real_button->Root();
}



WidgetHandler* TextButton::RootGui() {
   if (real_button == this) {
      return BasicButton::RootGui();
   }
   return real_button->RootGui();
}



int TextButton::AbsParentX() const {
   if (real_button == this) {
      return BasicButton::AbsParentX();
   }
   return real_button->AbsParentX();
}



int TextButton::AbsParentY() const {
   if (real_button == this) {
      return BasicButton::AbsParentY();
   }
   return real_button->AbsParentY();
}



Pos2d TextButton::GetParentOffset() const {
   if (real_button == this) {
      return BasicButton::GetParentOffset();
   }
   return real_button->GetParentOffset();
}



WidgetBase* TextButton::Parent() const  {
   if (real_button == this) {
      return BasicButton::Parent();
   }
   return real_button->Parent();
}



WidgetColorset& TextButton::WCols() {
   if (real_button == this) {
      return BasicButton::WCols();
   }
   return real_button->WCols();
}



const WidgetColorset& TextButton::WCols() const {
   if (real_button == this) {
      return BasicButton::WCols();
   }
   return real_button->WCols();
}



WidgetArea TextButton::Area() const {
   if (real_button == this) {
      return BasicButton::Area();
   }
   return real_button->Area();
}



Rectangle TextButton::OuterArea() const {
   if (real_button == this) {
      return BasicButton::OuterArea();
   }
   return real_button->OuterArea();
}



Rectangle TextButton::InnerArea() const {
   if (real_button == this) {
      return BasicButton::InnerArea();
   }
   return real_button->InnerArea();
}



int TextButton::MinWidth() const {
   if (real_button == this) {
      return BasicButton::MinWidth();
   }
   return real_button->MinWidth();
}



int TextButton::MinHeight() const {
   if (real_button == this) {
      return BasicButton::MinHeight();
   }
   return real_button->MinHeight();
}



int TextButton::MinInnerWidth() const {
   if (real_button == this) {
      return BasicButton::MinInnerWidth();
   }
   return real_button->MinInnerWidth();
}



int TextButton::MinInnerHeight() const {
   if (real_button == this) {
      return BasicButton::MinInnerHeight();
   }
   return real_button->MinInnerHeight();
}



UINT TextButton::Flags() const {
   if (real_button == this) {
      return BasicButton::Flags();
   }
   return real_button->Flags();
}



int TextButton::DisplayPriority() const {
   if (real_button == this) {
      return BasicButton::DisplayPriority();
   }
   return real_button->DisplayPriority();
}



BackgroundPainter* TextButton::GetBackgroundPainter() const {
   if (real_button == this) {
      return BasicButton::GetBackgroundPainter();
   }
   return real_button->GetBackgroundPainter();
}



BG_DRAW_TYPE TextButton::GetBackgroundDrawType() const {
   if (real_button == this) {
      return BasicButton::GetBackgroundDrawType();
   }
   return real_button->GetBackgroundDrawType();
}



FocusPainter* TextButton::GetFocusPainter() const {
   if (real_button == this) {
      return BasicButton::GetFocusPainter();
   }
   return real_button->GetFocusPainter();
}



FOCUS_DRAW_TYPE TextButton::GetFocusDrawType() const {
   if (real_button == this) {
      return BasicButton::GetFocusDrawType();
   }
   return real_button->GetFocusDrawType();
}



std::string TextButton::GetWidgetClassName() {
   if (real_button == this) {
      return "TextButton::BasicButton";
   }
   return StringPrintF("%s%s" , "TextButton::" , real_button->GetWidgetClassName());
}



std::ostream& TextButton::DescribeTo(std::ostream& os , Indenter indent) const {
   os << indent << "TextButton :" << std::endl;
   if (real_button == this) {
      BasicButton::DescribeTo(os,indent);
      return os;
   }
   real_button->DescribeTo(os,indent);
   return os;
}



