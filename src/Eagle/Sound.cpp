
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
 *    Copyright 2009-2023+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file Sound.cpp
 * @brief Base class implementations of sound objects
 * 
 */


 
#include "Eagle/Sound.hpp"
 
 


SoundSample::SoundSample() 
{}



SoundSample::~SoundSample() 
{}



SoundInstance::~SoundInstance() 
{}



SoundInstance::SoundInstance(SoundSample* parent_sample) :
      parent(parent_sample)
{}



SoundStream::SoundStream()
{}



SoundStream::~SoundStream() 
{}


