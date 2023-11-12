
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
 
 


EagleSoundSample::EagleSoundSample() :
      ResourceBase(RT_AUDIO_SAMPLE)
{}



EagleSoundSample::~EagleSoundSample() 
{}



EagleSoundInstance::EagleSoundInstance(EagleSoundSample* parent_sample) :
      parent(parent_sample)
{}



EagleSoundInstance::~EagleSoundInstance() 
{}



EagleSoundStream::EagleSoundStream() :
      ResourceBase(RT_AUDIO_STREAM)
{}



EagleSoundStream::~EagleSoundStream() 
{}


