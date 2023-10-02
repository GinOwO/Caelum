#include "ring2.h"

void Ring2::CMP(int dest, int src){
    /*The CF, OF, SF, ZF, AF, and PF flags are set according to the result.*/
    if(dest == 1 || src == 0) throw Exception::BadInstructionException();
    unsigned long long temp2 = getRegister(src);
    unsigned long long temp3 = getRegister(dest);
    unsigned long long temp = temp3-temp2;
    int dstSize = getRegSize(dest);
    setFlag(0, (temp>>(dstSize-1))!=(temp3>>(dstSize-1)) || temp>temp3); // Carry flag
    setFlag(1, Ring0::popcnt(temp)%2==0); // Parity flag
    if(dstSize==8) setFlag(2, ((temp2&0xF)-(temp3&0xF))&0x10); // Aux Carry Flag
    setFlag(3, temp==0); // Zero flag
    setFlag(4, temp&(1<<dstSize)); // Sign flag
    setFlag(8, Ring0::bitcnt(temp)>dstSize); // Overflow flag
}

void Ring2::TEST(int dest, int src){
    /*The OF and CF flags are set to 0. The SF, ZF, and PF flags are set according to the result.*/
    if(dest == 1 || src == 0) throw Exception::BadInstructionException();
    unsigned long long temp2 = getRegister(src);
    unsigned long long temp3 = getRegister(dest);
    unsigned long long temp = temp3&temp2;
    int dstSize = getRegSize(dest);
    setFlag(0, 0); // Carry flag
    setFlag(1, Ring0::popcnt(temp)%2==0); // Parity flag
    setFlag(3, temp==0); // Zero flag
    setFlag(4, temp&(1<<dstSize)); // Sign flag
    setFlag(8, 0); // Overflow flag
}

void Ring2::JMP(int dest){
    setPointer(getRegister(dest));
}

void Ring2::JE(int dest){
    if(getFlag(3)) setPointer(getRegister(dest));
}

void Ring2::JNE(int dest){
    if(!getFlag(3)) setPointer(getRegister(dest));
}

void Ring2::JG(int dest){
    if(!getFlag(3) && getFlag(4)==getFlag(8)) setPointer(getRegister(dest));
}

void Ring2::JGE(int dest){
    if(getFlag(4)==getFlag(8)) setPointer(getRegister(dest));
}

void Ring2::JL(int dest){
    if(getFlag(4)!=getFlag(8)) setPointer(getRegister(dest));
}

void Ring2::JLE(int dest){
    if(getFlag(3) || getFlag(4)!=getFlag(8)) setPointer(getRegister(dest));
}

void Ring2::JZ(int dest){
    if(getFlag(3)) setPointer(getRegister(dest));
}

void Ring2::JNZ(int dest){
    if(!getFlag(3)) setPointer(getRegister(dest));
}

void Ring2::JP(int dest){
    if(getFlag(1)) setPointer(getRegister(dest));
}

void Ring2::JNP(int dest){
    if(!getFlag(1)) setPointer(getRegister(dest));
}

void Ring2::JPE(int dest){
    if(getFlag(1)) setPointer(getRegister(dest));
}

void Ring2::JPO(int dest){
    if(!getFlag(1)) setPointer(getRegister(dest));
}

void Ring2::JO(int dest){
    if(getFlag(8)) setPointer(getRegister(dest));
}

void Ring2::JNO(int dest){
    if(!getFlag(8)) setPointer(getRegister(dest));
}

void Ring2::JC(int dest){
    if(getFlag(0)) setPointer(getRegister(dest));
}

void Ring2::JNC(int dest){
    if(!getFlag(0)) setPointer(getRegister(dest));
}

void Ring2::JS(int dest){
    if(getFlag(4)) setPointer(getRegister(dest));
}

void Ring2::JNS(int dest){
    if(!getFlag(4)) setPointer(getRegister(dest));
}

void Ring2::LOOP(int dest){
    unsigned long long temp = getRegister(3000);
    if(temp>1){
        setRegister(3000, temp-1);
        setPointer(getRegister(dest));
    }
    else setRegister(3000, 0);
}

void Ring2::LOOPE(int dest){
    unsigned long long temp = getRegister(3000);
    if(temp>1 && getFlag(3)){
        setRegister(3000, temp-1);
        setPointer(getRegister(dest));
    }
    else setRegister(3000, 0);
}

void Ring2::LOOPNE(int dest){
    unsigned long long temp = getRegister(3000);
    if(temp>1 && !getFlag(3)){
        setRegister(3000, temp-1);
        setPointer(getRegister(dest));
    }
    else setRegister(3000, 0);
}

void Ring2::RET(){
    setPointer(pop());
}