



#include "WidgetTree.hpp"
#include "Eagle/Gui/WidgetHandler.hpp"



void WNode::Resize(unsigned int nsize) {
   nsize = 3;
   LayoutBase::Resize(nsize);
}



void WNode::Clear() {
   widgetnode = 0;
   open = false;
   for (unsigned int i = 0 ; i < childnodes.size() ; ++i) {
      WNode* wnode = childnodes[i];
      if (wnode) {
         delete wnode;
         wnode = 0;
      }
   }
   childnodes.clear();
}



WNode::WNode() :
      widgetnode(0),
      open(false),
      childnodes(),
      openicon(),
      wtypetext(),
      wnametext()
{
   SimpleTable::Resize(std::vector<float>({0.1f , 0.45f , 0.45f}) , std::vector<float>({1.0f}));
   PlaceWidget(&openicon , 0);
   PlaceWidget(&wtypetext , 0);
   PlaceWidget(&wnametext , 0);
}



WNode::~WNode() {
   Clear();
   DetachFromGui();
}



void WNode::Setup(EagleFont* font , WidgetBase* widget , int lvl) {
   Clear();
   if (!widget) {
      return;
   }
   EAGLE_ASSERT(font && font->Valid());
   level = lvl;
   WidgetHandler* pgui = dynamic_cast<WidgetHandler*>(widget);
   LayoutBase* playout = dynamic_cast<LayoutBase*>(widget);
   if (pgui) {
      playout = pgui->GetRootLayout();
   }
   if (playout) {
      widgetnode = widget;
      open = false;
      openicon.SetButtonState(false , false , false);
      wtypetext.SetupText(widget->ClassName() , font , HALIGN_LEFT , VALIGN_CENTER);
      wnametext.SetupText(widget->ShortName() , font , HALIGN_RIGHT , VALIGN_CENTER);
      std::vector<WidgetBase*> wchildren2 = playout->ChildrenVector();
      childnodes.resize(wchildren2.size() , (WNode*)0);
      for (unsigned int i = 0 ; i < wchildren.size() ; ++i) {
         WidgetBase* w = wchildren2[i];
         if (w) {
            childnodes[i] = new WNode();
            childnodes[i]->Setup(font , w , level + 1);
         }
      }
   }
}



/// -----------------------------     WTree     ---------------------------------------

/**

int WTree::PrivateHandleEvent(EagleEvent ee) {
   treegui.HandleEvent(ee);
}



int WTree::PrivateCheckInputs() {
   (void)0;
}



int WTree::PrivateUpdate(double dt) {
   treegui.Update(dt);
}



void WTree::PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos) {
   treegui.Display(win , xpos , ypos);
}



void WTree::OnAreaChanged() {
   treegui.SetWidgetArea(OuterArea() , false);
}



void WTree::OnAttributeChanged(const ATTRIBUTE& a , const VALUE& v) {
   treegui.SetAttribute(a , v);
}



void WTree::OnFlagChanged(WIDGET_FLAGS f , bool on) {
   treegui.SetWidgetFlags(Flags().SetNewFlag(f , on));
}



void WTree::OnColorChanged() {
   treegui.SetWidgetColorset(GetWidgetColorset());
}


*/

