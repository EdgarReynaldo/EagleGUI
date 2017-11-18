
/*
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
 *    EAGLE
 *    Edgar's Agile Gui Library and Extensions
 *
 *    Copyright 2009-2016+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */



#include "Eagle/Area.hpp"
#include "Eagle/System.hpp"
#include "Eagle/StringWork.hpp"

#include "Eagle/Gui/Layout/Layout.hpp"
#include "Eagle/Gui/WidgetBase.hpp"
#include "Eagle/Gui/WidgetHandler.hpp"
#include "Eagle/Gui/Decorators/Decorator.hpp"



using std::string;



const unsigned int TOPIC_DIALOG = NextFreeTopicId();

const UINT BASIC_FLAGS = ENABLED | VISIBLE;
const UINT DEFAULT_FLAGS = ENABLED | VISIBLE | MOVEABLE | RESIZEABLE;



/// ---------------------------       Globals      ----------------------------------



string PrintFlags(UINT flags) {
   static string flagstrings[NUM_WIDGET_FLAGS] = {
      string("ENABLED"),
      string("VISIBLE"),
      string("HOVER"),
      string("HASFOCUS"),
      string("MOVEABLE"),
      string("RESIZEABLE"),
      string("NEEDS_REDRAW"),
      string("NEEDS_BG_REDRAW"),
      string("ALLOW_CLOSE"),
      string("ALLOW_OVERLAP")
   };
   string retstr;
   if (flags == FLAGS_NONE) {
      return string("FLAGS_NONE");
   }
   for (int i = 0 ; i < NUM_WIDGET_FLAGS ; ++i) {
      if (flags & (WIDGET_FLAGS)(1 << i)) {
         retstr = retstr + flagstrings[i];
         retstr.push_back(' ');
      }
   }
   return retstr;
}


/// -----------------------      WidgetBase      -----------------------------------




bool WidgetBase::clip_widgets = true;


bool WidgetBase::display_widget_area = false;


void WidgetBase::RaiseEvent(WidgetMsg msg) {
   // signal your slots
   PrivateRaiseEvent(msg);
   
   // send event to listeners
   EagleEvent e = MakeEagleEvent(msg);
   EmitEvent(e , 0);/// TODO : FIXME : We should fix this so it takes a thread*
   
   // tell our parent
   WidgetBase::QueueUserMessage(msg);
}





/*
   
   WidgetBase* wparent;
   Layout* layout;
   
   WidgetColorset wcols;
   WidgetColorset privwcols;
   bool use_private_colorset;

   WidgetArea area;
   int minw;
   int minh;
   
   UINT flags;
   
   int display_priority;
   
   BackgroundPainter* background_painter;
   BG_DRAW_TYPE background_draw_type;
   
   FocusPainter* focus_painter;
   FOCUS_DRAW_TYPE focus_draw_type;

*/
WidgetBase::WidgetBase(std::string objclass , std::string objname) :
      EagleObject(objclass , objname),
      EagleEventSource(),
      wparent(0),
      layout(0),
      wcols(),
      privwcols(),
      use_private_colorset(false),
      area(),
      minw(1),
      minh(1),
      flags(DEFAULT_FLAGS),
      display_priority(MID_DISPLAY_PRIORITY),
      background_painter(&default_background_painter),
      background_draw_type(BG_DRAW_BACKGROUND_EMPTY),
      focus_painter(&default_focus_painter),
      focus_draw_type(FOCUS_DRAW_HIGHLIGHT_OUTLINE),
      decorator_parent(0)
{
   
}



WidgetBase::~WidgetBase() {
   /// In case our widget goes out of scope before our layout does.
   if (layout) {
      layout->RemoveWidget(this);/// Don't call RemoveWidgetFromLayout, as that could call our destructor again in an endless loop
   }
/**
   WidgetBase* wparent;
   Layout* layout;
   
   WidgetColorset wcols;
   WidgetColorset privwcols;
   bool use_private_colorset;

   WidgetArea area;
   int minw;
   int minh;
   
   UINT flags;
   
   int display_priority;
   
   WIDGET_DRAW_FUNC bg_draw_func;
*/
   wparent = 0;
   layout = 0;
}



