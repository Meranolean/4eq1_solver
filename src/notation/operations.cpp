#include "operations.h"
#include <memory>

std::unordered_map<OPERATION_TYPE, std::string> op2str = {
    {OPERATION_TYPE::PLUS, "+"},
    {OPERATION_TYPE::MINUS, "-"},
    {OPERATION_TYPE::MULT, "*"},
    {OPERATION_TYPE::DIV, "/"}
};

UnknownSymbol::UnknownSymbol()
    : Notation() {
    _type = NOTATION_TYPE::UNKNOWN;
}

Bracket::Bracket(bool isRight)
    : Notation()
    , _isRight(isRight) {
    _type = NOTATION_TYPE::BRACKET;
}

Bracket::Bracket(char symbol)
    : Notation()
    , _isRight(symbol == ')') {
    _type = NOTATION_TYPE::BRACKET;
}

Number::Number(char symbol)
    : Notation() {
    _type = NOTATION_TYPE::NUM;
    _num = symbol - '0';
}

Number::Number(float num)
    : Notation() {
    _type = NOTATION_TYPE::NUM;
    _num = num;
}

Operation::Operation(OPERATION_TYPE operationType)
    : Notation() {
    _type = NOTATION_TYPE::OPERATION;
    _operationType = operationType;
}

PlusOperation::PlusOperation()
    : Operation(OPERATION_TYPE::PLUS) {
    _priority = 5;
}

NumberPtr PlusOperation::Operate(NumberPtr a, NumberPtr b) {
    return std::make_shared<Number>(a->GetNum() + b->GetNum());
}

MinusOperation::MinusOperation()
    : Operation(OPERATION_TYPE::MINUS) {
    _priority = 5;
}

NumberPtr MinusOperation::Operate(NumberPtr a, NumberPtr b) {
    return std::make_shared<Number>(a->GetNum() - b->GetNum());
}

MultOperation::MultOperation()
    : Operation(OPERATION_TYPE::MULT) {
    _priority = 10;
}

NumberPtr MultOperation::Operate(NumberPtr a, NumberPtr b) {
    return std::make_shared<Number>(a->GetNum() * b->GetNum());
}

DivOperation::DivOperation()
    : Operation(OPERATION_TYPE::DIV) {
    _priority = 10;
}

NumberPtr DivOperation::Operate(NumberPtr a, NumberPtr b) {
    return std::make_shared<Number>(a->GetNum() / b->GetNum());
}


NotationPtr BuildNotation(char symbol) {
    if (symbol == '(' or symbol == ')') {
        return std::make_shared<Bracket>(symbol);
    } else if (
        symbol == '+' ||
        symbol == '-' ||
        symbol == '*' ||
        symbol == '/') {
        switch (symbol) {
            case '+':
                return std::make_shared<PlusOperation>();
            case '-':
                return std::make_shared<MinusOperation>();
            case '*':
                return std::make_shared<MultOperation>();
            case '/':
                return std::make_shared<DivOperation>();
        }
    } else if ((symbol >= '0') and (symbol <= '9')) {
        return std::make_shared<Number>(symbol);
    }
    return std::make_shared<UnknownSymbol>();
}
