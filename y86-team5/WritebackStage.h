/*
    File:   WritebackStage.h
    Desc:   Declares WritebackStage class and associated constants
    
*/
#ifndef WRITEBACKSTAGE_H
#define WRITEBACKSTAGE_H

#include "Sim.h"
#include "PipeStage.h"
#include "Register.h"
//#include "Memory.h"
#include "ProgRegisters.h"
#include "FetchStage.h"
#include "DecodeStage.h"
#include "ExecuteStage.h"
#include "MemoryStage.h"
#include "Forward.h"

class WritebackStage : public PipeStage
{

    /* Pointers to ProgRegisters object */
    ProgRegisters *registers;
    Forward *forward;

    //Memory *memory;

	Register<uint64_t> valE;
    Register<uint64_t> valM;
    Register<uint64_t> dstE;
    Register<uint64_t> dstM;

    /* signals produced within the stage - Use names similar to Figure 4.57 p. 448 of text */
    uint64_t w_stat;
    uint64_t w_icode;
    uint64_t w_ifun;
    uint64_t w_dstE;
    uint64_t w_dstM;
    uint64_t w_valE;
    uint64_t w_valM;
    //no private methods

    uint64_t W_dstE;
    uint64_t W_valE;

	public:
		void reset(ProgRegisters *, Forward *);
        void updateWRegister(uint64_t, uint64_t, uint64_t, uint64_t, 
            uint64_t, uint64_t, uint64_t);

		/* (Virtual) Functions of superclass */
		void clockP0();
		void clockP1();
		void trace();
    
    
};

#endif