//void WidgetBase::ReadEvents() {
//   (void)0;
//   return;
//}


/*
void WidgetBase::RespondToEvent(EagleEvent e) {

   if (HasListeners()) {
      EmitEvent(e);
   }
   else if (wparent) {
      wparent->RespondToEvent(e);
   }
}
*/


int WidgetBase::PrivateHandleEvent(EagleEvent e) {
   (void)e;
   return DIALOG_OKAY;
}



void WidgetBase::PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos) {
   (void)win;
   (void)xpos;
   (void)ypos;
}



int WidgetBase::PrivateCheckInputs() {
   return DIALOG_OKAY;
}



int WidgetBase::PrivateUpdate(double tsec) {
   (void)tsec;
   return DIALOG_OKAY;
}



void WidgetBase::ClipWidgets(bool clip) {
   clip_widgets = clip;
}



void WidgetBase::DisplayWidgetArea(bool display) {
   display_widget_area = display;
}



int WidgetBase::HandleEvent(EagleEvent e) {
   if (!(Flags() & ENABLED)) {
      return DIALOG_DISABLED;
   }
   
   int ret = PrivateHandleEvent(e);
   
   /// TODO : Do we really want to call CheckInput every time we get an event? 
   /// TODO : It makes BUTTON_HELD messages spam like crazy when the mouse drags over a button - maybe that's a problem with Button class
   /// TODO : FIX : Every non timer event is passed to check inputs
   if (e.type != EAGLE_EVENT_TIMER) {
      if (!(ret & DIALOG_INPUT_USED)) {
         ret = ret | PrivateCheckInputs();
      }
   }
         
   return ret;
}



void WidgetBase::Display(EagleGraphicsContext* win , int xpos , int ypos) {
   if (!(Flags() & VISIBLE)) {
      return;
   }
   
   EagleImage* dest = win->GetDrawingTarget();
   EAGLE_ASSERT(dest);
   if (clip_widgets) {
      dest->PushClippingRectangle(area.OuterArea());
   }
   
   background_painter->PaintBackground(win , this , xpos , ypos , background_draw_type);
         
   PrivateDisplay(win , xpos , ypos);

   if (flags & HASFOCUS) {
      focus_painter->PaintFocus(win , this , xpos , ypos , focus_draw_type);
   }
   
   if (clip_widgets) {
      dest->PopClippingRectangle();
   }
   
   if (display_widget_area) {
      win->DrawRectangle(OuterArea() , 1.0 , EagleColor(255,255,255));
      win->DrawRectangle(InnerArea() , 1.0 , EagleColor(0,255,0));
   }
   
   ClearRedrawFlag();
}



int WidgetBase::Update(double tsec) {
   if (!(Flags() & ENABLED)) {
      return DIALOG_DISABLED;
   }
   
   return PrivateUpdate(tsec);
}



void WidgetBase::QueueUserMessage(const WidgetMsg& wmsg) {
   if (wparent) {
      wparent->QueueUserMessage(wmsg);
   }
}



void WidgetBase::QueueUserMessage(WidgetBase* widget_address , UINT widget_topic , int messages) {
   QueueUserMessage(WidgetMsg(widget_address , widget_topic , messages));
}



void WidgetBase::SetParent(WidgetBase* parent) {
   wparent = parent;
}



void WidgetBase::SetDecoratorParent(WidgetDecorator* decorator) {
   decorator_parent = decorator;
}



void WidgetBase::SetOwnerLayout(Layout* l) {
   layout = l;
}



void WidgetBase::SetBackgroundPainter(BackgroundPainter* painter) {
   if (!painter) {
      painter = &default_background_painter;
   }
   background_painter = painter;
   SetBgRedrawFlag();
}



