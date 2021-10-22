/*
    Name:   Memory.cpp
    Desc:   Implementation of the Memory class
    Auth:   Sydney O'Dell/Nick Burkett
    Date:   2/14/21
*/
#include <iostream>
#include <iomanip>
#include <cstdint>
#include <cassert>

#include "Memory.h"
#include "Tools.h"

using namespace std;
//Memory* Memory::singleInstance = NULL;


/*-------------------------------------------------------------------------
  Constructor:  Memory 
  Description:  Store 64-bit value to a word address in memory. Set
                memError if address out of bounds.
--------------------------------------------------------------------------*/
Memory::Memory()
{
    reset();
}

//Memory* Memory::getInstance(){
//   if(singleInstance == NULL){
//      singleInstance = new Memory();
//   }
//   return singleInstance;
//}

/*-------------------------------------------------------------------------
  Function:     store 
  Description:  Store 64-bit value to a word address in memory. Set
                memError if address out of bounds.
--------------------------------------------------------------------------*/
void   Memory::store	(uint64_t waddr, uint64_t val) // takes word address
{
   //checks address is valid
   memError = false;

   if(waddr >= MEMORY_SIZE){
           //set the flag
      memError = true;
   }
   else{
      mem[waddr] = val; //else store the value at that address inside the mem array
   }
}
/*-------------------------------------------------------------------------
  Function:     fetch 
  Description:  Fetch a 64-bit value from a word address in memory. Set
                memError if address out of bounds.
--------------------------------------------------------------------------*/
uint64_t  Memory::fetch	(uint64_t waddr) // takes word address
{
   //checking if address is valid
   memError = false;

   if(waddr >= MEMORY_SIZE){
      memError = true;  //set the flag
   }
   else{
      return mem[waddr]; //else return the value in that address in the mem array
   }
   return 0;
}
/*--------------------------------------------------------------------
   Function:   getByte

   Description:  Returns a byte from the specified byte address
                 of Y86 memory array. 
                 If address is out of range the error status is set.
--------------------------------------------------------------------*/
unsigned char   Memory::getByte	(uint64_t byteAddress) // takes byte address
{
      uint64_t waddy = byteAddress / 8; //returns the wa (0....)
      unsigned bnum = byteAddress % 8; //gets a number between 0-7

      uint64_t full_word = fetch(waddy); //returns 64-bit number from address
      if(isError()){
         return 0;
      }
      return Tools::getByteNumber(bnum, full_word); //
}

/*--------------------------------------------------------------------
   Function:     putByte

   Description:  Write a single byte into the Y86 simulated memory
                 at the byte address specified by 'address'.
                 If address is out of range the error status is set.                 
--------------------------------------------------------------------*/
void  Memory::putByte(uint64_t byteAddress, uint8_t value) // takes byte address
{
      uint64_t waddy = byteAddress / 8; //returns the wa (0....)
      unsigned bnum = byteAddress % 8; //gets a number between 0-7

      uint64_t full_word = fetch(waddy); //returns 64-bit number from address
      if(isError())
         return;

      uint64_t word = Tools::putByteNumber(bnum, value, full_word); //
      store(waddy, word);
}
/*--------------------------------------------------------------------
   Function:   getWord

   Description:  Returns word starting from the specified byte address.
                 No part of the word must lie outside memory range.
--------------------------------------------------------------------*/
uint64_t Memory::getWord (uint64_t byteAddress)
{
   uint64_t gwMAX = (MEMORY_SIZE * 8) - 8;

   if(byteAddress >= gwMAX){
      memError = true;
      return 0UL;
   }
   else{
      memError = false;

      unsigned char helperByte[8];
      for(uint64_t i = 0; i < 8; i++){
         helperByte[i] = getByte(byteAddress + i);
      }

      uint64_t helperWord = Tools::buildWord(helperByte[0], helperByte[1], helperByte[2], helperByte[3], helperByte[4], helperByte[5], helperByte[6], helperByte[7]);
      return helperWord;

   }

}
/*----------------------------------------------------------------------------------------------
   Function:   putWord

   Description:  Writes a word to memory starting at the specified byte address.
                 No part of the word must lie outside memory range. If there is
				 a memory error (out of range) the memory should not be changed.
------------------------------------------------------------------------------------------------*/
void Memory::putWord	(uint64_t byteAddress, uint64_t wordValue) 
{
   uint64_t pwMAX = (MEMORY_SIZE * 8) - 8;
   if(byteAddress >= pwMAX){ //if byte address is greater than pwMax
      memError = true;     //error is true
      return;              //and return
   }
   else{
      memError = false;    //else error is false
      //for loop

      for(uint64_t i = 0; i < 8; i++){;
         putByte(byteAddress + i, Tools::getByteNumber(i, wordValue));
      }

   }
}
/*--------------------------------------------------------------------
   Function:   reset

   Description:  Clears memory to all zero. Clears error status.
                 
--------------------------------------------------------------------*/
void  Memory::reset	(void) // clears memory to all zero
{
   memError = false;
   // loop
	for(int i = 0; i < MEMORY_SIZE; i++){
      //mem[i] = 0; //sets value to 0
      //mem[i] = 0;
      store(i, 0L);
   }
}