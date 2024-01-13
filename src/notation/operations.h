#pragma once

#include <unordered_map>
#include <string>
#include <memory>


const int MAX_OP_N = 4;
enum class OPERATION_TYPE {
    PLUS = 0,
    MINUS,
    MULT,
    DIV
};

extern std::unordered_map<OPERATION_TYPE, std::string> op2str;

enum class NOTATION_TYPE {
    UNKNOWN,
    NUM,
    OPERATION,
    BRACKET
};

class Notation {
public:
    Notation() = default;
    virtual ~Notation() = default;

    NOTATION_TYPE GetType() const {
        return _type;
    }

protected:
    NOTATION_TYPE _type;
};

class UnknownSymbol : public Notation {
public:
    UnknownSymbol();
};


using NotationPtr = std::shared_ptr<Notation>;

class Bracket : public Notation {
public:
    Bracket(bool isRight);
    Bracket(char symbol);

    bool IsRight() const {
        return _isRight;
    }

private:
    bool _isRight;
};

class Number : public Notation {
public:
    Number(char symbol);
    Number(float num);

    float GetNum() const {
        return _num;
    }

private:
    float _num;
};

using NumberPtr = std::shared_ptr<Number>;

class Operation : public Notation {
public:
    Operation(OPERATION_TYPE operationType);
    OPERATION_TYPE GetOperationType() const {
        return _operationType;
    }
    int GetPriority() const {
        return _priority;
    }

    virtual NumberPtr Operate(NumberPtr a, NumberPtr b) = 0;

protected:
    OPERATION_TYPE _operationType;
    int _priority;
};

class PlusOperation : public Operation {
public:
    PlusOperation();
    NumberPtr Operate(NumberPtr a, NumberPtr b) override;
};

class MinusOperation : public Operation {
public:
    MinusOperation();
    NumberPtr Operate(NumberPtr a, NumberPtr b) override;
};

class MultOperation : public Operation {
public:
    MultOperation();
    NumberPtr Operate(NumberPtr a, NumberPtr b) override;
};

class DivOperation : public Operation {
public:
    DivOperation();
    NumberPtr Operate(NumberPtr a, NumberPtr b) override;
};

NotationPtr BuildNotation(char symbol);
