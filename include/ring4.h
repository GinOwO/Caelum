#ifndef RING4_H
#define RING4_H

#include "ring3.h"

#include <functional>
#include <iostream>

/*
Class: Ring4
Reserved for future instruction implementations.
*/

class Ring4 : public Ring3{
protected:
    std::ostream& OUT;
    
    void PUTS();
    void FLUSH();
public:
    Ring4(
        std::function<void(int, unsigned long long)> setReg,
        std::function<unsigned long long(int)> getReg,
        std::function<void(int, bool)> setFlg,
        std::function<bool(int)> getFlg,
        std::function<void(size_t)> setPtr,
        std::function<void(unsigned long long)> pushFn,
        std::function<unsigned long long()> popFn,
        std::function<int(int)> getRegSz,
        std::ostream& out = std::cout
    ) : Ring3(setReg, getReg, setFlg, getFlg, setPtr, pushFn, popFn, getRegSz), OUT(out){}
    ~Ring4(){};
};

#endif