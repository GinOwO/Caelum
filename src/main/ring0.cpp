#include "ring0.h"

void Ring0::MOV(int dest, int src){
    setRegister(dest, getRegister(src));
}

int Ring0::popcnt(unsigned long long num) {
    int count = 0;
    while(num){
        num &= (num - 1);
        count++;
    }
    return count;
}

int Ring0::bitcnt(unsigned long long num){
    int count = 0;
    while(num){
        num >>= 1;
        count++;
    }
    return count;
}

inline unsigned long long Ring0::bitset(int n){
    return n==0 ? 0: n==8 ? 0xFF: n==16 ? 0xFFFF: n==32 ? 0xFFFFFFFF: n==64 ? 0xFFFFFFFFFFFFFFFF: 0;
}

void Ring0::XCHG(int dest, int src){
    if(dest == 1 || src == 0) throw Exception::BadInstructionException();
    unsigned long long temp = getRegister(dest);
    setRegister(dest, getRegister(src));
    setRegister(src, temp);
}

void Ring0::LEA(int dest, int src){
    if(dest == 1) throw Exception::BadInstructionException();
    setRegister(dest, getRegister(src));
}

void Ring0::INC(int dest){
    /*The CF flag is not affected. The AF, OF, SF, ZF, and PF flags are set according to the result.*/
    if(dest == 1) throw Exception::BadInstructionException();
    unsigned long long temp2 = getRegister(dest);
    unsigned long long temp = temp2 + 1;
    int dstSize = getRegSize(dest);
    setFlag(1, Ring0::popcnt(temp)%2==0); // Parity flag
    if(dstSize==8) setFlag(2, ((temp2&0xF)+1)&0x10); // Aux Carry Flag
    setFlag(3, temp==0); // Zero flag
    setFlag(4, temp&(1<<dstSize)); // Sign flag
    setFlag(8, Ring0::bitcnt(temp)>dstSize); // Overflow flag
    setRegister(dest, temp);
}

void Ring0::ADD(int dest, int src){
    /*The AF, OF, SF, ZF, CF, and PF flags are set according to the result.*/
    if(dest == 1) throw Exception::BadInstructionException();
    unsigned long long temp2 = getRegister(src);
    unsigned long long temp3 = getRegister(dest);
    unsigned long long temp = temp3+temp2;
    int dstSize = getRegSize(dest);
    setFlag(0, (temp>>(dstSize-1))!=(temp3>>(dstSize-1)) || temp<temp3); // Carry flag
    setFlag(1, Ring0::popcnt(temp)%2==0); // Parity flag
    if(dstSize==8) setFlag(2, ((temp2&0xF)+(temp3&0xF))&0x10); // Aux Carry Flag
    setFlag(3, temp==0); // Zero flag
    setFlag(4, temp&(1<<dstSize)); // Sign flag
    setFlag(8, Ring0::bitcnt(temp)>dstSize); // Overflow flag
    setRegister(dest, temp);
}

void Ring0::DEC(int dest){
    /*The CF flag is not affected. The AF, OF, SF, ZF, and PF flags are set according to the result.*/
    if(dest == 1) throw Exception::BadInstructionException();
    unsigned long long temp2 = getRegister(dest);
    unsigned long long temp = temp2 - 1;
    int dstSize = getRegSize(dest);
    setFlag(0, (temp>>(dstSize-1))!=(temp2>>(dstSize-1)) || temp>temp2); // Carry flag
    setFlag(1, Ring0::popcnt(temp)%2==0); // Parity flag
    if(dstSize==8) setFlag(2, ((temp2&0xF)-1)&0x10); // Aux Carry Flag
    setFlag(3, temp==0); // Zero flag
    setFlag(4, temp&(1<<dstSize)); // Sign flag
    setFlag(8, Ring0::bitcnt(temp)>dstSize); // Overflow flag
    setRegister(dest, temp);
}

void Ring0::SUB(int dest, int src){
    /*The AF, OF, SF, ZF, CF, and PF flags are set according to the result.*/
    if(dest == 1) throw Exception::BadInstructionException();
    unsigned long long temp2 = getRegister(src);
    unsigned long long temp3 = getRegister(dest);
    unsigned long long temp = temp3-temp2;
    int dstSize = getRegSize(dest);
    setFlag(0, (temp>>(dstSize-1))!=(temp3>>(dstSize-1)) || temp>temp3); // Carry flag
    setFlag(1, Ring0::popcnt(temp)%2==0); // Parity flag
    if(dstSize==8) setFlag(2, ((temp3&0xF)-(temp2&0xF))&0x10); // Aux Carry Flag
    setFlag(3, temp==0); // Zero flag
    setFlag(4, temp&(1<<dstSize)); // Sign flag
    setFlag(8, Ring0::bitcnt(temp)>dstSize); // Overflow flag
    setRegister(dest, temp);
}

