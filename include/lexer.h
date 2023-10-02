#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <regex>
#include <vector>
#include <unordered_map>

class Lexer{
private:
    std::unordered_map<std::string, int> regMap;
    std::unordered_map<std::string, int> flagMap;
    std::unordered_map<std::string, int> mneMap;

    int identifyRegister(const std::string&) noexcept;
    int identifyFlag(const std::string&) noexcept;
public:
    Lexer();
    ~Lexer(){}

    std::vector<std::string> parse(const std::string&);
    void parse(const std::string&, std::string&, 
        std::string&, std::string&, std::string&);

    std::vector<unsigned long long> tokenize(const std::string&);

    unsigned long long convert(const std::string&) noexcept;
    int identifyMnemonic(const std::string&) noexcept;
    int identify(const std::string&) noexcept;

    bool isValidLabel(const std::string&) noexcept;
    bool isNumeric(const std::string&) noexcept;
    bool isHex(const std::string&) noexcept;
    bool isSubExpr(const std::string&) noexcept;
    bool isEmptyLine(const std::string&) noexcept;
    bool isJump(const std::string&) noexcept;

    std::string isGlobal(const std::string&) noexcept;
};


#endif