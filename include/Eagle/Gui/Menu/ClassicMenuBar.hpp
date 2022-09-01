
/**
 *
 *         _______       ___       ____      __       _______
 *        /\  ____\    /|   \     /  __\    /\ \     /\  ____\
 *        \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_
 *         \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\
 *          \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_
 *           \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *            \/______/|/_/  \/_/  \_\____/     \/_____/ \/______/
 *
 *
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2022+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file ClassicMenuBar.hpp
 * @brief The interface for a classic menu bar in Eagle
 */



#ifndef CLASSIC_MENU_BAR_HPP
#define CLASSIC_MENU_BAR_HPP

#include "Eagle/Gui/Layout/MenuLayout.hpp"



/**! @class ClassicMenuBar @
 *   @brief simple implentation of a classic menu bar
 */



class ClassicMenuBar : public ClassicMenuBarLayout , public EagleEventListener {
protected :
   std::vector<SIMPLE_MENU_BAR_ITEM> bitems;
   std::vector<ClassicMenuBarItem*> mbitems;
   ClassicMenuBarItem* citem;
   bool open;



   virtual void RespondToEvent(EagleEvent e , EagleThread* thread = MAIN_THREAD);



public :
   
   ClassicMenuBar();
   
   virtual ~ClassicMenuBar() {Clear();}
   
   void Clear();
   void SetBarItems(SIMPLE_MENU_BAR_ITEM* mbi , int nitems);
   
   virtual void OpenMe();
   virtual void CloseMe();
   virtual bool IsOpen();

   virtual void SetSubMenu(int index , MenuBase* smenu);
   
   void CloseOtherMenus(ClassicMenuBarItem* exclude);
   
};



#endif // CLASSIC_MENU_BAR_HPP
