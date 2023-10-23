#include "lexer.h"

#include "ctexcept.h"

#include <regex>
#include <unordered_map>
#include <string>
#include <vector>

namespace RegexPatterns{
    std::regex elimComment = std::regex(R"/(^([^;]+))/");
    std::regex mainPattern = std::regex(R"/(^[ \t]*(?:(\w+)[ \t]*\:[ \t]*)?(\w{2,6})[ \t]+((?:[ER]?[A-D]X)|(?:[A-D][HL])|(?:[ER]?[SD]I)|(?:R[89][DWB]?)|(?:R1[0-5][DWB]?)|(?:(?=\[(?:(?:[ER]?[A-D]X)|(?:[A-D][HL])|(?:\d+)|(?:\d[\dA-F]*H)|[+\-/*\(\) ]|(?:[ER]?[SD]I)|(?:R[89][DWB]?)|(?:R1[0-5][DWB]?))+\])\[[^\n\r,.]+\]))[ \t]*(?:,[ \t]*((?:\d+)|(?:\d[\dA-F]*H)|(?:[ER]?[A-D]X)|(?:[A-D][HL])|(?:[ER]?[SD]I)|(?:R[89][DWB]?)|(?:R1[0-5][DWB]?)|(?:(?=\[(?:(?:[ER]?[A-D]X)|(?:[A-D][HL])|(?:\d+)|(?:\d[\dA-F]*H)|[+\-/*\(\) ]|(?:[ER]?[SD]I)|(?:R[89][DWB]?)|(?:R1[0-5][DWB]?))+\])\[[^\n\r,.]+\])))?[ \t]*$)/", std::regex_constants::icase);
    std::regex mainPattern1 = std::regex(R"/(^[ \t]*(?:(\w+)[ \t]*\:[ \t]*)?(\w{2,6})[ \t]+((?:\d+)|(?:\d[\dA-F]*H)|(?:[ER]?[A-D]X)|(?:[A-D][HL])|(?:[ER]?[SD]I)|(?:R[89][DWB]?)|(?:R1[0-5][DWB]?)|(?:(?=\[(?:(?:[ER]?[A-D]X)|(?:[A-D][HL])|(?:\d+)|(?:\d[\dA-F]*H)|[+\-/*\(\) ]|(?:[ER]?[SD]I)|(?:R[89][DWB]?)|(?:R1[0-5][DWB]?))+\])\[[^\n\r,.]+\]))[ \t]*$)/", std::regex_constants::icase);
    std::regex mainPattern0 = std::regex(R"/(^[ \t]*(?:(\w+)[ \t]*\:[ \t]*)?(\w{2,6})[ \t]*$)/", std::regex_constants::icase);
    std::regex jumpPattern = std::regex(R"/(^[ \t]*(?:(\w+)[ \t]*\:[ \t]*)?(j\w{1,3}|LOOP[N]?[E]?)[ \t]+(\w+)[ \t]*$)/", std::regex_constants::icase);
    std::regex callPattern = std::regex(R"/(^[ \t]*(?:(\w+)[ \t]*\:[ \t]*)?(call)[ \t]+(\w+)[ \t]*$)/", std::regex_constants::icase);
    std::regex validLabel = std::regex(R"/(^\s*([_a-zA-Z]\w*)\s*$)/");
    std::regex isNumeric = std::regex(R"/(^\s*(\d+)\s*$)/");
    std::regex isHex = std::regex(R"/(^\s*(\d[\dA-F]*)H\s*$)/", std::regex_constants::icase);
    std::regex isSubExpr = std::regex(R"/((?=\[(?:(?:[ER]?[A-D]X)|(?:[A-D][HL])|(?:\d+)|(?:\d[\dA-F]*H)|[+\-/*\(\) ]|(?:[ER]?[SD]I)|(?:R[89][DWB]?)|(?:R1[0-5][DWB]?))+\])\[([^\n\r,.]+)\])/", std::regex_constants::icase);
    std::regex token = std::regex(R"/((?:([ER]?[A-D]X|[A-D][HL]|\d[\dA-F]*H|\d+|[+\-/*\(\)]|[ER]?[SD]I|R[89][DWB]?|R1[0-5][DWB]?)| ))/", std::regex_constants::icase);
    std::regex emptyLine = std::regex(R"/(^\s*$)/");
    std::regex global = std::regex(R"/(^\s*global\s+([_a-zA-Z][_a-zA-Z]*)\s*$)/", std::regex_constants::icase);
    std::regex hlt = std::regex(R"/(^\s*hlt\s*$)/", std::regex_constants::icase);
    std::regex labelOnly = std::regex(R"/(^\s*([_a-zA-Z]\w*)\s*\:\s*$)/", std::regex_constants::icase);
    std::regex patternDB1 = std::regex(R"/(^[ \t]*DB[ \t]+(\w+)[ \t]+(.*?)[ \t]*$)/", std::regex_constants::icase);
    std::regex patternDB2 = std::regex(R"/(^[ \t]*(?:(\w+)[ \t]*\:[ \t]*)?(PUTS)[ \t]+(\w+)[ \t]*$)/", std::regex_constants::icase);
}

