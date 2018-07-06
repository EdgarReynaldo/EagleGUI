
/**
 *
 *     _______       ___       ____      __       _______       _______
 *    /\  ____\    /|   \     /  __\    /\ \     /\  ____\     /\  ____\
 *    \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_    \ \ \_____
 *     \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\    \ \_____ \
 *      \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_    \/ ____\ \
 *       \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\     /\______\
 *        \/______/|/_/  \/_/  \_\_____/    \/_____/ \/______/     \/______/
 *
 *
 *    EAGLE 5
 *    Edgar's Agile Gui Library and Extensions
 *
 *    Copyright 2009-2018+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */



#include "Eagle/Resources.hpp"


const RESOURCEID BADRESOURCEID = ~0;



/// ---------------------      ResourceBase     ------------------------



ResourceBase::ResourceBase(const ResourceBase& r) :
   rid(BADRESOURCEID),
   rtype(RT_UNKNOWN),
   resource(0)
{
   (void)r;
   throw EagleException("ResourceBase::ResourceBase - tried to copy a ResourceBase object.");
}



ResourceBase& ResourceBase::operator=(const ResourceBase& r) {
   (void)r;
   throw EagleException("Tried to assign ResourceBase object.");
   return *this;
}


ResourceBase::ResourceBase(RESOURCE_TYPE rt) :
   rid(NextRid()),
   rtype(rt),
   resource(0)
{}



