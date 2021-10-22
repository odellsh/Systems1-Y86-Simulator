/*
    File:   ExecuteStage.h
    Desc:   Declares ExecuteStage class and associated constants
    
*/
#ifndef EXECUTESTAGE_H
#define EXECUTESTAGE_H

#include "Sim.h"
#include "PipeStage.h"
#include "Register.h"
#include "DecodeStage.h"
#include "MemoryStage.h"
#include "ProgRegisters.h"
//#include "ExecuteStage.h"
#include "Forward.h"

class ExecuteStage : public PipeStage
{

    /* Pointers to Memory Stage */
    MemoryStage *memoryStage;
    Forward *forward;
    ProgRegisters *registers;

    Register<uint64_t> valA;
    Register<uint64_t> valB;
    Register<uint64_t> valC;

    Register<uint64_t> srcA;
    Register<uint64_t> srcB;
    Register<uint64_t> dstE;
    Register<uint64_t> dstM;

    /* signals produced within the stage - Use names similar to Figure 4.57 p. 448 of text */    
    uint64_t e_stat;
    uint64_t e_icode;
    uint64_t e_ifun;
    bool e_cnd;
    uint64_t e_valA;
    uint64_t e_valB;
    uint64_t e_valC;

    uint64_t e_valE;
    uint64_t e_srcA;
    uint64_t e_srcB;
    uint64_t e_dstE;
    uint64_t e_dstM;
    uint64_t e_rA;
    uint64_t e_rB;
    uint64_t e_valP;

    uint64_t e_aluA;
    uint64_t e_aluB;
    uint64_t e_alufun;
    uint64_t e_valueE;


    //no private methods

	public:
		void reset(MemoryStage *, ProgRegisters *, Forward *);
        void updateERegister(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, 
            uint64_t, uint64_t, uint64_t, uint64_t, uint64_t);
        
        uint64_t getALUA();
        uint64_t getALUB();
        uint64_t getALUFunction();
        bool isSetCC();
        uint64_t selectDstE();
        void setCC();


		/* (Virtual) Functions of superclass */
		void clockP0();
		void clockP1();
		void trace();
    
    
};

#endif
