
/**
 *
 *         _______       ___       ____      __       _______
 *        /\  ____\    /|   \     /  __\    /\ \     /\  ____\
 *        \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_
 *         \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\
 *          \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_
 *           \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *            \/______/|/_/  \/_/  \_\_____/    \/_____/ \/______/
 *
 *
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2019+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file MousePointer.hpp
 * @brief The main interface for working with mouse pointers in Eagle
 */

#ifndef MousePointer_HPP
#define MousePointer_HPP



#include <list>

/** From include/allegro5/mouse_cursor.h

typedef enum ALLEGRO_SYSTEM_MOUSE_CURSOR
{
   ALLEGRO_SYSTEM_MOUSE_CURSOR_NONE        =  0,
   ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT     =  1,
   ALLEGRO_SYSTEM_MOUSE_CURSOR_ARROW       =  2,
   ALLEGRO_SYSTEM_MOUSE_CURSOR_BUSY        =  3,
   ALLEGRO_SYSTEM_MOUSE_CURSOR_QUESTION    =  4,
   ALLEGRO_SYSTEM_MOUSE_CURSOR_EDIT        =  5,
   ALLEGRO_SYSTEM_MOUSE_CURSOR_MOVE        =  6,
   ALLEGRO_SYSTEM_MOUSE_CURSOR_RESIZE_N    =  7,
   ALLEGRO_SYSTEM_MOUSE_CURSOR_RESIZE_W    =  8,
   ALLEGRO_SYSTEM_MOUSE_CURSOR_RESIZE_S    =  9,
   ALLEGRO_SYSTEM_MOUSE_CURSOR_RESIZE_E    = 10,
   ALLEGRO_SYSTEM_MOUSE_CURSOR_RESIZE_NW   = 11,
   ALLEGRO_SYSTEM_MOUSE_CURSOR_RESIZE_SW   = 12,
   ALLEGRO_SYSTEM_MOUSE_CURSOR_RESIZE_SE   = 13,
   ALLEGRO_SYSTEM_MOUSE_CURSOR_RESIZE_NE   = 14,
   ALLEGRO_SYSTEM_MOUSE_CURSOR_PROGRESS    = 15,
   ALLEGRO_SYSTEM_MOUSE_CURSOR_PRECISION   = 16,
   ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK        = 17,
   ALLEGRO_SYSTEM_MOUSE_CURSOR_ALT_SELECT  = 18,
   ALLEGRO_SYSTEM_MOUSE_CURSOR_UNAVAILABLE = 19,
   ALLEGRO_NUM_SYSTEM_MOUSE_CURSORS
} ALLEGRO_SYSTEM_MOUSE_CURSOR;

*/


/**! @enum MOUSE_POINTER_TYPE
 *   @brief An enumeration to define which mouse pointer is in use
 */

enum MOUSE_POINTER_TYPE {
   POINTER_NORMAL   = 0,///< Default mouse pointer
   POINTER_ARROW    = 1,///< Arrow mouse pointer
   POINTER_BUSY     = 2,///< Busy mouse pointer
   POINTER_QUESTION = 3,///< Question mouse pointer
   POINTER_EDIT     = 4,///< Editing mouse pointer
   POINTER_GRAB     = 5,///< Grab mouse pointer
   POINTER_GRABBED  = 6,///< Grabbed mouse pointer

   POINTER_NWSE     = 7,///< Upper left and bottom right corner drag mouse pointer
   POINTER_NW       = 7,///< Same as @ref POINTER_NWSE
   POINTER_SE       = 7,///< Same as @ref POINTER_NWSE

   POINTER_NS       = 8,///< Upper edge and bottom edge drag mouse pointer
   POINTER_N        = 8,///< Same as @ref POINTER_NS
   POINTER_S        = 8,///< Same as @ref POINTER_NS

   POINTER_NESW     = 9,///< Upper right and lower left corner drag mouse pointer
   POINTER_NE       = 9,///< Same as @ref POINTER_NESW
   POINTER_SW       = 9,///< Same as @ref POINTER_NESW

   POINTER_EW       = 10,///< Right and left edge drag mouse pointer
   POINTER_E        = 10,///< Same as @ref POINTER_EW
   POINTER_W        = 10,///< Same as @ref POINTER_EW

   POINTER_CUSTOM   = 11,///< Custom mouse pointer
   NUM_POINTERS     = 12 ///< Number of pointers available in Eagle
};


extern const char* pointer_names[NUM_POINTERS];///< An array of string names for each pointer type



class EagleImage;



/**! @class MousePointerInfo
 *   @brief A data class to hold a mouse pointer image, type, and alignment with the cursor
 */

class MousePointerInfo {
public :

   MousePointerInfo();///< Empty constructor?
   
   /**! @fn MousePointerInfo::MousePointerInfo <MOUSE_POINTER_TYPE , EagleImage* , int , int , bool>
    *   @brief Constructor to create a mouse pointer object from a type, an image, focus x and y, and specify if this is a system pointer
    */
   
   MousePointerInfo(MOUSE_POINTER_TYPE ptype , EagleImage* img , int fx , int fy , bool is_system_pointer = false);