Lexer::Lexer(){
    regMap = std::unordered_map<std::string, int>{
        {"TEMP1",   0}, {"TEMP2",    1},
        {"RAX",  1000}, {"EAX",   1100}, {"AX",    1110}, {"AH",    1111}, {"AL", 1112},
        {"RBX",  2000}, {"EBX",   2100}, {"BX",    2110}, {"BH",    2111}, {"BL", 2112},
        {"RCX",  3000}, {"ECX",   3100}, {"CX",    3110}, {"CH",    3111}, {"CL", 3112},
        {"RDX",  4000}, {"EDX",   4100}, {"DX",    4110}, {"DH",    4111}, {"DL", 4112},
        {"RSI",  5000}, {"ESI",   5100}, {"SI",    5110},
        {"RDI",  6000}, {"EDI",   6100}, {"DI",    6110},
        {"RBP",  7000}, {"EBP",   7100}, {"BP",    7110},
        {"RSP",  8000}, {"ESP",   8100}, {"SP",    8110},
        {"RIP",  9000}, {"EIP",   9100}, {"IP",    9110},
        {"R8",  10000}, {"R8D",  10100}, {"R8W",  10110}, {"R8B",  10111},
        {"R9",  11000}, {"R9D",  11100}, {"R9W",  11110}, {"R9B",  11111},
        {"R10", 12000}, {"R10D", 12100}, {"R10W", 12110}, {"R10B", 12111},
        {"R11", 13000}, {"R11D", 13100}, {"R11W", 13110}, {"R11B", 13111},
        {"R12", 14000}, {"R12D", 14100}, {"R12W", 14110}, {"R12B", 14111},
        {"R13", 15000}, {"R13D", 15100}, {"R13W", 15110}, {"R13B", 15111},
        {"R14", 16000}, {"R14D", 16100}, {"R14W", 16110}, {"R14B", 16111},
        {"R15", 17000}, {"R15D", 17100}, {"R15W", 17110}, {"R15B", 17111}
    };
    flagMap = std::unordered_map<std::string, int>{
        {"CF", 0}, {"PF", 1}, {"AF", 2}, 
        {"ZF", 3}, {"SF", 4}, {"TF", 5}, 
        {"IF", 6}, {"DF", 7}, {"OF", 8}
    };

    /*
    1000s digit: number of operands
    1 = 0 operand
    2 = 1 operand
    3 = 2 operands
   */
    mneMap = std::unordered_map<std::string, int>{
        // instructions that take 2 operands
        {"mov",  3000},  {"xchg",  3001},  {"lea",    3002},
        {"add",  3003},  {"sub",   3004},  {"and",    3005},
        {"andn", 3006},  {"or",    3007},  {"xor",    3009},
        {"adc",  3010},  {"sbb",   3011},  {"shl",    3012},
        {"shr",  3013},  {"sal",   3014},  {"sar",    3015},
        {"rol",  3016},  {"ror",   3017},  {"rcl",    3018},
        {"rcr",  3019},  {"cmp",   3020},  {"test",   3021},
        {"adcx", 3022},  {"sbbx",  3023},  {"shlx",   3024},
        {"shrx", 3025},  {"salx",  3026},  {"sarx",   3027},
        {"rorx", 3028}, 
        
        // instructions that take 1 operands
        {"inc",  2000},  {"dec",   2001},  {"mul",    2002},
        {"div",  2003},  {"not",   2004},  {"jmp",    2005},
        {"je",   2006},  {"jne",   2007},  {"jg",     2008},
        {"jge",  2009},  {"jl",    2010},  {"jle",    2011},
        {"jz",   2012},  {"jnz",   2013},  {"jp",     2014},
        {"jnp",  2015},  {"jpe",   2016},  {"jpo",    2017},
        {"jo",   2018},  {"jno",   2019},  {"jc",     2020},
        {"jnc",  2021},  {"js",    2022},  {"jns",    2023},
        {"loop", 2024},  {"loope", 2025},  {"loopne", 2026},
        {"call", 2027},  {"push",  2028},  {"pop",    2029},
        // Ring 4 instructions have 5 as 100s place + 1 operand
        {"puts", 2500},  {"putch", 2501},

        // instructions that take no operands
        {"nop",  1000},  {"hlt",   1001},  {"ret",    1002},
        {"aaa",  1003},  {"aas",   1003},  {"aam",    1004},
        {"aad",  1005},  {"daa",   1006},  {"das",    1007},

        // Ring 4 instructions have 5 as 100s place + 0 operand
        {"flush",1500},  {"putu",  1501},  {"puti",   1502},
        {"putc", 1503},  {"putd",  1504},
    };
}

