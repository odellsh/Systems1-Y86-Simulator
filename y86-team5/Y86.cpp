/*
    Name:   Y86.cpp
    Desc:   Starting implementation of Y86 class
    Author: efb
    Date:   1/6/2016
*/
#include <cstdint>
#include "Y86.h"


/*-------------------------------------------------------------------------
  Name:  Y86()
  Desc:  Constructor
---------------------------------------------------------------------------*/
Y86::Y86()
{
    reset();
}
/*-------------------------------------------------------------------------
  reset - resets Y86 object to initial state.
  
---------------------------------------------------------------------------*/
void Y86::reset()
{
  stage[0] = &fetchStage;
  stage[1] = &decodeStage;
  stage[2] = &executeStage;
  stage[3] = &memoryStage;
  stage[4] = &writebackStage;

  fetchStage.reset(&decodeStage, &memory , &forward);
  decodeStage.reset(&executeStage, &regs, &forward);
  executeStage.reset(&memoryStage, &regs, &forward);
  memoryStage.reset(&writebackStage, &memory, &forward);
  writebackStage.reset(&regs, &forward);
  cycles = 0;
  
  memory.reset();   //resets memory
  regs.reset();     //restes progregisters
  
}

/*-------------------------------------------------------------------------
  clockP0 - calls clock function of ProgRegisters class
  
---------------------------------------------------------------------------*/
void  Y86::clock(){
  regs.clock();
  
  for(int i = 0; i < 5; i++){
    stage[i]->clockP0();
  }
  for(int i = 0; i < 5; i++){
    stage[i]->clockP1();
  }
}

uint64_t Y86::getStat(){
  uint64_t wstat = writebackStage.getStat();
  if(wstat == SAOK || wstat == SBUB){
    return 0;
  }
  else{
    return wstat;
  }
}