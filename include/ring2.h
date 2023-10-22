#ifndef RING2_H
#define RING2_H

#include "ring1.h"

/*
Class: Ring2
Contains branching and looping instructions such as JMP, JZ, JNZ, LOOP etc+CMP,TEST.
Guaranteed to not use any instructions that require Ring3 and above.
Not meant to be used directly, but to be inherited by Ring3 and above
*/

class Ring2 : public Ring1{
protected:
    void CMP(int, int);
    void TEST(int, int);
    void JMP(int);
    void JE(int);
    void JNE(int);
    void JG(int);
    void JGE(int);
    void JL(int);
    void JLE(int);
    void JZ(int);
    void JNZ(int);
    void JP(int);
    void JNP(int);
    void JPE(int);
    void JPO(int);
    void JO(int);
    void JNO(int);
    void JC(int);
    void JNC(int);
    void JS(int);
    void JNS(int);

    void LOOP(int);
    void LOOPE(int);
    void LOOPNE(int);

    void RET();
public:
    Ring2(
        std::function<void(int, unsigned long long)> setReg,
        std::function<unsigned long long(int)> getReg,
        std::function<void(int, bool)> setFlg,
        std::function<bool(int)> getFlg,
        std::function<void(size_t)> setPtr,
        std::function<void(unsigned long long)> pushFn,
        std::function<unsigned long long()> popFn,
        std::function<int(int)> getRegSz
    ) : Ring1(setReg, getReg, setFlg, getFlg, setPtr, pushFn, popFn, getRegSz){}
    ~Ring2(){}
};

#endif
