
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
 *    Copyright 2009-2013+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */



#ifndef EagleObject_HPP
#define EagleObject_HPP

#include "Eagle/Logging.hpp"

#include <string>
#include <map>
#include <vector>

#include <iostream>


class EagleObject;


#include <string>

void shutdown_object_module_function();

int register_object_shutdown_function();

int NextId();

//void RegisterObject(EagleObject* obj);
//void UnregisterObject(EagleObject* obj);
std::vector<EagleObject*> GetObjectsByName(std::string name);
void OutputObjects();
int ObjectCount();

EagleObject* GetObjectById(int eid);
bool GetValidById(int eid);
std::string GetNameById(int eid);

int GetIdByAddress(EagleObject* obj);

bool GetValidByAddress(EagleObject* obj);



class ShutdownVar {
   
   std::string str;
public :
   ShutdownVar(std::string name);
   ~ShutdownVar();

};

extern ShutdownVar global_shutdown_variable;



class EagleObject {
private :
   int id;
   std::string wname;
   bool destroyed;

public :
   
   EagleObject();
   EagleObject(std::string name);
   
   virtual ~EagleObject();


   
   std::string GetName() const;
   void SetName(std::string newname);
   
   int GetId();

   virtual std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const ;
//   virtual ostream& Describe(ostream& os);
};

std::ostream& operator<<(std::ostream& os , const EagleObject& obj);



#endif // EagleObject_HPP


