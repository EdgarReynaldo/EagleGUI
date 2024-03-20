



#ifndef Allegro5BinStream_HPP
#define Allegro5BinStream_HPP

#include "Eagle/BinStream.hpp"

class Allegro5BinStream : public BinStream {

public :
   virtual bool SaveDataToFile(FilePath fp) const override ;
   virtual bool ReadDataFromFile(FilePath fp) override;
};


#endif // Allegro5BinStream_HPP
