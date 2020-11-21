

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
 *    Copyright 2009-2021+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */



#ifndef WidgetFactory_HPP
#define WidgetFactory_HPP

class WidgetFactory;

extern WidgetFactory eagle_widget_factory;

///WidgetFactory eagle_widget_factory;



typedef WidgetBase* (*WIDGET_CREATION_FUNCTION)(string widget_class , string widget_parameters);


template <class WIDGET_CLASS_TYPE>
void WidgetCreator(WIDGET_CLASS_TYPE* pwidget_class_type , string widget_class_name

template <class WidgetBase>
void WidgetCreator(WidgetBase* pwidget_class_type , string widget_parameters);


class RegisteredWidgetCreator {
   
   
   
   RegisterWidgetCreator(string widget_class_name , string widget_parameters , WIDGET_CREATION_FUNCTION widget_creator);
   RegisterWidgetCreator(string widget_class_name , string widget_parameters , WIDGET_CREATION_FUNCTION widget_creator) {
      
   }
   int RegisterBasicWidgetCreator(string widget_class_name , WIDGET_CREATION_FUNCTION widget_creator);
   int RegisterTextWidgetCreator(string widget_class_name , WIDGET_CREATION_FUNCTION widget_creator);
   int RegisterButtonWidgetCreator(WIDGET_CREATION_FUNCTION widget_creator);
   
};



/// Subclass a robot and register it with the factory below

class WidgetFactoryRobot {

   map<int , WIDGET_CREATION_FUNCTION> widget_creator_map;

public :
   virtual WidgetBase* CreateWidget(int WIDGET_CLASS)=0;

   int RegisterWidgetCreator(WIDGET_CREATION_FUNCTION widget_creator);
   
   
};



class BasicWidgetFactoryRobot : public WidgetFactoryRobot {
   
   virtual WidgetBase* CreateWidget(int WIDGET_CLASS);/// Create all other kind of widgets here
};


extern BasicWidgetFactoryRobot basic_widget_robot;
/// BasicWidgetFactoryRobot basic_widget_robot;


WidgetBase* CreateExampleWidget() {return new ExampleWidget();}

extern int EXAMPLE_WIDGET_CLASS;
/// int EXAMPLE_WIDGET_CLASS = basic_widget_robot.RegisterWidgetCreator(CreateExampleWidget);



class ButtonWidgetFactoryRobot : public WidgetFactoryRobot {
   
   virtual BasicButton* CreateButton(int BUTTON_CLASS);
   /// IconButton* CreateIconButton();
   /// GuiButton* CreateGuiButton();
   /// HoverButton* CreateHoverButton();
};



class TextWidgetFactoryRobot : public WidgetFactoryRobot {
   
   virtual BasicText* CreateText(int TEXT_CLASS);
   /// DumbText* CreateDumbText(string text);
   /// SelectText CreateSelectText(string selecttext);
   /// LinkText* CreateLinkText(string linktext);
};


class LayoutWidgetFactoryRobot : public WidgetFactoryRobot {
   
   virtual Layout* CreateLayout(int LAYOUT_CLASS);
   /// GridLayout* CreateGridLayout(int nrows , int ncols);
   /// MenuLayout* CreateMenuLayout(int size , MENU_DIRECTION dir);
   /// FlowLayout* CreateFlowLayout();
   /// DumbLayout* CreateDumbLayout();
   /// PinLayout*  CreatePinLayout();
};



class WidgetFactory {
   
   map<WidgetBase* , bool> widget_memory_map;
   
   map<int , WidgetFactoryRobot*> factory_robot_map;

   void RegisterWidget(WidgetBase* widget);
   
public :
   
   WidgetFactory();
   WidgetFactory() :
         widget_memory_map(),
         factory_robot_map()
   {}
   ~WidgetFactory();
   ~WidgetFactory() {
      FreeAllWidgets();
   }
   
   void FreeWidget(WidgetBase* widget);
   void FreeAllWidgets();

   WidgetBase* CreateWidget(int ROBOT_CLASS , int WIDGET_CLASS);
   
   int RegisterRobot(WidgetFactoryRobot* robot);/// Returns the ROBOT_CLASS of this robot
};



#endif // WidgetFactory_HPP

