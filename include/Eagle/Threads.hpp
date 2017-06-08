


#ifndef Threads_HPP
#define Threads_HPP


class EagleThread;

// Your process will be given access to the eagle thread, and the ptr specified in EagleThread::Create
typedef void* (*EAGLE_THREAD_PROCESS)(EagleThread* , void*);



class EagleThread {
   
private :
   static int new_thread_id;
   
   int thread_id;
   
public :
   EagleThread();
   virtual ~EagleThread() {}
   
   virtual bool Create(void* (*process_to_run)(EagleThread* , void*) , void* arg)=0;
   virtual void Destroy()=0;
   virtual void Start()=0;
   virtual void SignalToStop()=0;

   virtual void* Join()=0;/// Implicitly signal stop
   virtual void* FinishThread()=0;/// Wait for thread to complete

   virtual bool ShouldStop()=0;
   virtual bool Running()=0;
   virtual bool Valid()=0;

   int ID() {return thread_id;}

};



#endif // Threads_HPP