void WidgetBase::SetFocusPainter(FocusPainter* painter) {
   if (!painter) {
      painter = &default_focus_painter;
   }
   focus_painter = painter;
   SetBgRedrawFlag();
}



void WidgetBase::SetBackgroundDrawType(BG_DRAW_TYPE draw_type) {
   background_draw_type = draw_type;
   SetBgRedrawFlag();
}



void WidgetBase::SetFocusDrawType(FOCUS_DRAW_TYPE draw_type) {
   focus_draw_type = draw_type;
   if (Flags() & HASFOCUS) {
      SetBgRedrawFlag();
   }
}



void WidgetBase::SetColorset(const WidgetColorset& colors , bool set_descendants_colors) {
   (void)set_descendants_colors;
   wcols = colors;
   if (!use_private_colorset) {
      SetRedrawFlag();
   }
}



void WidgetBase::SetPrivateColorset(const WidgetColorset& colors) {
   privwcols = colors;
   if (use_private_colorset) {
      SetRedrawFlag();
   }
}



void WidgetBase::UseColorset(bool use_public_colorset) {
   use_private_colorset = !use_public_colorset;
   SetRedrawFlag();
}



void WidgetBase::UsePrivateColorset(bool use_priv_colorset) {
   use_private_colorset = use_priv_colorset;
   SetRedrawFlag();
}



void WidgetBase::SetFlagStates(UINT FLAGS , bool state) {
   
   /** FLAGS contains the flags that you wish to change state, and state is whether they will be
   set to true or false.
   */
   
   UINT newflags = (
      state?
      // Set these flags to true
      (flags | FLAGS):
      // Set these flags to false
      (flags & ~FLAGS)
   );
   
   // Which flags changed state?
   UINT changed = flags^newflags;
   // Which state did they change to?
   if (changed) {
      if (changed & ENABLED)       {SetEnabledState     (newflags & ENABLED      );}
      if (changed & VISIBLE)       {SetVisibilityState  (newflags & VISIBLE      );}
      if (changed & HOVER)         {SetHoverState       (newflags & HOVER        );}
      if (changed & HASFOCUS)      {SetFocusState       (newflags & HASFOCUS     );}
      if (changed & MOVEABLE)      {SetMoveableState    (newflags & MOVEABLE     );}
      if (changed & RESIZEABLE)    {SetResizeableState  (newflags & RESIZEABLE   );}
      if (changed & NEEDS_REDRAW)  {SetNeedsRedrawState (newflags & NEEDS_REDRAW );}
      if (changed & ALLOW_CLOSE)   {SetAllowCloseState  (newflags & ALLOW_CLOSE  );}
      if (changed & ALLOW_OVERLAP) {SetAllowOverlapState(newflags & ALLOW_OVERLAP);}
   }
}



void WidgetBase::SetEnabledState(bool state) {
   const UINT FLAG = ENABLED;
   if (state != bool(flags & FLAG)) {
      flags ^= FLAG;
      SetBgRedrawFlag();
   }
}



void WidgetBase::SetHoverState(bool state) {
   const UINT FLAG = HOVER;
   if (state != bool(flags & FLAG)) {
      flags ^= FLAG;
/*
      SetRedrawFlag();
      if (!state) {
         SetBgRedrawFlag();
      }
//*/
   }
}



void WidgetBase::SetVisibilityState(bool state) {
   const UINT FLAG = VISIBLE;
   if (state != bool(flags & FLAG)) {
      flags ^= FLAG;
      if (!state) {
         WidgetHandler* wh = NearestParentGui();
         if (wh) {wh->MakeAreaDirty(OuterArea());}
      } else {
         SetRedrawFlag();
      }
//      SetBgRedrawFlag();
   }
}



void WidgetBase::SetFocusState(bool state) {
   const UINT FLAG = HASFOCUS;
   if (state != bool(flags & FLAG)) {
      flags ^= FLAG;
      SetRedrawFlag();
      if (!state) {
         SetBgRedrawFlag();
      }
   }
}



