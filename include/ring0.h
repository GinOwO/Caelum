#ifndef RING0_H
#define RING0_H

#include "ctexcept.h"

#include <functional>

/*
Class: Ring0
Contains only basic arithmetic, logical instructions, transfer and flag instructions.
Guaranteed to not use any instructions that require Ring1 and above.
Not meant to be used directly, but to be inherited by Ring1 and above.
*/

class Ring0{
public:
    std::function<void(int, unsigned long long)> setRegister;
    std::function<unsigned long long(int)> getRegister;
    std::function<void(int, bool)> setFlag;
    std::function<bool(int)> getFlag;
    std::function<void(size_t)> setPointer;
    std::function<int(int)> getRegSize;
    std::function<void(unsigned long long)> push; //call stack
    std::function<unsigned long long()> pop;

    void MOV(int, int); // Memory move implemented in interpreter.cpp as a special case
    void XCHG(int, int);
    void LEA(int, int); // Implemented in interpreter.cpp as a special case
    void INC(int);
    void ADD(int, int);
    void DEC(int);
    void SUB(int, int);
    void MUL(int);
    void DIV(int);
    void AND(int, int);
    void ANDN(int, int);
    void OR(int, int);
    void XOR(int, int);
    void NOT(int); 
    void NOP();
    void HLT();
public:
    Ring0(
        std::function<void(int, unsigned long long)> setReg,
        std::function<unsigned long long(int)> getReg,
        std::function<void(int, bool)> setFlg,
        std::function<bool(int)> getFlg,
        std::function<void(size_t)> setPtr,
        std::function<void(unsigned long long)> pushFn,
        std::function<unsigned long long()> popFn,
        std::function<int(int)> getRegSz
        ) : setRegister(setReg), getRegister(getReg), setFlag(setFlg),
          getFlag(getFlg), setPointer(setPtr), push(pushFn), pop(popFn),
          getRegSize(getRegSz){}
    ~Ring0(){}

    static int popcnt(unsigned long long);
    static int bitcnt(unsigned long long);
    static inline unsigned long long bitset(int n);
};

#endif