void Ring0::MUL(int src){
    /*The OF and CF flags are set to 0 if the upper half of the result is 0; otherwise, they are set to 1. The SF, ZF and PF flags are undefined.*/
    int srcSize = getRegSize(src);
    int dest = (srcSize==8)? 1111 : (srcSize==16)? 1110 : (srcSize==32)? 1100 : 1000;
    int dstSize = getRegSize(dest);
    unsigned long long temp1 = getRegister(dest);
    unsigned long long temp2 = getRegister(src);
    unsigned long long temp3 = temp1*temp2;
    setFlag(0, temp3>>(dstSize>>1)); // Carry flag
    setFlag(8, temp3>>(dstSize>>1)); // Overflow flag
    setRegister(dest, temp3);
}

void Ring0::DIV(int src){
    /*The CF, OF, SF, ZF and PF flags are undefined.*/
    int srcSize = getRegSize(src);
    int dest = (srcSize==8)? 1111 : (srcSize==16)? 1110 : (srcSize==32)? 1100 : 1000;
    unsigned long long temp1 = getRegister(dest);
    unsigned long long temp2 = getRegister(src);
    setRegister(dest, temp1/temp2);
    dest = (srcSize==8)? 4111 : (srcSize==16)? 4110 : (srcSize==32)? 4100 : 4000;
    setRegister(dest, temp1%temp2);
}

void Ring0::AND(int dest, int src){
    /*The OF and CF flags are cleared; the SF, ZF, and PF flags are set according to the result*/
    if(dest == 1) throw Exception::BadInstructionException();
    unsigned long long temp = getRegister(dest) & getRegister(src);
    int dstSize = getRegSize(dest);
    setFlag(0, false); // Carry flag
    setFlag(1, Ring0::popcnt(temp)%2==0); // Parity flag
    setFlag(3, temp==0); // Zero flag
    setFlag(4, temp&(1<<dstSize)); // Sign flag
    setFlag(8, false); // Overflow flag
    setRegister(dest, temp);
}

void Ring0::ANDN(int dest, int src){
    /*The OF and CF flags are cleared; the SF, ZF, and PF flags are set according to the result*/
    if(dest == 1) throw Exception::BadInstructionException();
    unsigned long long temp = getRegister(dest) & getRegister(src);
    int dstSize = getRegSize(dest);
    setFlag(0, false); // Carry flag
    setFlag(1, Ring0::popcnt(temp)%2==0); // Parity flag
    setFlag(3, temp==0); // Zero flag
    setFlag(4, temp&(1<<dstSize)); // Sign flag
    setFlag(8, false); // Overflow flag
    setRegister(dest, temp);
}

void Ring0::OR(int dest, int src){
    /*The OF and CF flags are cleared; the SF, ZF, and PF flags are set according to the result*/
    if(dest == 1) throw Exception::BadInstructionException();
    unsigned long long temp = getRegister(dest) | getRegister(src);
    int dstSize = getRegSize(dest);
    setFlag(0, false); // Carry flag
    setFlag(1, Ring0::popcnt(temp)%2==0); // Parity flag
    setFlag(3, temp==0); // Zero flag
    setFlag(4, temp&(1<<dstSize)); // Sign flag
    setFlag(8, false); // Overflow flag
    setRegister(dest, temp);
}

void Ring0::XOR(int dest, int src){
    /*The OF and CF flags are cleared; the SF, ZF, and PF flags are set according to the result*/
    if(dest == 1) throw Exception::BadInstructionException();
    unsigned long long temp = getRegister(dest) ^ getRegister(src);
    int dstSize = getRegSize(dest);
    setFlag(0, false); // Carry flag
    setFlag(1, Ring0::popcnt(temp)%2==0); // Parity flag
    setFlag(3, temp==0); // Zero flag
    setFlag(4, temp&(1<<dstSize)); // Sign flag
    setFlag(8, false); // Overflow flag
    setRegister(dest, temp);
}

void Ring0::NOT(int dest){
    /*The OF, CF, SF, ZF, and PF flags are unaffected.*/
    if(dest == 1) throw Exception::BadInstructionException();
    unsigned long long temp = ~getRegister(dest);
    setRegister(dest, temp);
}

void Ring0::NOP(){
    /*Does nothing*/
}

void Ring0::HLT(){
    /*Halts the program*/
    throw Exception::HaltException();
}