void WidgetBase::SetMoveableState(bool state) {
   const UINT FLAG = MOVEABLE;
   if (state != bool(flags & FLAG)) {
      flags ^= FLAG;
   }
}



void WidgetBase::SetResizeableState(bool state) {
   const UINT FLAG = RESIZEABLE;
   if (state != bool(flags & FLAG)) {
      flags ^= FLAG;
   }
}



void WidgetBase::SetNeedsRedrawState(bool state) {
   const UINT FLAG = NEEDS_REDRAW;
   if (state != bool(flags & FLAG)) {
      flags ^= FLAG;
      if (state) {
         if (wparent) {wparent->SetRedrawFlag();}
      }
   }
}



void WidgetBase::SetNeedsBgRedrawState(bool state) {
   const UINT FLAG = NEEDS_BG_REDRAW;
   if (state != bool(flags & FLAG)) {
      flags ^= FLAG;
      if (state) {
         WidgetBase::SetNeedsRedrawState(true);
         WidgetHandler* pwh = dynamic_cast<WidgetHandler*>(wparent);
         if (wparent) {
            if (pwh) {
               pwh->MakeAreaDirty(OuterArea());
            } else {
               wparent->SetNeedsBgRedrawState(true);
            }
         }
      }
   }
}


void WidgetBase::SetAllowCloseState(bool state) {
   const UINT FLAG = ALLOW_CLOSE;
   if (state != bool(flags & FLAG)) {
      flags ^= FLAG;
   }
}



void WidgetBase::SetAllowOverlapState(bool state) {
   const UINT FLAG = ALLOW_OVERLAP;
   if (state != bool(flags & FLAG)) {
      flags ^= FLAG;
   }
}



void WidgetBase::SetRedrawFlag() {
   if (area.HasImageAlpha()) {
      SetNeedsBgRedrawState(true);
   }
   else {
      SetNeedsRedrawState(true);
   }
}



void WidgetBase::SetBgRedrawFlag() {
   SetNeedsBgRedrawState(true);
}



void WidgetBase::ClearRedrawFlag() {
   SetNeedsRedrawState(false);
   SetNeedsBgRedrawState(false);
}



void WidgetBase::SetRedrawAllFlag() {
   if (wparent) {wparent->SetRedrawAllFlag();}
}



void WidgetBase::ShowWidget() {
   SetEnabledState(true);
   SetVisibilityState(true);
}



void WidgetBase::HideWidget() {
   SetEnabledState(false);
   SetVisibilityState(false);
}



void WidgetBase::ToggleWidgetVisibility() {
   SetEnabledState(!(flags & ENABLED));
   SetVisibilityState(!(flags & VISIBLE));
}



bool WidgetBase::IsMouseOver(int realmsx , int realmsy) const {
	Pos2I offset = GetParentOffset();
	Rectangle realarea = area.OuterArea();
	realarea.MoveBy(offset);
	return realarea.Contains(realmsx,realmsy);
}



bool WidgetBase::GiveWidgetFocus(WidgetBase* widget , bool notify_parent) {
   if (wparent && notify_parent) {
      return wparent->GiveWidgetFocus(widget);
   }
   return false;
}



void WidgetBase::SetBgImage(EagleImage* img , MARGIN_HCELL hcell , MARGIN_VCELL vcell) {
   area.SetImage(img , hcell , vcell);
}



void  WidgetBase::SetBgImages(EagleImage* imgs[3][3]) {
   area.SetImages(imgs);
}


void WidgetBase::WidgetBase::SetImagesHaveAlpha(bool have_alpha) {
   area.SetImagesHaveAlpha(have_alpha);
}



void WidgetBase::SetWidgetPos(int xpos , int ypos , bool notify_layout) {
   SetWidgetArea(xpos , ypos , INT_MAX , INT_MAX , notify_layout);
}



