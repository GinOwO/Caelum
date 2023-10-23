#include "ring4.h"

void Ring4::PUTU(){
    OUT << (unsigned long long) getRegister(1000);
}

void Ring4::PUTI(){
    OUT << (signed long long) getRegister(1000);
}

void Ring4::PUTC(){
    OUT << (unsigned char) getRegister(1112);
}

void Ring4::PUTD(){
    OUT << (double) getRegister(1000);
}

void Ring4::PUTCH(unsigned char c){
    OUT << (unsigned char) this->getRegister(c);
}

void Ring4::PUTS(unsigned int addr){
    unsigned char n;
    addr = this->getRegister(addr);
    while((n = this->getMem(addr++))) OUT << n;
}

void Ring4::FLUSH(){
    OUT.flush();
}
