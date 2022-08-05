



#ifndef WidgetTree_HPP
#define WidgetTree_HPP



#include "Eagle/Gui.hpp"


class WNode : public SimpleTable {
   friend class WTreeView;
   friend class WTree;
   
protected :
   /// For the tree
   WidgetBase* widgetnode;
   bool open;
   int level;
   std::vector<WNode*> childnodes;
   
   /// For display and input
   IconButton openicon;
   BasicText  wtypetext;
   BasicText  wnametext;
   
   
   /// LayoutBase
   virtual void Resize(unsigned int nsize);
    
   void Clear();
   
public :   
   WNode();
   ~WNode();
   
   void Setup(EagleFont* font , WidgetBase* widget , int lvl = 0);
   
};
class WTreeView : protected LayoutBase {
   friend class WTree;

   
   WNode* root_node;
public :
   
/*   
   virtual Rectangle RequestWidgetArea(int widget_slot , int newx , int newy , int newwidth , int newheight);
Rectangle RequestWidgetArea(int widget_slot , int newx , int newy , int newwidth , int newheight) {
   (void)newx;
   (void)newy;
   (void)newwidth;
   (void)newheight;
   WidgetBase* w = GetWidget(widget_slot);
   if (!w) {return BADRECTANGLE;}
   int index = 0;
   WNode* currentnode = root_node;
   while (1) {
      if (currentnode->widgetnode == w) {
         break;
      }
      
   }
}
*/
};

class WTree {

protected :
   /// For the tree
   WNode root;
   
   /// For display and input
   WidgetHandler treegui;
   ScrollArea scrollarea;/// Root layout for the treegui
   WTreeView treeview;/// Main widget for the scrollarea and parent layout for all the WNodes
   
   virtual int PrivateHandleEvent(EagleEvent ee);
   virtual int PrivateCheckInputs();
   virtual int PrivateUpdate(double dt);
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);

   virtual void OnAreaChanged();
   virtual void OnAttributeChanged(const ATTRIBUTE& a , const VALUE& v);
   virtual void OnFlagChanged(WIDGET_FLAGS f , bool on);
   virtual void OnColorChanged();
   
public :


};

#endif // WidgetTree_HPP