void WidgetBase::SetWidgetCorners(int x1 , int y1 , int x2 , int y2 , bool notify_layout) {
   int lx = (x1<x2)?x1:x2;
   int rx = (x1>x2)?x1:x2;
   int ty = (y1<y2)?y1:y2;
   int by = (y1>y2)?y1:y2;
   SetWidgetArea(lx , ty , (rx - lx) + 1 , (by - ty) + 1 , notify_layout);
}



void WidgetBase::SetWidgetDimensions(int width , int height , bool notify_layout) {
   SetWidgetArea(INT_MAX , INT_MAX , width , height , notify_layout);
}



void WidgetBase::SetWidgetArea(Rectangle outer , bool notify_layout) {
   SetWidgetArea(outer.X() , outer.Y() , outer.W() , outer.H() , notify_layout);
}



void WidgetBase::SetWidgetArea(int xpos , int ypos , int width , int height , bool notify_layout) {

   int realminwidth = minw + area.MLeft() + area.MRight();
   int realminheight= minh + area.MTop() + area.MBot();

	if (width < realminwidth) {
      width = realminwidth;
	}
	if (height < realminheight) {
		height = realminheight;
	}
   
   Rectangle r = OuterArea();
   
   if (!layout || !notify_layout) {
      if (xpos == INT_MAX) {
         xpos = r.X();
      }
      if (ypos == INT_MAX) {
         ypos = r.Y();
      }
      if (width == INT_MAX) {
         width = r.W();
      }
      if (height == INT_MAX) {
         height = r.H();
      }
   }
   
   
   if (layout && notify_layout) {
      if (flags & VISIBLE) {
         WidgetHandler* pwh = dynamic_cast<WidgetHandler*>(wparent);
         if (pwh) {pwh->MakeAreaDirty(area.OuterArea());}
      }
      area.SetOuterArea(layout->RequestWidgetArea(this , xpos , ypos , width , height));
      SetBgRedrawFlag();
   }
   else {
      if (flags & MOVEABLE) {
         r.SetPos(xpos,ypos);
      }
      if (flags & RESIZEABLE) {
         r.SetDimensions(width,height);
      }
      if (flags & VISIBLE) {
         WidgetHandler* pwh = dynamic_cast<WidgetHandler*>(wparent);
         if (pwh) {pwh->MakeAreaDirty(area.OuterArea());}
      }
      area.SetOuterArea(r);
      if (flags & VISIBLE) {
         SetBgRedrawFlag();
      }
   }
   
   /// Inner area needs to be refreshed
   area.SetMarginsContractFromOuter(area.MLeft() , area.MRight() , area.MTop() , area.MBot());
}








void WidgetBase::SetMarginsExpandFromInner(int left , int right , int top , int bottom) {
   
   EAGLE_ASSERT(left >= 0);
   EAGLE_ASSERT(right >= 0);
   EAGLE_ASSERT(top >= 0);
   EAGLE_ASSERT(bottom >= 0);
   
   Rectangle outer = area.InnerArea();
   
   /// Get position of new outer area
   outer.MoveBy(-left , -top);
   
   /// Expand outer area to include new margins
   outer.SetDimensions(left + area.InnerArea().W() + right , top + area.InnerArea().H() + bottom);
   
   /// Attempt to change position to new area
   SetWidgetArea(outer);
   
   /// Attempt to set the margins
   SetMarginsContractFromOuter(left,right,top,bottom);

}



void WidgetBase::SetMarginsContractFromOuter(int left , int right , int top , int bottom) {

   EAGLE_ASSERT(left >= 0);
   EAGLE_ASSERT(right >= 0);
   EAGLE_ASSERT(top >= 0);
   EAGLE_ASSERT(bottom >= 0);

   area.SetMarginsContractFromOuter(left,right,top,bottom);/// No need to notify layout, outer position doesn't change
   SetBgRedrawFlag();
}



