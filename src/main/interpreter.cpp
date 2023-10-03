#include "interpreter.h"

#include "ctexcept.h"
#include "lexer.h"
#include "ihandler.h"

#include <string>
#include <sstream>
#include <vector>
#include <functional>

#define REGFLAGS(){ \
        R = (_reg/100)%10 == 0, \
        E = !R && (_reg/100)%10 == 1, \
        X = E &&(_reg/10)%10 == 1, \
        H = X && _reg%10 == 1, \
        L = !H && _reg%10 == 2; \
        bits = (H|L)? 8: X? 16 : E? 32 : 64;}

Interpreter::Interpreter(){
    this->registers = std::vector<std::vector<bool>>(18, std::vector<bool>(64, false));
    this->memory = std::vector<unsigned long long>(1<<20);
    this->flags = std::vector<bool>(9, false);

    std::function<void(int, unsigned long long)> setRegisterFn = std::bind(&Interpreter::setRegister, this, std::placeholders::_1, std::placeholders::_2);
    std::function<unsigned long long(int)> getRegisterFn = std::bind(&Interpreter::getRegister, this, std::placeholders::_1);
    std::function<void(int, bool)> setFlagFn = std::bind(&Interpreter::setFlag, this, std::placeholders::_1, std::placeholders::_2);
    std::function<bool(int)> getFlagFn = std::bind(&Interpreter::getFlag, this, std::placeholders::_1);
    std::function<int(int)> getRegSizeFn = std::bind(&Interpreter::getRegSize, this, std::placeholders::_1);
    std::function<void(unsigned long long)> pushFn = std::bind(&Interpreter::PUSH, this, std::placeholders::_1);
    std::function<unsigned long long()> popFn = std::bind(&Interpreter::POP, this);
    std::function<size_t()> getPtr = std::bind(&Interpreter::getPointer, this);
    std::function<void(size_t)> setPtr = std::bind(&Interpreter::setPointer, this, std::placeholders::_1);

    this->instrHandler = new InstructionHandler(setRegisterFn, getRegisterFn, setFlagFn, getFlagFn, setPtr, pushFn, popFn, getRegSizeFn, getPtr, output);
}

Interpreter::~Interpreter(){
    delete this->instrHandler;
}

void Interpreter::setRegister(int _reg, unsigned long long num){
    if(_reg == -1) throw Exception::BadRegisterException();
    int reg = _reg/1000 - 1, bits;
    bool R, E, X, H, L;
    REGFLAGS();
    if(_reg == 0){
        if(this->temp1Type == 0) this->memory[this->temp1] = num;
        else throw Exception::UnknownException();
    }
    else if(_reg == 1){
        if(this->temp2Type == 0) this->memory[this->temp2] = num;
        else throw Exception::UnknownException();
    }
    else{
        for(int i=bits-1; i>=0; i--, num>>=1){
            this->registers[reg][64-bits+i-8*H] = num&1;
        }
    }
}

unsigned long long Interpreter::getRegister(int _reg){
    if(_reg == -1) throw Exception::BadRegisterException();
    if(_reg == 0){
        if (this->temp1Type == 2) return this->temp1;
        else if(this->temp1Type == 0) return this->memory.at(this->temp1);
        else throw Exception::UnknownException();
    }
    else if(_reg == 1){
        if (this->temp2Type == 2) return this->temp2;
        else if(this->temp2Type == 0) return this->memory.at(this->temp2);
        else throw Exception::UnknownException();
    }
    unsigned long long num = 0; int reg = _reg/1000 - 1, bits;
    bool R, E, X, H, L;

    REGFLAGS();
    for(int i=64-bits; i<64; i++){
        num<<=1;
        num += this->registers[reg][i-8*H];
    }
    return num;
}

void Interpreter::setFlag(int _flag, bool val){
    if(_flag == -1) throw Exception::BadFlagException();
    this->flags[_flag] = val;
}

bool Interpreter::getFlag(int _flag){
    if(_flag == -1) throw Exception::BadFlagException();
    return this->flags[_flag];
}

int Interpreter::getRegSize(int _reg){
    if(_reg == -1) throw Exception::BadRegisterException();
    if(_reg == 0) return this->temp1Size;
    else if(_reg == 1) return this->temp2Size;
    int bits;
    bool R, E, X, H, L;
    REGFLAGS();
    return bits;
}

void Interpreter::PUSH(unsigned long long num){
    this->callStack.push(num);
}

