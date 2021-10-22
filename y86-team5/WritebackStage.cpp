#include "Y86.h"
#include "WritebackStage.h"
//#include "ProgRegisters.h"
#include "Forward.h"

void WritebackStage::reset(ProgRegisters *progreg, Forward *forw){
    registers = progreg;
    forward = forw;
    
    stat.setInput(SBUB);
    icode.setInput(INOP);
    ifun.setInput(FNONE);
    valE.setInput(0);
    valM.setInput(0);
    dstE.setInput(RNONE);
    dstM.setInput(RNONE);
}

void WritebackStage::updateWRegister(uint64_t w_stat, uint64_t w_icode, uint64_t w_ifun, 
uint64_t w_valE, uint64_t w_valM, uint64_t w_dstE, uint64_t w_dstM){
    stat.setInput(w_stat);
    icode.setInput(w_icode);
    ifun.setInput(w_ifun);
    valE.setInput(w_valE);
    valM.setInput(w_valM);
    dstE.setInput(w_dstE);
    dstM.setInput(w_dstM);
}

void WritebackStage::clockP0(){
    stat.clock();
    icode.clock();
    ifun.clock();
    valE.clock();
    valM.clock();
    dstE.clock();
    dstM.clock();

    w_stat = stat.getState();
    w_icode = icode.getState();
    w_ifun = ifun.getState();
    w_valE = valE.getState();
    w_valM = valM.getState();
    w_dstE = dstE.getState();
    w_dstM = dstM.getState();

    forward->setWdstE(w_dstE);
    forward->setWvalE(w_valE);
}

void WritebackStage::clockP1(){
    registers->setReg(w_dstE, w_valE);
    registers->setReg(w_dstM, w_valM);
}