void WidgetBase::SetMinInnerWidth(int w) {

   const int amw = AbsMinWidth();
   if (w < amw) {w = amw;}
   minw = w;
   
   if (minw > area.InnerArea().W()) {
      
      area.SetInnerDimensions(minw , area.InnerArea().H());

      SetWidgetArea(area.OuterArea());/// Verify position with layout
   }
}



void WidgetBase::SetMinInnerHeight(int h) {

   const int amh = AbsMinHeight();
   if (h < amh) {h = amh;}
   minh = h;

   if (minh > area.InnerArea().H()) {
      
      area.SetInnerDimensions(area.InnerArea().W() , minh);

      SetWidgetArea(area.OuterArea());/// Verify position with layout
   }
}



void WidgetBase::SetMinInnerDimensions(int w , int h) {
   const int amw = AbsMinWidth();
   const int amh = AbsMinHeight();
   if (w < amw) {w = amw;}
   if (h < amh) {h = amh;}
   minw = w;
   minh = h;

   if ((minw > area.InnerArea().W()) || (minh > area.InnerArea().H())) {

      area.SetInnerDimensions(minw , minh);

      SetWidgetArea(area.OuterArea());/// Verify position with layout
   }

}



void WidgetBase::SetDisplayPriority(int priority) {
   if (priority < 0) {priority = 0;}
   if (priority > 255) {priority = 255;}
   display_priority = priority;
   SetBgRedrawFlag();
}



WidgetHandler* WidgetBase::NearestParentGui() {
   WidgetBase* p = wparent;
   while (p) {
      WidgetHandler* pwh = dynamic_cast<WidgetHandler*>(p);
      if (pwh) {return pwh;}
      p = p->wparent;
   }
   return 0;
}



WidgetBase* WidgetBase::Root() {
	if (!wparent) {return this;}
   WidgetBase* p = wparent;
   while (p->wparent) {
      p = p->wparent;
   }
   return p;
}



WidgetHandler* WidgetBase::RootGui() {
   return dynamic_cast<WidgetHandler*>(Root());
}



int WidgetBase::AbsParentX() const {
   int px = 0;
   WidgetBase* parent = wparent;
   while (parent) {
      /// TODO : FIXME? Outer area / inner area?
      px += parent->InnerArea().X();
      parent = parent->wparent;
   }
   return px;
}



int WidgetBase::AbsParentY() const {
   int py = 0;
   WidgetBase* parent = wparent;
   while (parent) {
      py += parent->InnerArea().Y();
      parent = parent->wparent;
   }
   return py;
}



Pos2I WidgetBase::GetParentOffset() const {
   return Pos2I(AbsParentX() , AbsParentY());
}



WidgetColorset& WidgetBase::WCols() {
   if (use_private_colorset) {return privwcols;}
   return wcols;
}



const WidgetColorset& WidgetBase::WCols() const {
   if (use_private_colorset) {return privwcols;}
   return wcols;
}



WidgetBase* WidgetBase::GetDecoratorRoot() {
   
   WidgetBase* root = this;
   while (root->GetDecoratorParent()) {
      root = root->GetDecoratorParent();
   }
   return root;
}


/**
const WidgetBase* WidgetBase::GetDecoratorRoot() const {
   
   const WidgetBase* root = this;
   while (root->GetDecoratorParent()) {
      root = root->GetDecoratorParent();
   }
   return root;
}
*/



