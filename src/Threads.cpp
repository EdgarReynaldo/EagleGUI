

#include "Eagle/Threads.hpp"




int EagleThread::new_thread_id = 0;



EagleThread::EagleThread() : thread_id(new_thread_id++) {}


