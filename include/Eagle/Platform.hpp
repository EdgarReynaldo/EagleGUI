
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
 *    Copyright 2009-2016+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */


#ifndef Eagle_Platform_HPP
#define Eagle_Platform_HPP



#ifdef EAGLE_LIBRARY_BUILD

   /// Library build
   #if defined (_WIN32) && !defined (EAGLE_STATIC_LINK)
      #if !defined (EAGLE_STATIC_LINK)
         #define EAGLE_LINK_TYPE __declspec__(dllexport)
      #endif

   #elif def __APPLE__

   #elif def __unix__ || def __linux__

   #else
      #error "Platform not supported yet."
   #endif

#else /// !defined EAGLE_LIBRARY_BUILD - Not building library - user building app / dll

   /// User building app
   #ifdef WIN32
      #ifndef EAGLE_STATIC_LINK
         #define EAGLE_LINK_TYPE __declspec(dllimport)
      #endif

   #elif defined __APPLE__

   #elif defined __unix__ || defined __linux__

   #else

   #endif


#endif

/// Make sure EAGLE_LINK_TYPE is defined
#if !defined (EAGLE_LINK_TYPE)
   #define EAGLE_LINK_TYPE
#endif

#if !defined EAGLE_CALL_TYPE
   #define EAGLE_CALL_TYPE __stdcall__
#endif

/// Native path separator definitions
#if defined _WIN32
   #define EAGLE_NATIVE_PATH_SEP '\\'
   #define EAGLE_OTHER_PATH_SEP '/'
#elif defined __APPLE__ || defined __unix__ || defined __linux__
   #define EAGLE_NATIVE_PATH_SEP '/'
   #define EAGLE_OTHER_PATH_SEP '\\'
#else
   #warning "Using default path separator!"
   #define EAGLE_NATIVE_PATH_SEP '/'
   #define EAGLE_OTHER_PATH_SEP '\\'
#endif

/**
#if defined _MSC_VER
   #if !defined __FUNCSIG__
      #error "__FUNCSIG__ is not defined"
   #endif
   #define EAGLE__FUNC __FUNCSIG__
#else
   #include <cassert>
   #define EAGLE__FUNC __PRETTY_FUNCTION__
#endif //
//*/

#if defined __FUNCSIG__
   #define EAGLE__FUNC __FUNCSIG__
#else
   #define EAGLE__FUNC __PRETTY_FUNCTION__
#endif // defined



/// Eagle platform definitions
#if defined _WIN32
   #define EAGLE_WIN32
#elif defined __linux__
   #define EAGLE_LINUX
#elif defined __unix__
   #define EAGLE_UNIX
#elif defined __APPLE__
   #define EAGLE_APPLE
#else
    #error "Unknown platform detected"
#endif



#endif // Eagle_Platform_HPP





