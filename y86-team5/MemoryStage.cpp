#include "Y86.h"
#include "MemoryStage.h"
#include "Memory.h"
#include "Forward.h"

void MemoryStage::reset(WritebackStage *wbs, Memory *mem, Forward *forw){
    writebackStage = wbs;
    memory = mem;
    forward = forw;
    
    stat.setInput(SBUB);
    icode.setInput(INOP);
    ifun.setInput(FNONE);
    cnd.setInput(false);
    valE.setInput(0);
    valA.setInput(0);
    dstE.setInput(RNONE);
    dstM.setInput(RNONE);
}

void MemoryStage::updateMRegister(uint64_t m_stat, uint64_t m_icode, uint64_t m_ifun,
bool m_cnd, uint64_t m_valE, uint64_t m_valA, uint64_t m_dstE, uint64_t m_dstM){
    stat.setInput(m_stat);
    icode.setInput(m_icode);
    ifun.setInput(m_ifun);
    cnd.setInput(m_cnd);
    valE.setInput(m_valE);
    valA.setInput(m_valA);
    dstE.setInput(m_dstE);
    dstM.setInput(m_dstM);
}

void MemoryStage::clockP0(){
    stat.clock();
    icode.clock();
    ifun.clock();
    cnd.clock();
    valE.clock();
    valA.clock();
    dstE.clock();
    dstM.clock();
    
    m_stat = stat.getState();
    m_icode = icode.getState();
    m_ifun = ifun.getState();
    m_cnd = cnd.getState();
    m_valE = valE.getState();
    m_valA = valA.getState();
    m_dstE = dstE.getState();
    m_dstM = dstM.getState();

    m_valM = 0;
}

void MemoryStage::clockP1(){
    writebackStage->updateWRegister(m_stat, m_icode, m_ifun, m_valE, m_valM, m_dstE, m_dstM);
}
