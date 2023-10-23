#include "ring3.h"

void Ring3::ADCX(int dest, int src){
    /*The CF flag is set based on result, other flags are unmodified*/
    if(dest == 1) throw Exception::BadInstructionException();
    unsigned long long temp2 = getRegister(src);
    unsigned long long temp3 = getRegister(dest);
    unsigned long long temp = temp3+temp2+getFlag(0);
    int dstSize = getRegSize(dest);
    setFlag(0, (temp>>(dstSize-1))!=(temp3>>(dstSize-1)) || temp<temp3); // Carry flag
    setRegister(dest, temp);
}

void Ring3::SBBX(int dest, int src){
    /*The CF flag is set based on result, other flags are unmodified*/
    if(dest == 1) throw Exception::BadInstructionException();
    unsigned long long temp2 = getRegister(src);
    unsigned long long temp3 = getRegister(dest);
    unsigned long long temp = temp3-temp2-getFlag(0);
    int dstSize = getRegSize(dest);
    setFlag(0, (temp>>(dstSize-1))!=(temp3>>(dstSize-1)) || temp>temp3); // Carry flag
    setRegister(dest, temp);
}

void Ring3::SHLX(int dest, int src){
    /*Affects no flags*/
    if(dest == 1) throw Exception::BadInstructionException();
    unsigned long long temp2 = getRegister(src);
    unsigned long long temp3 = getRegister(dest);
    setRegister(dest, temp3<<temp2);
}

void Ring3::SHRX(int dest, int src){
    /*Affects no flags*/
    if(dest == 1) throw Exception::BadInstructionException();
    unsigned long long temp2 = getRegister(src);
    unsigned long long temp3 = getRegister(dest);
    setRegister(dest, temp3>>temp2);
}

void Ring3::SALX(int dest, int src){
    /*Affects no flags*/
    this->SHLX(dest, src);
}

void Ring3::SARX(int dest, int src){
    /*Affects no flags*/
    if(dest == 1) throw Exception::BadInstructionException();
    unsigned long long temp = getRegister(dest);
    unsigned long long cnt = getRegister(src);
    if(cnt == 0) return;
    int dstSize = getRegSize(dest);
    unsigned long long msb = 1<<dstSize;
    bool t = temp&msb;
    while(cnt--){
        temp >>= 1;
        if(t) temp |= msb;
    }
    setRegister(dest, temp);
}

void Ring3::RORX(int dest, int src){
    /*Affects no flags*/
    if(dest == 1) throw Exception::BadInstructionException();
    unsigned long long temp = getRegister(dest);
    unsigned long long cnt = getRegister(src);
    if(cnt == 0) return;
    int dstSize = getRegSize(dest);
    unsigned long long msb = 1<<dstSize;
    while(cnt--){
        bool t = temp&1;
        temp >>= 1;
        if(t) temp |= msb;
    }
    setRegister(dest, temp);
}

void Ring3::PUSH(int src){
    /*Affects no flags*/
    if(src == 1) throw Exception::BadInstructionException();
    unsigned long long temp = getRegister(src);
    this->push(temp);
}

void Ring3::POP(int dest){
    /*Affects no flags*/
    if(dest == 1) throw Exception::BadInstructionException();
    unsigned long long temp = this->pop();
    setRegister(dest, temp);
}