int Lexer::identifyRegister(const std::string& reg) noexcept{
    std::string s = reg;
    std::transform(s.begin(), s.end(), s.begin(),
        [](unsigned char c){ return std::toupper(c);});
    if(this->regMap.count(s)) return this->regMap.at(s);
    return -1;
}

int Lexer::identifyFlag(const std::string& flag) noexcept{
    std::string s = flag;
    std::transform(s.begin(), s.end(), s.begin(),
        [](unsigned char c){ return std::toupper(c);});
    if(this->flagMap.count(s)) return this->flagMap.at(s);
    return -1;
}

int Lexer::identifyMnemonic(const std::string& mne) noexcept{
    std::string data = mne;
    std::transform(data.begin(), data.end(), data.begin(),
        [](unsigned char c){ return std::tolower(c);});
    if(this->mneMap.count(data)) return this->mneMap.at(data);
    return -1;
}

int Lexer::identify(const std::string& s) noexcept{
    int t = identifyRegister(s);
    if(t == -1) t = identifyFlag(s);
    return t;
}

std::vector<std::string> Lexer::parse(const std::string& s){
    std::smatch match;
    if (!std::regex_search(s,match,RegexPatterns::elimComment)) 
        return {"", "", "", ""};
    std::string ss = match[1];
    if(std::regex_search(ss, match, RegexPatterns::emptyLine)) 
        return {"", "", "", ""};
    if(std::regex_search(ss, match, RegexPatterns::mainPattern))
        return {match[1], match[2], match[3], match[4]};
    if(std::regex_search(ss, match, RegexPatterns::mainPattern1)||
       std::regex_search(ss, match, RegexPatterns::jumpPattern) ||
       std::regex_search(ss, match, RegexPatterns::callPattern) ||
       std::regex_search(ss, match, RegexPatterns::patternDB2)
    )  return {match[1], match[2], match[3], ""};
    if(std::regex_search(ss, match, RegexPatterns::mainPattern0))
        return {match[1], match[2], "", ""};
    if(std::regex_search(ss, match, RegexPatterns::labelOnly))
        return {match[1], "", "", ""};
    throw Exception::BadInstructionException();
}

void Lexer::parse(const std::string& s, 
        std::string& label, std::string& instruction, 
        std::string& operand1, std::string& operand2){
    std::vector<std::string> tokens = parse(s);
    label = tokens[0];
    instruction = tokens[1];
    operand1 = tokens[2];
    operand2 = tokens[3];
}

bool Lexer::isValidLabel(const std::string& s) noexcept{
    return std::regex_search(s, RegexPatterns::validLabel);
}

bool Lexer::isNumeric(const std::string& s) noexcept{
    return std::regex_search(s, RegexPatterns::isNumeric);
}

bool Lexer::isHex(const std::string& s) noexcept{
    return std::regex_search(s, RegexPatterns::isHex);
}

bool Lexer::isSubExpr(const std::string& s) noexcept{
    return std::regex_search(s, RegexPatterns::isSubExpr);
}

bool Lexer::isJump(const std::string& s) noexcept{
    return std::regex_search(s, RegexPatterns::jumpPattern);
}