unsigned long long Interpreter::POP(){
    if(callStack.empty()) throw Exception::StackUnderflowException();
    unsigned long long num = this->callStack.top();
    this->callStack.pop();
    return num;
}
/*
type: 
0 = mem location
1 = register
2 = immediate
3 = operator, only for tokens
*/
void Interpreter::execute(){
    if(code.size() == 0) throw Exception::BadSyntaxException("Code not built");
    size_t backup_ptr = ptr;
    this->output.clear();
    int cnt, i, type, lineNum, memTemp1, memTemp2;
    unsigned long long a, b, val;
    while(true){
        memTemp1=memTemp2=-1;
        std::vector<int> instr; i=0;
        if(ptr >= this->code.size()) throw Exception::BadSyntaxException("Code segment out of bounds");
        try{
            auto cur = this->code[ptr];
            lineNum = cur[i++];
            instr.push_back(cur[i++]);

            if(i<cur.size()){
                if(cur[i]==1){
                    instr.push_back(cur[i++]);
                    temp2Type = 0;
                    temp2Size = 64;
                    cnt = cur[i++];
                    std::stack<unsigned long long> stk;
                    for(int j=0; j<cnt; j++){
                        // Postfix eval
                        type = cur[i++];
                        val = cur[i++];
                        if(type==1) stk.push(this->getRegister(val));
                        else if(type==2) stk.push(val);
                        else if(type==3){
                            b = stk.top(); stk.pop();
                            a = stk.top(); stk.pop();
                            if(val==1) stk.push(a+b);
                            else if(val==2) stk.push(a-b);
                            else if(val==3) stk.push(a*b);
                            else if(val==4) stk.push(a/b);
                            else throw Exception::UnknownException();
                        }
                    }
                    memTemp2 = stk.top();
                    temp2 = getMemory(memTemp2);
                }
                else if(cur[i]==3){ // immediate
                    temp2Type = 2;
                    temp2Size = 64; 
                    i++;
                    temp2 = cur[i++];
                    instr.push_back(1);
                }
                else if(cur[i]==2){ // register
                    i++;
                    instr.push_back(cur[i++]);
                }
                else throw Exception::UnknownException();
            }
            if(i<cur.size()){
                if(cur[i]==0){
                    instr.push_back(cur[i++]);
                    temp1Type = 0;
                    temp1Size = 64;
                    cnt = cur[i++];
                    std::stack<unsigned long long> stk;
                    for(int j=0; j<cnt; j++){
                        // Postfix eval
                        type = cur[i++];
                        val = cur[i++];
                        if(type==1) stk.push(this->getRegister(val));
                        else if(type==2) stk.push(val);
                        else if(type==3){
                            b = stk.top(); stk.pop();
                            a = stk.top(); stk.pop();
                            if(val==1) stk.push(a+b);
                            else if(val==2) stk.push(a-b);
                            else if(val==3) stk.push(a*b);
                            else if(val==4) stk.push(a/b);
                            else throw Exception::UnknownException();
                        }
                    }
                    memTemp1 = stk.top();
                    temp1 = getMemory(memTemp1);
                }
                else if(cur[i]==3){ // immediate
                    temp1Type = 2;
                    temp1Size = 64; 
                    i++;
                    temp1 = cur[i++];
                    instr.push_back(0);
                }
                else if(cur[i]==2){ // register
                    i++;
                    instr.push_back(cur[i++]);
                }
                else throw Exception::UnknownException();
            }
            if(instr[0]==3002){
                setRegister(instr[1], temp1);
                ptr++;
            }
            else{
                this->instrHandler->execute(instr);
                if(memTemp2!=-1)
                    memory[memTemp2] = temp2;
                
            }
        }
        catch(Exception::HaltException){
            ptr = backup_ptr;
            return;
        }
        catch(Exception::BadInstructionException){
            ptr = backup_ptr;
            throw Exception::BadSyntaxException("Bad instruction at line "+std::to_string(lineNum));
        }
        catch(Exception::UnknownException){
            ptr = backup_ptr;
            throw Exception::BadSyntaxException("Unknown error at line "+std::to_string(lineNum));
        }
        catch(Exception::PointerOutOfBoundsException){
            ptr = backup_ptr;
            throw Exception::BadSyntaxException("Pointer out of bounds at line "+std::to_string(lineNum));
        }
        catch(Exception::StackUnderflowException){
            ptr = backup_ptr;
            throw Exception::BadSyntaxException("Popping from empty stack at line "+std::to_string(lineNum));
        }
    }
    ptr = backup_ptr;
}

