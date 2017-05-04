



#include "Eagle/Gui/Factory/WidgetCreators.hpp"
#include "Eagle/Gui/Factory/WidgetFactory.hpp"
#include "Eagle/StringWork.hpp"


WidgetFactory* eagle_widget_factory = 0;

WidgetFactory& EagleWidgetFactory() {
   static int create = 1;
   if (create) {
      eagle_widget_factory = new WidgetFactory;
      create = 0;
   }
   /** TODO : Destroy eagle_widget_factory */
   return *eagle_widget_factory;
}


const char* const DEFAULT_GROUP_NAME = "__DEFAULT_GROUP_NAME";


/// --------------------------------      WidgetFactory     --------------------------------------




void WidgetFactory::TrackWidget(EAGLE_ID eid , WIDGET_FREED_MAP& freed_map) {
   WIDGET_FREED_MAP::iterator it = freed_map.find(eid);
   if (it == freed_map.end()) {
      if (GetValidById(eid)) {
         freed_map[eid] = false;
      }
   }
}



void WidgetFactory::TrackWidget(EAGLE_ID eid , std::string group_name) {
   TrackWidget(eid , widget_freed_map);
   WIDGET_GROUP_MAP::iterator it = widget_group_map.find(group_name);
   if (it == widget_group_map.end()) {
      WIDGET_FREED_MAP freed_map;
      it = widget_group_map.insert(std::pair<string , WIDGET_FREED_MAP>(group_name , WIDGET_FREED_MAP())).first;
   }
   TrackWidget(eid , it->second);
}



bool WidgetFactory::FreeWidget(EAGLE_ID eid , WIDGET_FREED_MAP& freed_map) {
   WIDGET_FREED_MAP::iterator it = freed_map.find(eid);
   if (it != freed_map.end()) {
      if (GetValidById(eid) && !(it->second)) {
         delete GetObjectById(eid);
         return true;
      }
      it->second = true;
   }
   return false;
}



void WidgetFactory::RegisterWidgetCreationFunction(string widget_class_name , WIDGET_CREATION_FUNCTION widget_creator) {
   EAGLE_ASSERT(widget_creator);
   widget_maker_map[widget_class_name] = widget_creator;
}



WidgetBase* WidgetFactory::CreateWidgetBaseObject(string widget_class_name , string widget_parameters) {
   if (widget_maker_map.find(widget_class_name) == widget_maker_map.end()) {
      throw EagleException(StringPrintF("WidgetFactory::CreateWidgetBaseObject : Could not find a creation function registered for %s class widgets.\n",
                                    widget_class_name.c_str()));
   }
   WIDGET_CREATION_FUNCTION CreatorFunc = widget_maker_map[widget_class_name];
   if (!CreatorFunc) {
      throw EagleException(StringPrintF("WidgetFactory::CreateWidgetBaseObject : Creation function for %s class widgets is NULL!\n",
                       widget_class_name.c_str()));
   }
   WidgetBase* new_widget = CreatorFunc(widget_parameters);
   
   if (new_widget) {
         
   }
   
   return new_widget;
}



bool WidgetFactory::FreeWidget(EAGLE_ID eid) {

   WIDGET_GROUP_MAP::iterator default_group_it = widget_group_map.find(DEFAULT_GROUP_NAME);
   EAGLE_ASSERT(default_group_it != widget_group_map.end());
   
   WIDGET_FREED_MAP& default_group = default_group_it->second;

   WIDGET_FREED_MAP::iterator freed_map_it = default_group.find(eid);
   if (freed_map_it != default_group.end()) {
      /// Found eid in default group
      return FreeWidget(eid , default_group);
   }
   else {
      /// Need to iterate over groups and search for eid
      WIDGET_GROUP_MAP::iterator group_it = widget_group_map.begin();
      while (group_it != widget_group_map.end()) {
         if (group_it == default_group_it) {++group_it;continue;}
         WIDGET_FREED_MAP& freed_map = group_it->second;
         freed_map_it = freed_map.find(eid);
         if (freed_map_it != freed_map.end()) {
            /// Found eid in freed_map
            return FreeWidget(eid , freed_map);
         }
         ++group_it;
      }
   }
   return false;
}



bool WidgetFactory::FreeWidget(EAGLE_ID eid , std::string group_name) {
   WIDGET_GROUP_MAP::iterator it = widget_group_map.find(group_name);
   if (it != widget_group_map.end()) {
      return FreeWidget(eid , it->second);
   }
   return false;
}



bool WidgetFactory::FreeWidgetGroup(std::string group_name) {
   WIDGET_GROUP_MAP::iterator it = widget_group_map.find(group_name);
   if (it != widget_group_map.end()) {
      WIDGET_FREED_MAP& group = it->second;
      WIDGET_FREED_MAP::iterator freed_it = group.begin();
      while (freed_it != group.end()) {
         int eid = freed_it->first;
         if (GetValidById(eid) && !freed_it->second) {
            delete GetObjectById(eid);
         }
         freed_it->second = true;
         ++freed_it;
      }
      return true;
   }
   return false;
}



void WidgetFactory::FreeAllWidgets() {
   WIDGET_GROUP_MAP::iterator group_it = widget_group_map.begin();
   while (group_it != widget_group_map.end()) {
      FreeWidgetGroup(group_it->first);
      ++group_it;
   }
}



/// ------------------     Global functions     -----------------------------



bool FreeWidget(EAGLE_ID eid) {
   return EagleWidgetFactory().FreeWidget(eid);
}



bool FreeWidget(EAGLE_ID eid , std::string group_name) {
   return EagleWidgetFactory().FreeWidget(eid , group_name);
}



bool FreeWidgetGroup(std::string group_name) {
   return EagleWidgetFactory().FreeWidgetGroup(group_name);
}



void FreeAllWidgets() {
   EagleWidgetFactory().FreeAllWidgets();
}



WidgetBase* CreateWidgetObject(string widget_class_name , string widget_parameters) {
   return EagleWidgetFactory().CreateWidgetBaseObject(widget_class_name , widget_parameters);
}



