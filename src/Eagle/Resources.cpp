
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


#include "Eagle/Exception.hpp"
#include "Eagle/Resources.hpp"


const RESOURCEID BADRESOURCEID = ~0;



/// ---------------------      ResourceBase     ------------------------



ResourceBase::ResourceBase(const ResourceBase& r) :
   rid(BADRESOURCEID),
   rtype(RT_UNKNOWN)
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
   rtype(rt)
{}



