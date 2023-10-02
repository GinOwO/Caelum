#ifndef CTEXCEPT_H
#define CTEXCEPT_H

#include <exception>
#include <string>

namespace Exception{

class BadRegisterException : public std::exception{
    const char* what() const throw(){
        return "Bad register";
    }
};

class BadFlagException : public std::exception{
    const char* what() const throw(){
        return "Bad flag";
    }
};

class BadValueException : public std::exception{
    const char* what() const throw(){
        return "Bad value";
    }
};

class BadInstructionException : public std::exception{
    const char* what() const throw(){
        return "Bad instruction";
    }
};

class HaltException : public std::exception{
    const char* what() const throw(){
        return "HALTED";
    }
};

class BadLabelException : public std::exception{
    const char* what() const throw(){
        return "Bad label";
    }
};

class BadMemoryException : public std::exception{
    const char* what() const throw(){
        return "Bad memory";
    }
};

class PointerOutOfBoundsException : public std::exception{
    const char* what() const throw(){
        return "Pointer out of bounds";
    }
};

class UnknownException : public std::exception{
    const char* what() const throw(){
        return "Unknown exception";
    }
};

class BadSyntaxException : public std::exception{
    std::string e;
    const char* what() const throw(){
        return e.c_str();
    }
public:
    std::string error(){
        return e;
    }
public:
    BadSyntaxException(std::string e = "Bad syntax"){
        this->e = e;
    }
};

class MissingGlobalException : public std::exception{
    const char* what() const throw(){
        return "Missing global";
    }
};

class LabelRedefinedException : public std::exception{
    const char* what() const throw(){
        return "Label redefined";
    }
};

class StackUnderflowException : public std::exception{
    const char* what() const throw(){
        return "Stack underflow";
    }
};

}
#endif
