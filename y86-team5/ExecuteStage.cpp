#include "Y86.h"
#include "ExecuteStage.h"
#include "ProgRegisters.h"
#include "Forward.h"
#include <iostream>

void ExecuteStage::reset(MemoryStage *memorystage, ProgRegisters *progreg, Forward *forw){
    memoryStage = memorystage;
    forward = forw;
    registers = progreg;
    
    stat.setInput(SBUB);
    icode.setInput(INOP);
    ifun.setInput(FNONE);
    valC.setInput(0);
    valA.setInput(0);
    dstE.setInput(RNONE);
    dstM.setInput(RNONE);
    srcA.setInput(RNONE);
    srcB.setInput(RNONE);
}

void ExecuteStage::updateERegister(uint64_t e_stat, uint64_t e_icode, uint64_t e_ifun, uint64_t e_valA, 
uint64_t e_valB, uint64_t e_valC, uint64_t e_srcA, uint64_t e_srcB, uint64_t e_dstE, uint64_t e_dstM){
    stat.setInput(e_stat);
    icode.setInput(e_icode);
    ifun.setInput(e_ifun);
    valA.setInput(e_valA);
    valB.setInput(e_valB);
    valC.setInput(e_valC);
    srcA.setInput(e_srcA);
    srcB.setInput(e_srcB);
    dstE.setInput(e_dstE);
    dstM.setInput(e_dstM);
}

void ExecuteStage::clockP0(){
    stat.clock();
    icode.clock();
    ifun.clock();
    valA.clock();
    valB.clock();
    valC.clock();
    srcA.clock();
    srcB.clock();
    dstE.clock();
    dstM.clock();

    e_stat = stat.getState();
    e_icode = icode.getState();
    e_ifun = ifun.getState();
    e_cnd = true;
    e_valE = 0;

    e_srcA = srcA.getState();
    e_srcB = srcB.getState();

    e_dstE = dstE.getState();
    e_dstM = dstM.getState();

    e_valA = valA.getState();
    e_valB = valB.getState();
    e_valC = valC.getState();

    e_aluA = getALUA();
    e_aluB = getALUB();
    e_alufun = getALUFunction();

    //sets e_valE;
    if(e_alufun == FADDQ)
        e_valE = e_aluA + e_aluB;
    else if(e_alufun == FSUBQ)
        e_valE = e_aluB - e_aluA;
    else if(e_alufun == FANDQ)
        e_valE = e_aluA & e_aluB;
    else if(e_alufun == FXORQ)
        e_valE = e_aluA ^ e_aluB;
}

void ExecuteStage::clockP1(){
    if(e_icode == IOPX)
        setCC();
    
    memoryStage->updateMRegister(e_stat, e_icode, e_ifun, e_cnd, e_valE, e_valA, e_dstE, e_dstM);
}





uint64_t ExecuteStage::getALUA(){
    if (e_icode == IOPX || e_icode == IRRMOVQ){
        return e_valA;
    }
    else if (e_icode == IIRMOVQ || e_icode == IMRMOVQ || e_icode == IRMMOVQ){
        return e_valC;
    }
    else if (e_icode == IPOPQ || e_icode == IRET){
        return 8;
    }
    else if (e_icode == IPUSHQ || e_icode == ICALL){
        return -8;
    }
    return 0;
}

uint64_t ExecuteStage::getALUB(){
    if (e_icode == IOPX ||
        e_icode == IRMMOVQ ||
        e_icode == IMRMOVQ ||
        e_icode == IPOPQ ||
        e_icode == IRET ||
        e_icode == IPUSHQ ||
        e_icode == ICALL){
        
        return e_valB;
    }
    else {
        return 0;
    }
    return 0;
}

uint64_t ExecuteStage::getALUFunction(){
    if (e_icode == IOPX){
        return e_ifun;
    }
    return 0;
}

bool ExecuteStage::isSetCC(){
    if (e_icode == IOPX){
        return true;
    }
    else
        return false;
}

uint64_t ExecuteStage::selectDstE(){
    if (e_icode == !e_cnd && IRRMOVQ){
        return RNONE;
    }
    else {
        return e_dstE;
    }
}

void ExecuteStage::setCC() {
    bool i = e_valE == 0;
    bool j = e_valE >> 63 == 1;
    bool k = 0;

    bool n_aluA = e_aluA >> 63;
    bool n_aluB = e_aluB >> 63;
    bool n_valE = e_valueE >> 63;

    if (e_alufun == FADDQ){
        if (n_valE && !n_aluB && !n_aluA){
            k = 1;
        }
        else if (!n_valE && n_aluA && n_aluB){
            k = 1;
        }
    }
    else if (e_alufun == FSUBQ){
        if (!n_valE && !n_aluA && n_aluB){
            k = 1;
        }
        else if (n_valE && n_aluA && !n_aluB){
            k = 1;
        }
    }

    registers->setCC(ZF, i);
    registers->setCC(SF, j);
    registers->setCC(OF, k);
}


