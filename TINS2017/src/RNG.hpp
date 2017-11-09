


#ifndef RNG_HPP
#define RNG_HPP


class RNG {
public :
   virtual ~RNG() {}
   
   virtual void SeedRng(int seed);

   virtual int Rand0toNminus1(int n);

   int Rand0toN(int n);



};


extern RNG rng;


#endif // RNG_HPP