   MOUSE_POINTER_TYPE pointer_type;///< The type of pointer in use
   bool system_pointer_in_use;///< Whether this is a system pointer
   
   EagleImage* image;///< The image to use for the pointer
   
   /**! @var alignx @var aligny
    *   @brief The mouse pointer image will be drawn at mouse_x - align_x , mouse_y - align_y
    */
   int alignx;///< The x alignment for the image in relation to the cursor
   int aligny;///< The y alignment for the image in relation to the cursor
};


/**! @class MousePointer
 *   @brief Abstract base class for all mouse pointers
 */

class MousePointer {
protected :
   MousePointerInfo mpinfo;///< A mouse pointer info object
   
public :
   MousePointer();///< Empty default constructor
   virtual ~MousePointer() {}///< Virtual destructor
   
   MousePointerInfo GetInfo();///< Get the MousePointerInfo associated with this MousePointer

   virtual bool Create(MousePointerInfo info)=0;///< Pure virtual function to create a mouse pointer given the specified @var info
   virtual void Free()=0;///< Pure virtual function to free this mouse pointer's data
   virtual bool Valid()=0;///< Pure virtual function to test validity of this object and its readiness
};


/**! @class SystemMousePointer
 *   @brief A system mouse pointer
 */

class SystemMousePointer : public MousePointer {
   
public :
   SystemMousePointer(MOUSE_POINTER_TYPE type);///< Create a system mouse pointer from the specified pointer type
   
   virtual bool Create(MousePointerInfo info);///< Create a system pointer
   virtual void Free();///< Free a system pointer
   virtual bool Valid();///< Check if a system pointer is valid
};


/**! @class MousePointerSet
 *   @brief A set of mouse pointers
 */

class MousePointerSet {
private :
   MousePointer* pointer_set[NUM_POINTERS];///< An array of MousePointer pointers
   
public :
   MousePointerSet();///< Default empty constructor
   ~MousePointerSet();///< Destructor
   void Free();///< Free this set

   /**! @fn MousePointerSet::operator[]
    *   @brief Get the mouse pointer for the specified type.
    *   @retval Not guaranteed to return non-null, check before using
    */
   MousePointer*& operator[](MOUSE_POINTER_TYPE type);

};



class EagleGraphicsContext;



/**! @class MousePointerManager
 *   @brief An abstract base class for managing mouse pointers
 */

class MousePointerManager {
   
protected :

   EagleGraphicsContext* display;///< Our display
   
   MousePointerSet custom_pointer_set;///< A custom pointer set
   MousePointerSet system_pointer_set;///< The system pointer set
   
   MousePointerSet* pointer_set_in_use;///< A pointer to the pointer set in use
   
   MousePointer** current_pointer;/// The address of the MousePointer* in use for making changes
   

   std::list< std::pair<void* , MousePointer**> > mouse_owners;///< A map of callers to mouse pointers in use



   void SetupSystemPointers();///< Setup the system pointers

   MousePointerManager(EagleGraphicsContext* window);///< Constructor takes an EagleGraphicsContext* for use with images and display
   
   

   virtual MousePointer* CreateMousePointer(MousePointerInfo info)=0;///< Pure virtual function to create a MousePointer from mouse info

   virtual bool SelectMousePointerDirect(MousePointer** mp)=0;///< Pure virtual function to select a mouse pointer

public :
   
   virtual ~MousePointerManager();///< Public destructor

   void FreePointers();///< Free the pointer sets in use

   
   virtual void ShowMouse()=0;///< Pure virtual function to show the mouse
   virtual void HideMouse()=0;///< Pure virtual function to hide the mouse
   
   /**! @fn AcquireMousePointer <void* , MOUSE_POINTER_TYPE , bool>
    *   @brief Acquire the mouse pointer, using our address as an ownership token.
    *   @param caller Pass an address for the owning object.
    *   @param ptype The mouse pointer type to use
    *   @param use_system_pointer Whether or not to use a system pointer
    *   
    *   @retval True if the mouse cursor was acquired by the caller
    *   See also @ref ReleaseMousePointer
    */

   bool AcquireMousePointer(void* caller , MOUSE_POINTER_TYPE ptype , bool use_system_pointer);

   /**! @fn ReleaseMousePointer
    *   @brief Release the hold on the mouse pointer
    *   @param caller Pass the address you used for the caller in @ref AcquireMousePointer
    */
   
   void ReleaseMousePointer(const void* caller);


   /**! @fn ReAcquireMouse
    *   @brief Reacquire the mouse if the window loses focus
    *
    *   TODO : Automate this from the graphics context switch event
    */

   void ReAcquireMouse();// used when a window regains focus, to refresh the mouse, not called automatically
   
   /**! @fn SetCustomPointer
    *   @brief Set a custom mouse pointer to use in the custom mouse pointer set
    *   @retval True if successful
    */
   bool SetCustomPointer(MousePointerInfo info);

///   void SetCustomPointerSet(MousePointerSet* pointer_set);
   
   virtual void SetMousePosition(int mousex , int mousey)=0;///< Pure virtual function to set the mouse position on the display
};



#endif // MousePointer_HPP