/// Right now only NAME , POS, DIM, and AREA are supported
void WidgetBase::SetAttributes(AttributeValueMap avmap) {

    ATTRIBUTE_VALUE_MAP attribute_map = avmap.GetMap();

   WidgetBase* widget = this;
   
   widget = widget->GetDecoratorRoot();
   
   std::map<std::string , std::string>::const_iterator cit = attribute_map.end();
   
   const char* cstr = 0;
      
   if ((cit = attribute_map.find("NAME")) != attribute_map.end()) {
      widget->SetShortName(cit->second);
   }

   if ((cit = attribute_map.find("POS")) != attribute_map.end()) {
      cstr = cit->second.c_str();
      int x = 0 , y = 0;
      if (2 != sscanf(cstr , "%d,%d" , &x , &y)) {
         throw EagleException(StringPrintF("ApplyWidgetBaseAtributes:: (POS) Failed to read x,y pair from (%s)\n" , cstr));
      }
      widget->SetWidgetPos(x,y);
   }
   if ((cit = attribute_map.find("DIM")) != attribute_map.end()) {
      cstr = cit->second.c_str();
      int w = 0 , h = 0;
      if (2 != sscanf(cstr , "%d,%d" , &w , &h)) {
         throw EagleException(StringPrintF("ApplyWidgetBaseAtributes:: (DIM) Failed to read w,h pair from (%s)\n" , cstr));
      }
      int x = widget->Area().OuterArea().X();
      int y = widget->Area().OuterArea().Y();
      if (w < 0 || h < 0) {
         if (w < 0) {
            w = abs(w);
            x = x - w;
         }
         if (h < 0) {
            h = abs(h);
            y = y - h;
         }
         widget->SetWidgetPos(x,y);
      }
      widget->SetWidgetDimensions(w,h);
   }
   if ((cit = attribute_map.find("AREA")) != attribute_map.end()) {
      cstr = cit->second.c_str();
      int x = 0 , y = 0 , w = 0 , h = 0;
      if (4 != sscanf(cstr , "%d,%d,%d,%d" , &x , &y , &w , &h)) {
         throw EagleException(StringPrintF("ApplyWidgetBaseAtributes:: (AREA) Failed to read x,y,w,h set from (%s)\n" , cstr));
      }
      widget->SetWidgetArea(x,y,w,h);
   }
   
}





std::ostream& WidgetBase::DescribeTo(std::ostream& os , Indenter indent) const {
   using std::endl;
   os << indent << "WidgetBase Info : " << endl;
   os << indent << StringPrintF("WidgetBase object %s" , FullName()) << endl;
   ++indent;
   os << indent << StringPrintF("Widget parent (%p:(%s)) , layout (%p:(%s))" ,
                                wparent , wparent?wparent->FullName():"NULL", layout , layout?layout->FullName():"NULL") << endl;
   area.DescribeTo(os , indent);
   os << indent << StringPrintF("Min WxH = %i x %i" , minw , minh) << endl;
   os << indent << PrintFlags(flags);
   os << endl;
   os << indent << StringPrintF("Display priority = %i" , display_priority) << endl;
   os << indent << StringPrintF("Use private colorset = %s" , use_private_colorset?"true":"false") << endl;
   os << indent << "wcols : " << endl;
   wcols.DescribeTo(os , indent);
   os << indent << "privwcols :" << endl;
   privwcols.DescribeTo(os , indent);
   
   os << indent << "BgDrawType is " << BgDrawTypeString(background_draw_type) << " , FocusDrawType is " << FocusDrawTypeString(focus_draw_type) << endl;
   os << indent << "BackgroundPainter class is " << background_painter->GetPainter(background_draw_type)->GetPainterName() << endl;
   os << indent << "FocusPainter class is " << focus_painter->GetPainter(focus_draw_type)->GetPainterName() << endl;
   
   --indent;
   return os;
}




/// ---------------------------      Global functions     -------------------------------




EagleEvent MakeEagleEvent(WidgetMsg msg) {
   EagleEvent e;
   e.source = msg.From();
   e.timestamp = EagleSystem::GetProgramTime();
   e.type = EAGLE_EVENT_WIDGET;
   e.widget.from = msg.from;
   e.widget.topic = msg.topic;
   e.widget.msgs = msg.msgs;
   return e;
}



bool DrawPriorityIsLess(WidgetBase* lhs , WidgetBase* rhs) {
   return lhs->DisplayPriority()<rhs->DisplayPriority();
}



void PrintWidget(WidgetBase* widget) {
   EagleInfo() << *widget << std::endl;
}



