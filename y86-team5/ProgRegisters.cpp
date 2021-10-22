/*
    Name:       ProgRegisters.cpp
    
    Desc:       Implements functions for the program register file.
    
    Author:     Sydney O'Dell, Nick Burkett
    
    Date:       
*/
#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <cassert>


#include "Tools.h"
#include "ProgRegisters.h"


/*-------------------------------------------------------------------------------------------------
    ProgRegisters constructor  
--------------------------------------------------------------------------------------------------*/
ProgRegisters::ProgRegisters()
{
    reset();
}

/*-------------------------------------------------------------------------------------------------
    clock  - calls clock method of each register and also the CC register
--------------------------------------------------------------------------------------------------*/
void ProgRegisters:: clock()
{
	for(int i = 0; i < NUM_REGISTERS; i++){
        reg[i].clock();
    }
    CC.clock();
} 

/*-------------------------------------------------------------------------------------------------
    setReg - sets the INPUT of the specified register
--------------------------------------------------------------------------------------------------*/
void ProgRegisters:: setReg(unsigned regNum, uint64_t rval)
{
    assert(regNum < 16); //assert to make sure regNum is < 16
    if(regNum == 15) //if the regNum is the NONREG
        return;
    else
        reg[regNum].setInput(rval);
}
/*-------------------------------------------------------------------------------------------------
    getReg - returns STATE of specified register
--------------------------------------------------------------------------------------------------*/
uint64_t ProgRegisters:: getReg(unsigned regNum)
{
    assert(regNum < 16); //assert to make sure regNum is < 16
    if(regNum == 15) //if the regNum is the NONREG
        return 0;   //return 0
    else
        return reg[regNum].getState();
}       
/*-------------------------------------------------------------------------------------------------
    setCC - Sets the INPUT of the specified CC bit to the specified value (0 or 1)
--------------------------------------------------------------------------------------------------*/
void ProgRegisters:: setCC(unsigned bitNumber, unsigned val)
{
	// your code here
    assert(val == 0 || val == 1); //value needs to be 0 or 1
    assert(bitNumber < 8);      //bitnumber needs to be 0-7

    //assigns bit of 0/1 to a bitNumber 0-8 with the input of a specific CC bit
    uint64_t holder = Tools::assignOneBit(bitNumber, val, CC.getInput());
    CC.setInput(holder);

}

/*-------------------------------------------------------------------------------------------------
    getCC - returns the STATE of the specified CC bit
--------------------------------------------------------------------------------------------------*/
unsigned ProgRegisters:: getCC(unsigned bitNumber)
{
	assert(bitNumber < 8); //assert to make sure input bitnumber is less than 8(0-7)
    return Tools::getBits(bitNumber, bitNumber, CC.getState());
}
          
/*-------------------------------------------------------------------------------------------------
    reset - resets registers and CC flags
--------------------------------------------------------------------------------------------------*/
void ProgRegisters:: reset(void)
{
    //unsigned long seesee = 0;
	for(int i = 0; i < NUM_REGISTERS; i++){ //for loop for every value < NUM_REGISTERS
        reg[i].reset(); //resets every value
    }
    CC.reset(4);
     
    setCC(OF,0);
    setCC(ZF,1);
    setCC(SF,0);
} 
