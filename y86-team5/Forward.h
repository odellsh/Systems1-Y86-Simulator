#ifndef FORWARD_H
#define FORWARD_H

class Forward
{   
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
		void setWdstE(uint64_t W_dstE_) {W_dstE = W_dstE_;}
        uint64_t getWdtsE() {return W_dstE;}

        void setWvalE(uint64_t W_valE_) {W_valE = W_valE_;}
        uint64_t getWvalE() {return W_valE;}

        uint64_t getedstE() {return e_dstE;}
        uint64_t getMdstM() {return M_dstM;}
        uint64_t getMdstE() {return M_dstE;}
        uint64_t getWdstM() {return W_dstM;}
        uint64_t getevalE() {return e_valE;}
        uint64_t getmvalM() {return m_valM;}
        uint64_t getMvalE() {return M_valE;}
        uint64_t getWvalM() {return W_valM;}
        uint64_t getdrvalA() {return d_rvalA;}
        uint64_t getdrvalB() {return d_rvalB;}
};

#endif