std::vector<unsigned char> Lexer::isDB(const std::string& s, size_t& t) noexcept{
    std::smatch match; std::string s1;
    if(std::regex_search(s, match, RegexPatterns::patternDB1)){
        s1 = match[1];
        std::transform(s1.begin(), s1.end(), s1.begin(),
            [](unsigned char c){ return std::tolower(c);});
        t = std::hash<std::string>{}(s1);

        std::stringstream ss((std::string)match[2]);
        std::vector<unsigned char> arr;
        while(std::getline(ss, s1, ',')){
            size_t start = s1.find_first_not_of(" \t\n\r");
            if (start != std::string::npos) s1 = s1.substr(start);
            size_t end = s1.find_last_not_of(" \t\n\r");
            if (end != std::string::npos) s1 = s1.substr(0, end + 1);

            if(s1[0]=='"'){
                for(int i=1; i<s1.size()-1; i++)
                    arr.push_back(s1[i]);
            }
            else arr.push_back((unsigned char) (std::stoull(s1)&0xFF));
        }
        return arr;
    }
    t = 0;
    return {};
}

bool Lexer::isEmptyLine(const std::string& s) noexcept {
    std::smatch match, m1; std::string ss;
    std::regex_search(s, match, RegexPatterns::elimComment);
    ss = match[1];
    return std::regex_search(ss, m1, RegexPatterns::emptyLine);
}

std::string Lexer::isGlobal(const std::string& s) noexcept{
    std::smatch match;
    std::regex_search(s, match, RegexPatterns::elimComment);
    std::string ss = match[1];
    std::regex_search(ss, match, RegexPatterns::global);
    return match[1];
}

std::vector<std::string> toPostfix(const std::vector<std::string>& infix){
    std::stack<std::string> stack;
    std::vector<std::string> postfix;
    for(auto&token:infix){
        if(token == "(") stack.push(token);
        else if(token == ")"){
            while(!stack.empty() && stack.top() != "("){
                postfix.push_back(stack.top());
                stack.pop();
            }
            if(stack.empty()) throw Exception::BadSyntaxException("Mismatched parentheses");
            stack.pop();
        }
        else if(token == "+" || token == "-"){
            while(!stack.empty() && stack.top() != "("){
                postfix.push_back(stack.top());
                stack.pop();
            }
            stack.push(token);
        }
        else if(token == "*" || token == "/"){
            while(!stack.empty() && (stack.top() == "*" || stack.top() == "/")){
                postfix.push_back(stack.top());
                stack.pop();
            }
            stack.push(token);
        }
        else postfix.push_back(token);
    }
    while(!stack.empty()){
        if(stack.top() == "(") throw Exception::BadSyntaxException("Mismatched parentheses");
        postfix.push_back(stack.top());
        stack.pop();
    }
    return postfix;
}

std::vector<unsigned long long> Lexer::tokenize(const std::string& s){
    std::vector<unsigned long long> tokens; 
    std::vector<std::string> infix;
    std::string token;
    std::sregex_iterator it(s.begin(), s.end(), RegexPatterns::token);
    std::sregex_iterator end;
    while(it!=end){
        infix.push_back((*it)[1]);
        ++it;
    }
    std::vector<std::string> postfix = toPostfix(infix);
    for(auto&token:postfix) {
        if(isNumeric(token)){
            tokens.push_back(2);
            tokens.push_back(std::stoull(token));
        }
        else if(isHex(token)){
            tokens.push_back(2);
            tokens.push_back(std::stoull(token, nullptr, 16));
        }
        else if(token == "+"){
            tokens.push_back(3);
            tokens.push_back(1);
        }
        else if(token == "-"){
            tokens.push_back(3);
            tokens.push_back(2);
        }
        else if(token == "*"){
            tokens.push_back(3);
            tokens.push_back(3);
        }
        else if(token == "/"){
            tokens.push_back(3);
            tokens.push_back(4);
        }
        else{
            tokens.push_back(1);
            tokens.push_back(identify(token));
        }
    }
    return tokens;
}

unsigned long long Lexer::convert(const std::string& s) noexcept{
    if(isNumeric(s)) return std::stoull(s);
    if(isHex(s)) return std::stoull(s, nullptr, 16);
    if(s.size()==1) return s[0];
    return identify(s);
}
