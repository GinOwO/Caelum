#include "ihandler.h"

#include "ctexcept.h"
#include "ring4.h"

#include <functional>
#include <string>
#include <unordered_map>

InstructionHandler::InstructionHandler(
    std::function<void(int, unsigned long long)> setReg,
    std::function<unsigned long long(int)> getReg,
    std::function<void(int, bool)> setFlg,
    std::function<bool(int)> getFlg,
    std::function<void(size_t)> setPtr,
    std::function<void(unsigned long long)> pushFn,
    std::function<unsigned long long()> popFn,
    std::function<int(int)> getRegSz,
    std::function<size_t()> getPtr
) : Ring4(setReg, getReg, setFlg, getFlg, setPtr, pushFn, popFn, getRegSz){
    instructionMap2 = std::unordered_map<int, std::function<void(int, int)>>{
        {3000, std::bind(&InstructionHandler::MOV,    this, std::placeholders::_1, std::placeholders::_2)},
        {3001, std::bind(&InstructionHandler::XCHG,   this, std::placeholders::_1, std::placeholders::_2)},
        {3002, std::bind(&InstructionHandler::LEA,    this, std::placeholders::_1, std::placeholders::_2)},
        {3003, std::bind(&InstructionHandler::ADD,    this, std::placeholders::_1, std::placeholders::_2)},
        {3004, std::bind(&InstructionHandler::SUB,    this, std::placeholders::_1, std::placeholders::_2)},
        {3005, std::bind(&InstructionHandler::AND,    this, std::placeholders::_1, std::placeholders::_2)},
        {3006, std::bind(&InstructionHandler::ANDN,   this, std::placeholders::_1, std::placeholders::_2)},
        {3007, std::bind(&InstructionHandler::OR,     this, std::placeholders::_1, std::placeholders::_2)},
        {3009, std::bind(&InstructionHandler::XOR,    this, std::placeholders::_1, std::placeholders::_2)},
        {3010, std::bind(&InstructionHandler::ADC,    this, std::placeholders::_1, std::placeholders::_2)},
        {3011, std::bind(&InstructionHandler::SBB,    this, std::placeholders::_1, std::placeholders::_2)},
        {3012, std::bind(&InstructionHandler::SHL,    this, std::placeholders::_1, std::placeholders::_2)},
        {3013, std::bind(&InstructionHandler::SHR,    this, std::placeholders::_1, std::placeholders::_2)},
        {3014, std::bind(&InstructionHandler::SAL,    this, std::placeholders::_1, std::placeholders::_2)},
        {3015, std::bind(&InstructionHandler::SAR,    this, std::placeholders::_1, std::placeholders::_2)},
        {3016, std::bind(&InstructionHandler::ROL,    this, std::placeholders::_1, std::placeholders::_2)},
        {3017, std::bind(&InstructionHandler::ROR,    this, std::placeholders::_1, std::placeholders::_2)},
        {3018, std::bind(&InstructionHandler::RCL,    this, std::placeholders::_1, std::placeholders::_2)},
        {3019, std::bind(&InstructionHandler::RCR,    this, std::placeholders::_1, std::placeholders::_2)},
        {3020, std::bind(&InstructionHandler::CMP,    this, std::placeholders::_1, std::placeholders::_2)},
        {3021, std::bind(&InstructionHandler::TEST,   this, std::placeholders::_1, std::placeholders::_2)},
        {3022, std::bind(&InstructionHandler::ADCX,   this, std::placeholders::_1, std::placeholders::_2)},
        {3023, std::bind(&InstructionHandler::SBBX,   this, std::placeholders::_1, std::placeholders::_2)},
        {3024, std::bind(&InstructionHandler::SHLX,   this, std::placeholders::_1, std::placeholders::_2)},
        {3025, std::bind(&InstructionHandler::SHRX,   this, std::placeholders::_1, std::placeholders::_2)},
        {3026, std::bind(&InstructionHandler::SALX,   this, std::placeholders::_1, std::placeholders::_2)},
        {3027, std::bind(&InstructionHandler::SARX,   this, std::placeholders::_1, std::placeholders::_2)},
        {3028, std::bind(&InstructionHandler::RORX,   this, std::placeholders::_1, std::placeholders::_2)},
    };

    instructionMap1 = std::unordered_map<int, std::function<void(int)>>{
        {2000, std::bind(&InstructionHandler::INC,    this, std::placeholders::_1)},
        {2001, std::bind(&InstructionHandler::DEC,    this, std::placeholders::_1)},
        {2002, std::bind(&InstructionHandler::MUL,    this, std::placeholders::_1)},
        {2003, std::bind(&InstructionHandler::DIV,    this, std::placeholders::_1)},
        {2004, std::bind(&InstructionHandler::NOT,    this, std::placeholders::_1)},
        {2005, std::bind(&InstructionHandler::JMP,    this, std::placeholders::_1)},
        {2006, std::bind(&InstructionHandler::JE,     this, std::placeholders::_1)},
        {2007, std::bind(&InstructionHandler::JNE,    this, std::placeholders::_1)},
        {2008, std::bind(&InstructionHandler::JG,     this, std::placeholders::_1)},
        {2009, std::bind(&InstructionHandler::JGE,    this, std::placeholders::_1)},
        {2010, std::bind(&InstructionHandler::JL,     this, std::placeholders::_1)},
        {2011, std::bind(&InstructionHandler::JLE,    this, std::placeholders::_1)},
        {2012, std::bind(&InstructionHandler::JZ,     this, std::placeholders::_1)},
        {2013, std::bind(&InstructionHandler::JNZ,    this, std::placeholders::_1)},
        {2014, std::bind(&InstructionHandler::JP,     this, std::placeholders::_1)},
        {2015, std::bind(&InstructionHandler::JNP,    this, std::placeholders::_1)},
        {2016, std::bind(&InstructionHandler::JPE,    this, std::placeholders::_1)},
        {2017, std::bind(&InstructionHandler::JPO,    this, std::placeholders::_1)},
        {2018, std::bind(&InstructionHandler::JO,     this, std::placeholders::_1)},
        {2019, std::bind(&InstructionHandler::JNO,    this, std::placeholders::_1)},
        {2020, std::bind(&InstructionHandler::JC,     this, std::placeholders::_1)},
        {2021, std::bind(&InstructionHandler::JNC,    this, std::placeholders::_1)},
        {2022, std::bind(&InstructionHandler::JS,     this, std::placeholders::_1)},
        {2023, std::bind(&InstructionHandler::JNS,    this, std::placeholders::_1)},
        {2024, std::bind(&InstructionHandler::LOOP,   this, std::placeholders::_1)},
        {2025, std::bind(&InstructionHandler::LOOPE,  this, std::placeholders::_1)},
        {2026, std::bind(&InstructionHandler::LOOPNE, this, std::placeholders::_1)},
        {2027, std::bind(&InstructionHandler::CALL,   this, std::placeholders::_1)},
        {2028, std::bind(&InstructionHandler::PUSH,   this, std::placeholders::_1)},
        {2029, std::bind(&InstructionHandler::POP,    this, std::placeholders::_1)},
    };

    instructionMap0 = std::unordered_map<int, std::function<void()>>{
        {1000, std::bind(&InstructionHandler::NOP,    this)},
        {1001, std::bind(&InstructionHandler::HLT,    this)},
        {1002, std::bind(&InstructionHandler::RET,    this)},
        {1003, std::bind(&InstructionHandler::AAA,    this)},
        {1004, std::bind(&InstructionHandler::AAM,    this)},
        {1005, std::bind(&InstructionHandler::AAD,    this)},
        {1006, std::bind(&InstructionHandler::DAA,    this)},
        {1007, std::bind(&InstructionHandler::DAS,    this)},



        // Ring 4
        {1500, std::bind(&InstructionHandler::PUTS,   this)},
        {1501, std::bind(&InstructionHandler::FLUSH,  this)},
    };
    this->getPointer = getPtr;
}

void InstructionHandler::execute(const std::vector<int>& instruction){
    setPointer(getPointer() + 1);
    if(instructionMap2.count(instruction[0]))
        instructionMap2.at(instruction[0])(instruction[1], instruction[2]);
    else if(instructionMap1.count(instruction[0]))
        instructionMap1.at(instruction[0])(instruction[1]);
    else if(instructionMap0.count(instruction[0]))
        instructionMap0.at(instruction[0])();
}

void InstructionHandler::CALL(int dest){
    push(getPointer());
    setPointer(getRegister(dest));
}