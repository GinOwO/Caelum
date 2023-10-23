#ifndef RING4_H
#define RING4_H

#include "ring3.h"

#include <functional>
#include <sstream>

/*
Class: Ring4
Non Standard Higher Order Instructions implemented using the C/C++ lib
*/

class Ring4 : public Ring3{
protected:
    std::function<unsigned long long(unsigned long long)> getMem;
    std::stringstream& OUT;
    
    void PUTU();
    void PUTI();
    void PUTC();
    void PUTD();
    void PUTCH(unsigned char);
    void PUTS(unsigned int);
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
        std::function<unsigned long long(unsigned long long)> getMem,
        std::stringstream& out
    ) : Ring3(setReg, getReg, setFlg, getFlg, setPtr, pushFn, popFn, getRegSz),
        OUT(out), getMem(getMem){}
    ~Ring4(){};
};

#endif
