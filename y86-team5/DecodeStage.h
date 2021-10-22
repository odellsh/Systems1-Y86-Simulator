/*
    File:   DecodeStage.h
    Desc:   Declares DecodeStage class and associated constants
    
*/
#ifndef DECODESTAGE_H
#define DECODESTAGE_H

#include "Sim.h"
#include "PipeStage.h"
//#include "Register.h"
#include "ExecuteStage.h"
#include "ProgRegisters.h"
//#include "Memory.h"
#include "Forward.h"

class DecodeStage : public PipeStage
{

    /* Pointers to Execute Stage and ProgRegisters object */
    ExecuteStage *executeStage;
    ProgRegisters *registers;
    Forward *forward;
    //Memory *memory;

    Register <uint64_t> rA;
    Register <uint64_t> rB;
    Register <uint64_t> valC;
    Register <uint64_t> valP;
	
    /* signals produced within the stage - Use names similar to Figure 4.57 p. 448 of text */    
    uint64_t d_stat;
    uint64_t d_icode;
    uint64_t d_ifun;

    uint64_t d_valC;   
    uint64_t d_valA;
    uint64_t d_valB;

    uint64_t d_srcA;
    uint64_t d_srcB;

    uint64_t d_dstE;
    uint64_t d_dstM;

    uint64_t d_rA;  //are these ok?
    uint64_t d_rB;

    uint64_t d_valP;
    //no private methods

    uint64_t W_dstE;
    uint64_t W_valE;
    uint64_t e_dstE;
    uint64_t M_dstM;
    uint64_t M_dstE;
    uint64_t W_dstM;

    uint64_t e_valE;
    uint64_t m_valM;
    uint64_t M_valE;
    uint64_t W_valM;
    uint64_t d_rvalA;
    uint64_t d_rvalB;


	public:
		void reset(ExecuteStage *, ProgRegisters *, Forward *);
        void updateDRegister(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t);

        uint64_t getSrcA();
        uint64_t getSrcB();
        uint64_t getDstE();
        uint64_t getDstM();
        uint64_t selectFwdA();
        uint64_t forwardB();



		/* (Virtual) Functions of superclass */
		void clockP0();
		void clockP1();
		void trace();
    
    
};

#endif