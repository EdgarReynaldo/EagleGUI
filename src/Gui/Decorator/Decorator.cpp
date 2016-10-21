

#include "Eagle/Gui/WidgetHandler.hpp"

#include "Eagle/Gui/Decorators/Decorator.hpp"


#include "Eagle/StringWork.hpp"

/// -----------------------     WidgetDecoratorBase     --------------------------------------




void WidgetDecorator::PrivateRaiseEvent(WidgetMsg msg) {
   return WidgetBase::PrivateRaiseEvent(msg);
   
///   EAGLE_ASSERT(decorated_widget);
///   decorated_widget->PrivateRaiseEvent(msg);
}



/// -----------------     WidgetDecoratorBase member functions     ------------------------------



WidgetDecorator::WidgetDecorator(WidgetBase* widget_to_decorate,
                                 Layout* widget_layout,
                                 std::string decorator_name) :
      WidgetBase(decorator_name),
      decorated_widget(0),
      layout(&default_dumb_layout),
      default_dumb_layout()
{
   DecorateWidget(widget_to_decorate);
   UseLayout(widget_layout);
   if (decorator_name.compare("") == 0) {
      SetName(StringPrintF("WidgetDecorator at %p" , this));
   }
}



WidgetDecorator::~WidgetDecorator() {
   decorated_widget = 0;
}



void WidgetDecorator::DecorateWidget(WidgetBase* widget_to_decorate) {
   
   if (decorated_widget) {
      layout->RemoveWidget(decorated_widget);
      decorated_widget->WidgetBase::SetParent(0);
      decorated_widget->WidgetBase::SetDecoratorParent(0);
      decorated_widget = 0;
   }
   
   if (widget_to_decorate) {
      decorated_widget = widget_to_decorate;
      layout->Resize(1);
      layout->PlaceWidget(decorated_widget , 0);
      decorated_widget->WidgetBase::SetParent(this);
      decorated_widget->WidgetBase::SetDecoratorParent(this);
      decorated_widget->WidgetBase::SetFlagStates(Flags() , true);
      decorated_widget->WidgetBase::SetFlagStates(~Flags() , false);
   }
}



void WidgetDecorator::UseLayout(Layout* new_layout) {

   if (layout) {
      layout->SetParent(0);
   }

   WidgetBase* old_decorated_widget = decorated_widget;
   DecorateWidget(0);

   if (!new_layout) {
      new_layout = &default_dumb_layout;
   }
   layout = new_layout;
   layout->SetParent(this);
   layout->WidgetBase::SetWidgetArea(area.InnerArea() , false);
   
   DecorateWidget(old_decorated_widget);
}
   




/// ---------------      WidgetDecoratorBase overloaded functions from WidgetBase      -----------------



int WidgetDecorator::PrivateHandleEvent(EagleEvent e) {
   
   int ret = DIALOG_OKAY;
   
   if (decorated_widget) {
      ret |= decorated_widget->HandleEvent(e);
   }
   if (!(ret & DIALOG_INPUT_USED)) {
      ret |= layout->HandleEvent(e);
   }
   
   return ret;
///   throw EagleError("WidgetDecorator::PrivateHandleEvent : This function should not be called!");
///   return -1;
}



int WidgetDecorator::PrivateCheckInputs() {
   
   return WidgetBase::PrivateCheckInputs();
   
///   throw EagleError("WidgetDecorator::PrivateCheckInputs : This function should not be called!");
///   return -1;

///   return DIALOG_OKAY;
   
///   return decorated_widget->PrivateCheckInputs();
   
}



void WidgetDecorator::PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos) {

   layout->Display(win , xpos , ypos);
   if (decorated_widget) {
      decorated_widget->Display(win , xpos , ypos);
   }
///   throw EagleError("WidgetDecorator::PrivateHandleEvent : This function should not be called!");
}



