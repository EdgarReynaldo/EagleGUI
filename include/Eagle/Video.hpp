
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
 *    Copyright 2009-2025+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file Video.hpp
 * @brief Interface for EagleVideo objects
 */

 
#ifndef EagleVideo_HPP
#define EagleVideo_HPP

class EagleGraphicsContext;
class EagleSound;
class EagleImage;
 
 
#include <cstdint>
 
class EagleVideo {
protected :
   EagleGraphicsContext* window;
   EagleSound* sound;
   int64_t audio_sample_index;///< Counted in samples
   EagleImage** frames;
   int32_t frame_buffer_count;
   int32_t frame_buffer_index;
   
public :
   
   
   
};



#endif // EagleVideo_HPP
