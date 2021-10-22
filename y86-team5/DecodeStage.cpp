#include "Y86.h"
#include "DecodeStage.h"
//#include "ProgRegisters.h"
#include "Forward.h"


void DecodeStage::reset(ExecuteStage *execute, ProgRegisters *progreg, Forward *forw){
    executeStage = execute;
    registers = progreg;
    forward = forw;
    
    stat.setInput(SBUB);
    icode.setInput(INOP);
    ifun.setInput(FNONE);
    rA.setInput(RNONE);
    rB.setInput(RNONE);
    valC.setInput(0);
    valP.setInput(0);
}

void DecodeStage::updateDRegister(uint64_t d_stat, uint64_t d_icode, uint64_t d_ifun, uint64_t d_rA, 
uint64_t d_rB, uint64_t d_valC, uint64_t d_valP){
    valP.setInput(d_valP);
    rA.setInput(d_rA);
    rB.setInput(d_rB);
    valC.setInput(d_valC);
    icode.setInput(d_icode);
    ifun.setInput(d_ifun);
    stat.setInput(d_stat);
}

void DecodeStage::clockP0(){
    stat.clock();
    icode.clock();
    ifun.clock();
    rA.clock();
    rB.clock();
    valC.clock();
    valP.clock();
}

void DecodeStage::clockP1(){
    d_stat = stat.getState();
    d_icode = icode.getState();
    d_ifun = ifun.getState();

    d_rA = rA.getState();
    d_rB = rB.getState();

    d_valC = valC.getState();
    d_valP = valP.getState();

    d_srcA = getSrcA();
    d_srcB = getSrcB();

    d_dstE = getDstE();
    d_dstM = getDstM();

    d_valA = selectFwdA();
    d_valB = forwardB();


    //for comparing in selectFwdA and ForwardB()
    e_dstE = forward->getedstE();
    M_dstM = forward->getMdstM();
    M_dstE = forward->getMdstE();
    W_dstM = forward->getWdstM();
    W_dstE = forward->getWdtsE();
    e_valE = forward->getevalE();
    m_valM = forward->getmvalM();
    M_valE = forward->getMvalE();
    W_valM = forward->getWvalM();
    W_valE = forward->getWvalE();
    d_rvalA = forward->getdrvalA();


    executeStage->updateERegister(d_stat, d_icode, d_ifun, d_valA, d_valB, d_valC, d_srcA, d_srcB, d_dstE, d_dstM);

}
uint64_t DecodeStage::getSrcA(){
    if(d_icode == IRRMOVQ || d_icode == IRMMOVQ || d_icode == IOPX || d_icode == IPUSHQ){
        return d_rA;
    }
    else if(d_icode == IPOPQ || d_icode == IRET){
        return RSP;
    }
    else{
        return RNONE;
    }
}

uint64_t DecodeStage::getSrcB(){
    if(d_icode == IOPX || d_icode == IRMMOVQ || d_icode == IMRMOVQ){
        return d_rB;
    }
    else if(d_icode == IPUSHQ || d_icode == IPOPQ || d_icode == ICALL || d_icode == IRET){
        return RSP;
    }
    else{
        return RNONE;
    }
}

uint64_t DecodeStage::getDstE(){
    if(d_icode == IRRMOVQ || d_icode == IIRMOVQ || d_icode == IOPX){
        return d_rB;
    }
    else if(d_icode == IPUSHQ || d_icode == IPOPQ || d_icode == ICALL || d_icode == IRET){
        return RSP;
    }
    else{
        return RNONE;
    }
}

uint64_t DecodeStage::getDstM(){
    if(d_icode == IMRMOVQ || d_icode == IPOPQ){
        return d_rA;
    }
    else{
        return RNONE;
    }
}

uint64_t DecodeStage::selectFwdA(){
    if(d_icode == ICALL || d_icode == IJXX){
        return d_valP;
    }
    else if(d_srcA == e_dstE){
        return e_valE;
    }
    else if(d_srcA == M_dstM){
        return m_valM;
    }
    else if(d_srcA == M_dstE){
        return M_valE;
    }
    else if(d_srcA == W_dstM){
        return W_valM;
    }
    else if(d_srcA == W_dstE){
        return W_valE;
    }
    else{
        return d_rvalA;
    }
}

uint64_t DecodeStage::forwardB(){
    if(d_srcB == e_dstE){
        return e_valE;
    }
    else if(d_srcB == M_dstM){
        return m_valM;
    }
    else if(d_srcB == M_dstE){
        return M_valE;
    }
    else if(d_srcB == W_dstM){
        return W_valM;
    }
    else if(d_srcB == W_dstE){
        return W_valE;
    }
    else{
        return d_rvalB;
    }
}