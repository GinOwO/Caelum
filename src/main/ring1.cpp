#include "ring1.h"

void Ring1::ADC(int dest, int src){
    /*The OF, SF, ZF, CF, and PF flags are set according to the result.*/
    if(dest == 1) throw Exception::BadInstructionException();
    this->ADD(dest, src+getFlag(0));
}

void Ring1::SBB(int dest, int src){
    /*The OF, SF, ZF, CF, and PF flags are set according to the result.*/
    if(dest == 1) throw Exception::BadInstructionException();
    this->SUB(dest, src+getFlag(0));
}

void Ring1::AAA(){
    /*The AF, CF flags are set to 1 if the adjustment results in a decimal carry; otherwise they are set to 0.*/
    unsigned temp = getRegister(1112);
    if((temp&0x0F)>9 || getFlag(2)){
        setRegister(1110, temp+0x106);
        setFlag(0, true);
        setFlag(2, true);
    }
    else{
        setFlag(0, false);
        setFlag(2, false);
    }
    setRegister(1112, getRegister(1112)&0x0F);

}

void Ring1::AAS(){
    /*The AF, CF flags are set to 1 if the adjustment results in a decimal carry; otherwise they are set to 0.*/
    unsigned temp = getRegister(1112);
    if((temp&0x0F)>9 || getFlag(2)){
        setRegister(1110, temp-6);
        setRegister(1111, getRegister(1111)-1);
        setFlag(0, true);
        setFlag(2, true);
    }
    else{
        setFlag(0, false);
        setFlag(2, false);
        setRegister(1112, getRegister(1112)&0x0F);
    }
}

void Ring1::AAM(){
    /*The SF, ZF, and PF flags are set according to the resulting binary value in the AL register.*/
    unsigned temp = getRegister(1112);
    setRegister(1111, temp/0x0A);
    temp = temp%0x0A;
    setRegister(1112, temp);
    setFlag(1, Ring0::popcnt(temp)%2==0);
    setFlag(3, temp==0);
    setFlag(4, temp&0x80);
}

void Ring1::AAD(){
    /*The SF, ZF, and PF flags are set according to the resulting binary value in the AL register.*/
    unsigned temp0 = getRegister(1112), temp1 = getRegister(1111);
    unsigned temp = (temp0 + (temp1*0x0A))&0xFF;
    setRegister(1112, temp);
    setRegister(1111, 0);
    temp = getRegister(1112);
    setFlag(1, Ring0::popcnt(temp)%2==0);
    setFlag(3, temp==0);
    setFlag(4, temp&0x80);
}

void Ring1::DAA(){
    /*The CF and AF flags are set if the adjustment of the value results in a decimal carry in either digit of the result. 
    The SF, ZF, and PF flags are set according to the result. The OF flag is undefined.*/
    unsigned tempAL = getRegister(1112);
    unsigned tempCF = getFlag(0);
    setFlag(0, false);  
    if((tempAL&0x0F)>9 || getFlag(2)){
        setRegister(1112, tempAL+6);
        setFlag(0, tempCF || ((tempAL&0x80==1) && ((tempAL+6)&&0x80==0)));
        setFlag(2, true);
    } else setFlag(2, false);
    if(tempAL>0x99 || tempCF){
        setRegister(1112, tempAL+0x60);
        setFlag(0, true);
    } else setFlag(0, false);
    tempAL = getRegister(1112);
    setFlag(1, Ring0::popcnt(tempAL)%2==0);
    setFlag(3, tempAL==0);
    setFlag(4, tempAL&0x80);
}

void Ring1::DAS(){
    /*The CF and AF flags are set if the adjustment of the value results in a decimal carry in either digit of the result. 
    The SF, ZF, and PF flags are set according to the result. The OF flag is undefined.*/
    unsigned tempAL = getRegister(1112);
    unsigned tempCF = getFlag(0);
    setFlag(0, false);
    if((tempAL&0x0F)>9 || getFlag(2)){
        setRegister(1112, tempAL-6);
        setFlag(0, tempCF || ((tempAL&0x80==0) && ((tempAL-6)&&0x80==1)));
        setFlag(2, true);
    } else setFlag(2, false);
    if(tempAL>0x99 || tempCF){
        setRegister(1112, tempAL-0x60);
        setFlag(0, true);
    } else setFlag(0, false);
}