int WidgetDecorator::PrivateUpdate(double tsec) {
   int ret = DIALOG_OKAY;

   ret |= layout->Update(tsec);
   if (decorated_widget) {
      ret |= decorated_widget->Update(tsec);
   }
   return ret;
///   throw EagleError("WidgetDecorator::PrivateUpdate : This function should not be called!");
///   return -1;
}


/** Don't overload these functions. Allow them to be called normally by the GUI

int WidgetDecorator::HandleEvent(EagleEvent e) {
   EAGLE_ASSERT(decorated_widget);
   return decorated_widget->HandleEvent(e);
}



void WidgetDecorator::Display(EagleGraphicsContext* win , int xpos , int ypos) {
   EAGLE_ASSERT(decorated_widget);
   decorated_widget->Display(win,xpos,ypos);
}



int WidgetDecorator::Update(double tsec) {
   EAGLE_ASSERT(decorated_widget);
   return decorated_widget->Update(tsec);
}

*/



void WidgetDecorator::QueueUserMessage(const WidgetMsg& wmsg) {
   WidgetBase::QueueUserMessage(wmsg);
   
///   EAGLE_ASSERT(decorated_widget);
///   decorated_widget->QueueUserMessage(wmsg);
}



void WidgetDecorator::SetWidgetArea(int xpos , int ypos , int width , int height , bool notify_layout) {

   WidgetBase::SetWidgetArea(xpos,ypos,width,height,notify_layout);
   
   layout->WidgetBase::SetWidgetArea(area.InnerArea() , false);
///   EAGLE_ASSERT(decorated_widget);
///   decorated_widget->SetWidgetArea(xpos,ypos,width,height,notify_layout);

   /** Sync area with decorated widget?
   WidgetArea na = decorated_widget->Area();
   Rectangle nr = na.OuterArea();

   WidgetBase::SetWidgetArea(nr.X() , nr.Y() , nr.W() , nr.H() , false);
   WidgetBase::SetMarginsContractFromOuter(na.MLeft() , na.MRight() , na.MTop() , na.MBot());
   //*/
}



void WidgetDecorator::SetMarginsExpandFromInner(int left , int right , int top , int bottom) {

   WidgetBase::SetMarginsExpandFromInner(left,right,top,bottom);

   layout->WidgetBase::SetWidgetArea(area.InnerArea() , false);
///   EAGLE_ASSERT(decorated_widget);
///   decorated_widget->SetMarginsExpandFromInner(left,right,top,bottom);

   /** Sync area with decorated widget?
   WidgetArea na = decorated_widget->Area();
   Rectangle nr = na.OuterArea();

   WidgetBase::SetWidgetArea(nr.X() , nr.Y() , nr.W() , nr.H() , false);
   WidgetBase::SetMarginsContractFromOuter(na.MLeft() , na.MRight() , na.MTop() , na.MBot());
   //*/
}



void WidgetDecorator::SetMarginsContractFromOuter(int left , int right , int top , int bottom) {

   WidgetBase::SetMarginsContractFromOuter(left,right,top,bottom);

   layout->WidgetBase::SetWidgetArea(area.InnerArea() , false);

///   EAGLE_ASSERT(decorated_widget);
///   decorated_widget->SetMarginsContractFromOuter(left,right,top,bottom);

   /** Sync area with decorated widget?
   WidgetArea na = decorated_widget->Area();
   Rectangle nr = na.OuterArea();

   WidgetBase::SetWidgetArea(nr.X() , nr.Y() , nr.W() , nr.H() , false);
   WidgetBase::SetMarginsContractFromOuter(na.MLeft() , na.MRight() , na.MTop() , na.MBot());
   //*/
}



void WidgetDecorator::SetParent(WidgetBase* parent) {
   WidgetBase::SetParent(parent);
///   EAGLE_ASSERT(decorated_widget);
///   decorated_widget->SetParent(parent);
}



void WidgetDecorator::SetOwnerLayout(Layout* l) {
   WidgetBase::SetOwnerLayout(l);
///   EAGLE_ASSERT(decorated_widget);
///   decorated_widget->SetOwnerLayout(l);
}



