/*
    File:   MemoryStage.h
    Desc:   Declares MemoryStage class and associated constants
    
*/
#ifndef MEMORYSTAGE_H
#define MEMORYSTAGE_H

#include "Sim.h"
#include "PipeStage.h"
#include "Register.h"
#include "WritebackStage.h"
#include "Memory.h"
//#include "ProgRegisters.h"
#include "ExecuteStage.h"
#include "Forward.h"

class MemoryStage : public PipeStage
{

    /* Pointers to Writeback Stage and Memory object */
    WritebackStage *writebackStage;
    Memory *memory;
    Forward *forward;


	Register<bool> cnd;
    Register<uint64_t> valE;
    Register<uint64_t> valA;
    Register<uint64_t> dstE;
    Register<uint64_t> dstM;



    /* signals produced within the stage - Use names similar to Figure 4.57 p. 448 of text */  
    uint64_t m_stat;
    uint64_t m_icode;
    uint64_t m_ifun;
    uint64_t m_valA;
    uint64_t m_dstE;
    uint64_t m_dstM;
    bool m_cnd;
    uint64_t m_valE;
    uint64_t m_valM;

    //no private methods

	public:
		void reset(WritebackStage *, Memory *, Forward *);
        void updateMRegister(uint64_t, uint64_t, uint64_t, bool, uint64_t, uint64_t, uint64_t, uint64_t);

		/* (Virtual) Functions of superclass */
		void clockP0();
		void clockP1();
		void trace();
    
    
};

#endif