void Ring1::SHL(int dest, int src){
    /*The CF flag contains the value of the last bit shifted out of the destination operand; 
    it is undefined for SHL instructions where the count is greater than or equal to the size (in bits) of the destination operand. 
    The OF flag is affected only for 1-bit shifts; otherwise, it is undefined. 
    The SF, ZF, and PF flags are set according to the result. 
    If the count is 0, the flags are not affected. For a non-zero count, the AF flag is undefined.*/
    if(dest == 1) throw Exception::BadInstructionException();
    unsigned long long temp = getRegister(dest);
    unsigned long long cnt = getRegister(src);
    if(cnt == 0) return;
    int dstSize = getRegSize(dest);
    unsigned long long msb = 1<<dstSize;
    if(cnt == 1){
        bool t = temp&msb;
        setFlag(0, t);
        temp<<=1;
        setFlag(8, (temp&msb)==t);
    }
    else{
        while(cnt--){
            setFlag(0, temp&msb);
            temp <<= 1;
        }
    }
    setRegister(dest, temp);
    temp = getRegister(dest);
    setFlag(1, Ring0::popcnt(temp)%2==0);
    setFlag(3, temp==0);
    setFlag(4, temp&msb);
}

void Ring1::SHR(int dest, int src){
    /*The CF flag contains the value of the last bit shifted out of the destination operand; 
    it is undefined for SHL instructions where the count is greater than or equal to the size (in bits) of the destination operand. 
    The OF flag is affected only for 1-bit shifts; otherwise, it is undefined. 
    The SF, ZF, and PF flags are set according to the result. 
    If the count is 0, the flags are not affected. For a non-zero count, the AF flag is undefined.*/
    if(dest == 1) throw Exception::BadInstructionException();
    unsigned long long temp = getRegister(dest);
    unsigned long long cnt = getRegister(src);
    if(cnt == 0) return;
    int dstSize = getRegSize(dest);
    unsigned long long msb = 1<<dstSize;
    if(cnt == 1){
        setFlag(0, temp&1);
        setFlag(8, temp&msb);
        temp>>=1;
    }
    else{
        while(cnt--){
            setFlag(0, temp&1);
            temp >>= 1;
        }
    }
    setRegister(dest, temp);
    temp = getRegister(dest);
    setFlag(1, Ring0::popcnt(temp)%2==0);
    setFlag(3, temp==0);
    setFlag(4, temp&msb);
}

void Ring1::SAL(int dest, int src){
    /*Same as SHL*/
    this->SHL(dest, src);
}

void Ring1::SAR(int dest, int src){
    /*The CF flag contains the value of the last bit shifted out of the destination operand; 
    it is undefined for SHL instructions where the count is greater than or equal to the size (in bits) of the destination operand. 
    The OF flag is affected only for 1-bit shifts; otherwise, it is undefined. 
    The SF, ZF, and PF flags are set according to the result. 
    If the count is 0, the flags are not affected. For a non-zero count, the AF flag is undefined.*/
    if(dest == 1) throw Exception::BadInstructionException();
    unsigned long long temp = getRegister(dest);
    unsigned long long cnt = getRegister(src);
    if(cnt == 0) return;
    int dstSize = getRegSize(dest);
    unsigned long long msb = 1<<dstSize;
    bool t = temp&msb;
    if(cnt == 1){
        setFlag(8, false);
        setFlag(0, t);
        temp>>=1;
        if(t) temp |= msb;
    }
    else{
        while(cnt--){
            setFlag(0, temp&msb);
            temp >>= 1;
            if(t) temp |= msb;
        }
    }
    setRegister(dest, temp);
    temp = getRegister(dest);
    setFlag(1, Ring0::popcnt(temp)%2==0);
    setFlag(3, temp==0);
    setFlag(4, temp&msb);
}