void WidgetDecorator::SetBackgroundPainter(BackgroundPainter* painter) {
   WidgetBase::SetBackgroundPainter(painter);
///   EAGLE_ASSERT(decorated_widget);
///   decorated_widget->SetBackgroundPainter(painter);
}



void WidgetDecorator::SetFocusPainter(FocusPainter* painter) {
   WidgetBase::SetFocusPainter(painter);
///   EAGLE_ASSERT(decorated_widget);
///   decorated_widget->SetFocusPainter(painter);
}



void WidgetDecorator::SetBackgroundDrawType(BG_DRAW_TYPE draw_type) {
   WidgetBase::SetBackgroundDrawType(draw_type);
///   EAGLE_ASSERT(decorated_widget);
///   decorated_widget->SetBackgroundDrawType(draw_type);
}



void WidgetDecorator::SetFocusDrawType(FOCUS_DRAW_TYPE draw_type) {
   WidgetBase::SetFocusDrawType(draw_type);
///   EAGLE_ASSERT(decorated_widget);
///   decorated_widget->SetFocusDrawType(draw_type);
}



void WidgetDecorator::SetColorset(const WidgetColorset& colors , bool set_descendants_colors) {
   WidgetBase::SetColorset(colors , set_descendants_colors);
   
///   EAGLE_ASSERT(decorated_widget);
///   decorated_widget->SetColorset(colors , set_descendants_colors);
}



void WidgetDecorator::SetPrivateColorset(const WidgetColorset& colors) {
   WidgetBase::SetPrivateColorset(colors);

///   EAGLE_ASSERT(decorated_widget);
///   decorated_widget->SetPrivateColorset(colors);
}



void WidgetDecorator::UseColorset(bool use_public_colorset) {
   WidgetBase::UseColorset(use_public_colorset);

///   EAGLE_ASSERT(decorated_widget);
///   decorated_widget->UseColorset(use_public_colorset);
}



void WidgetDecorator::UsePrivateColorset(bool use_priv_colorset) {
   WidgetBase::UsePrivateColorset(use_priv_colorset);

///   EAGLE_ASSERT(decorated_widget);
///   decorated_widget->UsePrivateColorset(use_priv_colorset);
}



/**
   Sync the decorated widget's flags with our own
*/



void WidgetDecorator::SetFlagStates(UINT FLAGS , bool state) {
   WidgetBase::SetFlagStates(FLAGS , state);
   if (decorated_widget) {
      decorated_widget->SetFlagStates(FLAGS , state);
   }
}



void WidgetDecorator::SetEnabledState(bool state) {
   WidgetBase::SetEnabledState(state);
   if (decorated_widget) {
      decorated_widget->SetEnabledState(state);
   }
}



void WidgetDecorator::SetVisibilityState(bool state) {
   WidgetBase::SetVisibilityState(state);
   if (decorated_widget) {
      decorated_widget->SetVisibilityState(state);
   }
}



void WidgetDecorator::SetHoverState(bool state) {
   if (!state) {
      if (decorated_widget) {
         decorated_widget->SetHoverState(false);
      }
      WidgetBase::SetHoverState(false);
      return;
   }

   WidgetBase::SetHoverState(true);

   WidgetHandler* wh = NearestParentGui();
   if (wh) {
      int mx = wh->GetMouseX();
      int my = wh->GetMouseY();
      if (decorated_widget->OuterArea().Contains(mx,my)) {
         decorated_widget->SetHoverState(true);
      }
   }
   else {
      if (decorated_widget) {
         decorated_widget->SetHoverState(true);
      }
   }
}



void WidgetDecorator::SetFocusState(bool state) {
   WidgetBase::SetFocusState(state);
   if (decorated_widget) {
      decorated_widget->SetFocusState(state);
   }
}



