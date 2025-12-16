



#ifndef Scene_HPP
#define Scene_HPP

class Scene {
public :
   Scene() {}
   virtual int HandleEvent(EagleEvent e)=0;
   virtual void Display(EagleGraphicsContext* win)=0;
   virtual int Update(double dt)=0;
};

#endif // Scene_HPP
