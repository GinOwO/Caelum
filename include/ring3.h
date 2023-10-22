#ifndef RING3_H
#define RING3_H

#include "ring2.h"

#include <stack>

/*
Class: Ring3
Contains extended instructions which do not trigger flags, except CF maybe
Guaranteed to not use any instructions that require higher Rings.
Not meant to be used directly, but to be inherited by InstructionHandler
*/

class Ring3 : public Ring2{
protected:
    std::stack<unsigned long long> stack;

    void ADCX(int, int); // CF affect
    void SBBX(int, int); // CF affect
    void SHLX(int, int);
    void SHRX(int, int);
    void SALX(int, int);
    void SARX(int, int);
    void RORX(int, int);
    void PUSH(int);
    void POP(int);

public:
    Ring3(
        std::function<void(int, unsigned long long)> setReg,
        std::function<unsigned long long(int)> getReg,
        std::function<void(int, bool)> setFlg,
        std::function<bool(int)> getFlg,
        std::function<void(size_t)> setPtr,
        std::function<void(unsigned long long)> pushFn,
        std::function<unsigned long long()> popFn,
        std::function<int(int)> getRegSz
    ) : Ring2(setReg, getReg, setFlg, getFlg, setPtr, pushFn, popFn, getRegSz){}
    ~Ring3(){}
};


#endif