void WidgetDecorator::SetMoveableState(bool state) {
   WidgetBase::SetMoveableState(state);
   if (decorated_widget) {
      decorated_widget->SetMoveableState(state);
   }
}



void WidgetDecorator::SetResizeableState(bool state) {
   WidgetBase::SetResizeableState(state);
   if (decorated_widget) {
      decorated_widget->SetResizeableState(state);
   }
}



void WidgetDecorator::SetNeedsRedrawState(bool state) {
   WidgetBase::SetNeedsRedrawState(state);
   if (decorated_widget) {
      decorated_widget->SetNeedsRedrawState(state);
   }
}



void WidgetDecorator::SetNeedsBgRedrawState(bool state) {
   WidgetBase::SetNeedsBgRedrawState(state);
   if (decorated_widget) {
      decorated_widget->SetNeedsBgRedrawState(state);
   }
}



void WidgetDecorator::SetAllowCloseState(bool state) {
   WidgetBase::SetAllowCloseState(state);
   if (decorated_widget) {
      decorated_widget->SetAllowCloseState(state);
   }
}



void WidgetDecorator::SetAllowOverlapState(bool state) {
   WidgetBase::SetAllowOverlapState(state);
   if (decorated_widget) {
      decorated_widget->SetAllowOverlapState(state);
   }
}




/**

void WidgetDecorator::SetRedrawFlag() {
   WidgetBase::SetRedrawFlag();
   if (decorated_widget) {
      decorated_widget->SetRedrawFlag();
   }
}



void WidgetDecorator::SetBgRedrawFlag() {
   WidgetBase::SetBgRedrawFlag();
   if (decorated_widget) {
      decorated_widget->SetBgRedrawFlag();
   }
}



void WidgetDecorator::ClearRedrawFlag() {
   WidgetBase::ClearRedrawFlag();
   if (decorated_widget) {
      decorated_widget->ClearRedrawFlag();
   }
}



void WidgetDecorator::SetRedrawAllFlag() {
   WidgetBase::SetRedrawAllFlag();
   if (decorated_widget) {
      decorated_widget->SetRedrawAllFlag();
   }
}



void WidgetDecorator::ShowWidget() {
   WidgetBase::ShowWidget();
   if (decorated_widget) {
      decorated_widget->ShowWidget();
   }
}



void WidgetDecorator::HideWidget() {
   WidgetBase::HideWidget();
   if (decorated_widget) {
      decorated_widget->HideWidget();
   }
}



void WidgetDecorator::ToggleWidgetVisibility() {
   WidgetBase::ToggleWidgetVisibility();
   if (decorated_widget) {
      decorated_widget->ToggleWidgetVisibility();
   }
}

//*/

bool WidgetDecorator::AcceptsFocus() {
   if (decorated_widget) {
      return decorated_widget->AcceptsFocus();
   }
   return false;
}



bool WidgetDecorator::IsMouseOver(int realmsx , int realmsy) const {
   
   /// TODO : Should we return whether the mouse is over us or our decorated widget?
   
   return WidgetBase::IsMouseOver(realmsx,realmsy);
///   EAGLE_ASSERT(decorated_widget);
///   return decorated_widget->IsMouseOver(realmsx , realmsy);
}



bool WidgetDecorator::GiveWidgetFocus(WidgetBase* widget , bool notify_parent) {
   return WidgetBase::GiveWidgetFocus(widget , notify_parent);
}



void WidgetDecorator::SetBgImage(EagleImage* img , MARGIN_HCELL hcell, MARGIN_VCELL vcell) {
   WidgetBase::SetBgImage(img , hcell , vcell);
///   EAGLE_ASSERT(decorated_widget);
///   decorated_widget->SetBgImage(img,hcell,vcell);
}



void WidgetDecorator::SetBgImages(EagleImage* imgs[3][3]) {
   WidgetBase::SetBgImages(imgs);
///   EAGLE_ASSERT(decorated_widget);
///   decorated_widget->SetBgImages(imgs);
}



