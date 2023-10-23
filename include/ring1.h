#ifndef RING1_H
#define RING1_H

#include "ring0.h"

/*
Class: Ring1
Contains extended arithmetic and logic instructions such as ADC, SBB, shifting etc.
Guaranteed to not use any instructions that require Ring2 and above.
Not meant to be used directly, but to be inherited by Ring2 and above
*/

class Ring1 : public Ring0{
protected:
    void ADC(int, int);
    void SBB(int, int);
    void AAA();
    void AAS();
    void AAM();
    void AAD();
    void DAA();
    void DAS();
    void SHL(int, int);
    void SHR(int, int);
    void SAL(int, int);
    void SAR(int, int);
    void ROL(int, int);
    void ROR(int, int);
    void RCL(int, int);
    void RCR(int, int);

public:
    Ring1(
        std::function<void(int, unsigned long long)> setReg,
        std::function<unsigned long long(int)> getReg,
        std::function<void(int, bool)> setFlg,
        std::function<bool(int)> getFlg,
        std::function<void(size_t)> setPtr,
        std::function<void(unsigned long long)> pushFn,
        std::function<unsigned long long()> popFn,
        std::function<int(int)> getRegSz
    ) : Ring0(setReg, getReg, setFlg, getFlg, setPtr, pushFn, popFn, getRegSz){}
    ~Ring1(){}
};


#endif