void Interpreter::build(const std::string& instructions){
    size_t h = std::hash<std::string>{}(instructions);
    if(hash == h) return;
    this->code.clear();
    this->labelMap.clear();
    this->callStack = std::stack<unsigned long long>();
    unsigned long long i=0, j=0;
    std::string label, op, op1, op2, instr;
    std::stringstream ss(instructions);
    
    do std::getline(ss, instr, '\n'), j++; 
    while(lexer.isEmptyLine(instr));
    
    instr = lexer.isGlobal(instr);
    if(instr == "") throw Exception::MissingGlobalException();
    
    std::transform(instr.begin(), instr.end(), instr.begin(),
        [](unsigned char c){ return std::tolower(c);});
    if(!this->lexer.isValidLabel(instr)) throw Exception::BadLabelException();
    ptr = std::hash<std::string>{}(instr);
    try{
        while(j++ && std::getline(ss, instr, '\n')){
            if(lexer.isEmptyLine(instr)) continue;
            this->lexer.parse(instr, label, op, op1, op2);
            if(instr.empty() && op.empty()) throw Exception::BadInstructionException();
            if(label != ""){
                std::transform(label.begin(), label.end(), label.begin(),
                    [](unsigned char c){ return std::tolower(c);});
                if(this->labelMap.count(std::hash<std::string>{}(label)))
                    throw Exception::LabelRedefinedException();
                if(!this->lexer.isValidLabel(label))
                    throw Exception::BadLabelException();
                this->labelMap[std::hash<std::string>{}(label)] = i;
                if(op == "") continue;
            }
            std::vector<unsigned long long> tokens{j};
            if(op != ""){
                int mne = this->lexer.identifyMnemonic(op), t;
                if(mne == -1) throw Exception::BadInstructionException();
                switch(mne/1000){
                    case 1: if(op1 != "" || op2 != "") throw Exception::BadInstructionException(); break;
                    case 2: if(op1 == "" || op2 != "") throw Exception::BadInstructionException(); break;
                    case 3: if(op1 == "" || op2 == "" || lexer.isNumeric(op1) || lexer.isHex(op1)) 
                        throw Exception::BadInstructionException();
                    default: break;
                }
                tokens.push_back(mne);
                if(op1 != ""){
                    if(mne>=2005 && mne<=2027){
                        std::transform(op1.begin(), op1.end(), op1.begin(),
                            [](unsigned char c){ return std::tolower(c);});
                        size_t t = std::hash<std::string>{}(op1);
                        tokens.push_back(3);
                        tokens.push_back(t);
                    }
                    else if(lexer.isSubExpr(op1)){
                        tokens.push_back(1);
                        auto subExpr = lexer.tokenize(op1);
                        tokens.push_back(subExpr.size()/2);
                        for(auto&token:subExpr) tokens.push_back(token);
                    }
                    else{
                        if(lexer.isNumeric(op1)||lexer.isHex(op1)) tokens.push_back(3);
                        else tokens.push_back(2);
                        tokens.push_back(lexer.convert(op1));
                    }
                }
                if(op2 != ""){
                    if(lexer.isSubExpr(op2)){
                        tokens.push_back(0);
                        auto subExpr = lexer.tokenize(op2);
                        tokens.push_back(subExpr.size()/2);
                        for(auto&token:subExpr) tokens.push_back(token);
                    }
                    else{
                        if(lexer.isNumeric(op2)||lexer.isHex(op2)) tokens.push_back(3);
                        else tokens.push_back(2);
                        tokens.push_back(lexer.convert(op2));
                    }
                }
            }
            this->code.push_back(tokens);
            i++;
        }
        if(!this->labelMap.count(ptr)) throw Exception::MissingGlobalException();
        ptr = this->labelMap[ptr]; // TODO backup ptr for next run
        this->resolveLabels();
        hash = h;
    }
    catch(Exception::BadLabelException){
        throw Exception::BadSyntaxException("Bad label at line "+std::to_string(j));
    }
    //catch(Exception::BadInstructionException){
    //    throw Exception::BadSyntaxException("Bad instruction at line "+std::to_string(j));
    //}
    catch(Exception::LabelRedefinedException){
        throw Exception::BadSyntaxException("Label redefined at line "+std::to_string(j));
    }
    catch(Exception::UnknownException){
        throw Exception::BadSyntaxException("Unknown error at line "+std::to_string(j));
    }
    catch(Exception::MissingGlobalException){
        throw Exception::BadSyntaxException("Global should be first line or label not found");
    }
}

void Interpreter::resolveLabels(){
    for(auto&instr:this->code){
        int mne = instr[1];
        if(mne>=2005 && mne<=2027){
            if(this->labelMap.count(instr[3])) 
                instr[3] = this->labelMap[instr[3]];
            else throw Exception::BadLabelException();
        }
    }
}

void Interpreter::setPointer(size_t _ptr){
    if(_ptr<=this->code.size()) ptr = _ptr;
    else throw Exception::PointerOutOfBoundsException();
}

size_t Interpreter::getPointer() noexcept{
    return ptr;
}

unsigned long long Interpreter::getMemory(unsigned long long addr){
    if(addr >= this->memory.size()) throw Exception::BadMemoryException();
    return this->memory[addr];
}

void Interpreter::setMemory(unsigned long long a, unsigned long long b){
    this->memory[a] = b;
}