void WidgetDecorator::SetImagesHaveAlpha(bool have_alpha) {
   WidgetBase::SetImagesHaveAlpha(have_alpha);
///   EAGLE_ASSERT(decorated_widget);
///   decorated_widget->SetImagesHaveAlpha(have_alpha);
}



void WidgetDecorator::SetMinInnerWidth(int w) {
   WidgetBase::SetMinInnerWidth(w);
///   EAGLE_ASSERT(decorated_widget);
///   decorated_widget->SetMinInnerWidth(w);
}



void WidgetDecorator::SetMinInnerHeight(int h) {
   WidgetBase::SetMinInnerHeight(h);
///   EAGLE_ASSERT(decorated_widget);
///   decorated_widget->SetMinInnerHeight(h);
}



void WidgetDecorator::SetMinInnerDimensions(int w , int h) {
   WidgetBase::SetMinInnerDimensions(w,h);
///   EAGLE_ASSERT(decorated_widget);
///   decorated_widget->SetMinInnerDimensions(w,h);
}



void WidgetDecorator::SetDisplayPriority(int priority) {
   WidgetBase::SetDisplayPriority(priority);
///   EAGLE_ASSERT(decorated_widget);
///   decorated_widget->SetDisplayPriority(priority);
}



int WidgetDecorator::AbsMinWidth() const {
   return WidgetBase::AbsMinWidth();
///   EAGLE_ASSERT(decorated_widget);
///   return decorated_widget->AbsMinWidth();
}



int WidgetDecorator::AbsMinHeight() const {
   return WidgetBase::AbsMinHeight();
///   EAGLE_ASSERT(decorated_widget);
///   return decorated_widget->AbsMinHeight();
}



bool WidgetDecorator::HasGui() {
   return WidgetBase::HasGui();
///   EAGLE_ASSERT(decorated_widget);
///   return decorated_widget->HasGui();
}



WidgetHandler* WidgetDecorator::GetGui() {
   return WidgetBase::GetGui();
///   EAGLE_ASSERT(decorated_widget);
///   return decorated_widget->GetGui();
}



WidgetHandler* WidgetDecorator::NearestParentGui() {
   return WidgetBase::NearestParentGui();
///   EAGLE_ASSERT(decorated_widget);
///   return decorated_widget->NearestParentGui();
}



WidgetBase* WidgetDecorator::Root() {
   return WidgetBase::Root();
   
///   EAGLE_ASSERT(decorated_widget);
///   return decorated_widget->Root();
}



WidgetHandler* WidgetDecorator::RootGui() {
   return WidgetBase::RootGui();
   
///   EAGLE_ASSERT(decorated_widget);
///   return decorated_widget->RootGui();
}



int WidgetDecorator::AbsParentX() const {
   return WidgetBase::AbsParentX();
   
///   EAGLE_ASSERT(decorated_widget);
///   return decorated_widget->AbsParentX();
}



int WidgetDecorator::AbsParentY() const {
   return WidgetBase::AbsParentY();
   
///   EAGLE_ASSERT(decorated_widget);
///   return decorated_widget->AbsParentY();
}



Pos2d WidgetDecorator::GetParentOffset() const {
   return WidgetBase::GetParentOffset();
   
///   EAGLE_ASSERT(decorated_widget);
///   return decorated_widget->GetParentOffset();
}



WidgetBase* WidgetDecorator::Parent() const {
   return WidgetBase::Parent();
   
///   EAGLE_ASSERT(decorated_widget);
///   return decorated_widget->Parent();
}



WidgetColorset& WidgetDecorator::WCols() {
   return WidgetBase::WCols();
   
///   EAGLE_ASSERT(decorated_widget);
///   return decorated_widget->WCols();
}



const WidgetColorset& WidgetDecorator::WCols() const {
   return WidgetBase::WCols();
   
///   EAGLE_ASSERT(decorated_widget);
///   return decorated_widget->WCols();
}