void Ring1::ROL(int dest, int src){
    /*For RCL and RCR instructions, a zero-bit rotate does nothing, i.e., 
    affects no flags. For ROL and ROR instructions, if the masked count 
    is 0, the flags are not affected. If the masked count is 1, then the 
    OF flag is affected, otherwise (masked count is greater than 1) the 
    OF flag is undefined. For all instructions, the CF flag is affected 
    when the masked count is non-zero. The SF, ZF, AF, and PF flags are 
    always unaffected.*/
    if(dest == 1) throw Exception::BadInstructionException();
    unsigned long long temp = getRegister(dest);
    unsigned long long cnt = getRegister(src);
    if(cnt == 0) return;
    int dstSize = getRegSize(dest);
    unsigned long long msb = 1<<dstSize;
    bool t;
    setFlag(0, false);
    if(cnt==1){
        t = temp&msb;
        temp <<= 1;
        temp |= t;
        setFlag(8, (temp&msb)^t);
    }
    else{
        while(cnt--){
            t = temp&msb;
            temp <<= 1;
            temp |= t;
        }
    }
    setFlag(0, t);
    setRegister(dest, temp);
}

void Ring1::ROR(int dest, int src){
    /*For RCL and RCR instructions, a zero-bit rotate does nothing, i.e., 
    affects no flags. For ROL and ROR instructions, if the masked count 
    is 0, the flags are not affected. If the masked count is 1, then the 
    OF flag is affected, otherwise (masked count is greater than 1) the 
    OF flag is undefined. For all instructions, the CF flag is affected 
    when the masked count is non-zero. The SF, ZF, AF, and PF flags are 
    always unaffected.*/
    if(dest == 1) throw Exception::BadInstructionException();
    unsigned long long temp = getRegister(dest);
    unsigned long long cnt = getRegister(src);
    if(cnt == 0) return;
    int dstSize = getRegSize(dest);
    unsigned long long msb = 1<<dstSize;
    bool t;
    setFlag(0, false);
    if(cnt==1){
        t = temp&1;
        temp >>= 1;
        if(t) temp |= msb;
        setFlag(8, (temp&msb)^((temp<<1)&msb));
    }
    else{
        while(cnt--){
            t = temp&msb;
            temp >>= 1;
            if(t) temp |= msb;
        }
    }
    setFlag(0, t);
    setRegister(dest, temp);
}

void Ring1::RCL(int dest, int src){
    /*For RCL and RCR instructions, a zero-bit rotate does nothing, i.e., 
    affects no flags. For ROL and ROR instructions, if the masked count 
    is 0, the flags are not affected. If the masked count is 1, then the 
    OF flag is affected, otherwise (masked count is greater than 1) the 
    OF flag is undefined. For all instructions, the CF flag is affected 
    when the masked count is non-zero. The SF, ZF, AF, and PF flags are 
    always unaffected.*/
    if(dest == 1) throw Exception::BadInstructionException();
    unsigned long long temp = getRegister(dest);
    unsigned long long cnt = getRegister(src);
    if(cnt == 0) return;
    int dstSize = getRegSize(dest);
    unsigned long long msb = 1<<dstSize;
    bool t;
    if(cnt==1){
        t = temp&msb;
        temp <<= 1;
        temp |= getFlag(0);
        setFlag(0, t);
        setFlag(8, (temp&msb)^t);
    }
    else{
        while(cnt--){
            t = temp&msb;
            temp <<= 1;
            temp |= getFlag(0);
            setFlag(0, t);
        }
    }
    setRegister(dest, temp);
}

void Ring1::RCR(int dest, int src){
    /*For RCL and RCR instructions, a zero-bit rotate does nothing, i.e., 
    affects no flags. For ROL and ROR instructions, if the masked count 
    is 0, the flags are not affected. If the masked count is 1, then the 
    OF flag is affected, otherwise (masked count is greater than 1) the 
    OF flag is undefined. For all instructions, the CF flag is affected 
    when the masked count is non-zero. The SF, ZF, AF, and PF flags are 
    always unaffected.*/
    if(dest == 1) throw Exception::BadInstructionException();
    unsigned long long temp = getRegister(dest);
    unsigned long long cnt = getRegister(src);
    if(cnt == 0) return;
    int dstSize = getRegSize(dest);
    unsigned long long msb = 1<<dstSize;
    bool t;
    setFlag(0, false);
    if(cnt==1){
        t = temp&1;
        temp >>= 1;
        if(getFlag(0)) temp |= msb;
        setFlag(0, t);
        setFlag(8, (temp&msb)^((temp<<1)&msb));
    }
    else{
        while(cnt--){
            t = temp&1;
            temp >>= 1;
            if(getFlag(0)) temp |= msb;
            setFlag(0, t);
        }
    }
    setRegister(dest, temp);
}