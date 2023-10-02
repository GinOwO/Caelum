#include "ring4.h"

void Ring4::PUTS(){
    OUT << getRegister(1000) << '\n';
}

void Ring4::FLUSH(){
    OUT.flush();
}