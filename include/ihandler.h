#ifndef INSTRUCTION_HANDLER_H
#define INSTRUCTION_HANDLER_H

#include "ring4.h"

#include <functional>
#include <string>
#include <vector>
#include <unordered_map>

class InstructionHandler : public Ring4{
    std::unordered_map<int, std::function<void(int, int)>> instructionMap2;
    std::unordered_map<int, std::function<void(int)>> instructionMap1;
    std::unordered_map<int, std::function<void()>> instructionMap0;

    std::function<size_t()> getPointer;

    void CALL(int);
public:
    InstructionHandler(
        std::function<void(int, unsigned long long)>& setReg,
        std::function<unsigned long long(int)>& getReg,
        std::function<void(int, bool)>& setFlg,
        std::function<bool(int)>& getFlg,
        std::function<void(size_t)>& setPtr,
        std::function<void(unsigned long long)>& pushFn,
        std::function<unsigned long long()>& popFn,
        std::function<int(int)>& getRegSz,
        std::function<size_t()>& getPtr,
        std::function<unsigned long long(unsigned long long)>& getMem,
        std::stringstream& out
    );

    ~InstructionHandler(){};
    

    void execute(const std::vector<int>&);
};

#endif