WidgetArea WidgetDecorator::Area() const {
   return WidgetBase::Area();
   
///   EAGLE_ASSERT(decorated_widget);
///   return decorated_widget->Area();
}



Rectangle WidgetDecorator::OuterArea() const {
   return WidgetBase::OuterArea();
   
///   EAGLE_ASSERT(decorated_widget);
///   return decorated_widget->OuterArea();
}



Rectangle WidgetDecorator::InnerArea() const {
   return WidgetBase::InnerArea();

///   EAGLE_ASSERT(decorated_widget);
///   return decorated_widget->InnerArea();
}



int WidgetDecorator::MinWidth() const {
   return WidgetBase::MinWidth();
   
///   EAGLE_ASSERT(decorated_widget);
///   return decorated_widget->MinWidth();
}



int WidgetDecorator::MinHeight() const {
   return WidgetBase::MinHeight();
   
///   EAGLE_ASSERT(decorated_widget);
///   return decorated_widget->MinHeight();
}



int WidgetDecorator::MinInnerWidth() const {
   return WidgetBase::MinInnerWidth();
   
///   EAGLE_ASSERT(decorated_widget);
///   return decorated_widget->MinInnerWidth();
}



int WidgetDecorator::MinInnerHeight() const {
   return WidgetBase::MinInnerHeight();

///   EAGLE_ASSERT(decorated_widget);
///   return decorated_widget->MinInnerHeight();
}



UINT WidgetDecorator::Flags() const {
   return WidgetBase::Flags();

///   EAGLE_ASSERT(decorated_widget);
///   return decorated_widget->Flags();
}



int WidgetDecorator::DisplayPriority() const {
   return WidgetBase::DisplayPriority();

///   EAGLE_ASSERT(decorated_widget);
///   return decorated_widget->DisplayPriority();
}



BackgroundPainter* WidgetDecorator::GetBackgroundPainter() const {
   return WidgetBase::GetBackgroundPainter();

///   EAGLE_ASSERT(decorated_widget);
///   return decorated_widget->BackgroundPainter();
}



BG_DRAW_TYPE WidgetDecorator::GetBackgroundDrawType() const {
   return WidgetBase::GetBackgroundDrawType();

///   EAGLE_ASSERT(decorated_widget);
///   return decorated_widget->BackgroundDrawType();
}



FocusPainter* WidgetDecorator::GetFocusPainter() const {
   return WidgetBase::GetFocusPainter();

///   EAGLE_ASSERT(decorated_widget);
///   return decorated_widget->FocusPainter();
}



FOCUS_DRAW_TYPE WidgetDecorator::GetFocusDrawType() const {
   return WidgetBase::GetFocusDrawType();

///   EAGLE_ASSERT(decorated_widget);
///   return decorated_widget->FocusDrawType();
}



std::string WidgetDecorator::GetWidgetClassName() {
   return WidgetBase::GetWidgetClassName();

///   EAGLE_ASSERT(decorated_widget);
///   return std::string("WidgetDecorator<") + decorated_widget->GetWidgetClassName(); + std::string(">");
}


using std::endl;
std::ostream& WidgetDecorator::DescribeTo(std::ostream& os , Indenter indent) const {

   os << indent << StringPrintF("< WidgetDecoratorBase %s at %p >" , GetName() , this) << endl;
   ++indent;
   WidgetBase::DescribeTo(os,indent);
   --indent;

   os << StringPrintF("< WidgetDecoratorBase : Decorator layout %s at %p >" , layout->GetName().c_str() , layout) << endl;
   ++indent;
   layout->DescribeTo(os,indent);
   --indent;

   os << StringPrintF("< WidgetDecoratorBase : Decorated widget %s at %p >" , decorated_widget?decorated_widget->GetName().c_str():"None" , decorated_widget);
   ++indent;
   decorated_widget?(void)decorated_widget->DescribeTo(os,indent):(void)0;
   --indent;
   
   return os;
}



































/**

#include "Eagle/Gui/Decorators/Decorator.hpp"










Decorator::Decorator() :
      WidgetBase(StringPrintF("Decorator layout at %p" , this)),
      dwidget(0),
      dwidget_layout(0),
      dumb_layout(StringPrintF("Decorator's dumb layout at %p" , &dumb_layout))
{
   UseLayout(&dumb_layout);
}



Decorator::Decorator(std::string name) :
      WidgetBase(name),
      dwidget(0),
      dwidget_layout(0),
      dumb_layout(StringPrintF("%s's dumb layout at %p" , name.c_str() , &dumb_layout))
{
   UseLayout(&dumb_layout);
}


   
int Decorator::PrivateHandleEvent(EagleEvent e) {
   int retmsg = DIALOG_OKAY;
   
   if (dwidget) {
      retmsg |= dwidget->PrivateHandleEvent(e);
   }
   if (!(retmsg & DIALOG_INPUT_USED)) {
      retmsg |= dwidget_layout->PrivateHandleEvent(e);
   }
   return retmsg;
}



int Decorator::PrivateCheckInputs() {
   int retmsg = DIALOG_OKAY;
   
   if (dwidget) {
      retmsg |= dwidget->PrivateCheckInputs();
   }
   if (!(retmsg & DIALOG_INPUT_USED)) {
      retmsg |= dwidget_layout->PrivateCheckInputs();
   }
   return retmsg;
}



void Decorator::PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos) {
   
   WidgetBase::Display(win,xpos,ypos);
   dwidget_layout->PrivateDisplay(win,xpos,ypos);
   if (dwidget) {
      dwidget->PrivateDisplay(win , xpos , ypos);
   }
   
}



int Decorator::PrivateUpdate(double tsec) {
   int retmsg = DIALOG_OKAY;
   
   retmsg |= dwidget_layout->PrivateUpdate(tsec);
   if (dwidget) {
      retmsg |= dwidget->PrivateUpdate(tsec);
   }
   
   return retmsg;
}



void Decorator::SetWidgetArea(int xpos , int ypos , int width , int height , bool notify_layout) {
   WidgetBase::SetWidgetArea(xpos,ypos,width,height,notify_layout);
   dwidget_layout->SetWidgetArea(InnerArea() , false);
}



void Decorator::SetMarginsExpandFromInner(int left , int right , int top , int bottom) {
   WidgetBase::SetMarginsExpandFromInner(left,right,top,bottom);
   dwidget_layout->SetWidgetArea(InnerArea() , false);
}



void Decorator::SetMarginsContractFromOuter(int left , int right , int top , int bottom) {
   WidgetBase::SetMarginsContractFromOuter(left,right,top,bottom);
   dwidget_layout->SetWidgetArea(InnerArea() , false);
}



void Decorator::UseLayoutForDecoratedWidget(Layout* layout) {
   
   if (dwidget_layout) {
      dwidget_layout->SetParent(0);
   }
   
   dwidget_layout->PlaceWidget(0 , 0);

   /// layout may be NULL to use the default DumbLayout
   if (!layout) {
      layout = &dumb_layout;
   }

   dwidget_layout = layout;
   
   dwidget_layout->SetWidgetArea(InnerArea() , false);

   dwidget_layout->SetParent(this);/// So QueueUserMessage works as expected
   
   dwidget_layout->Resize(1);

   dwidget_layout->PlaceWidget(dwidget , 0);/// dwidget may be NULL
}



void Decorator::DecorateWidget(WidgetBase* widget) {
   /// widget may be NULL to remove the decorated widget
   if (dwidget) {
      dwidget->SetParent(0);
   }
   dwidget = widget;
   if (dwidget) {
      dwidget->SetParent(this);/// So QueueUserMessage works as expected
   }
   dwidget_layout->PlaceWidget(dwidget , 0);/// If there is an old widget, it will be removed by PlaceWidget
   SetBgRedrawFlag();
}


//*/


