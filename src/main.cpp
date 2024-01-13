#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <stack>
#include <unordered_set>
// #include <set>

#include "notation/operations.h"
#include "permutation/permutation.h"

size_t solutions = 1;

std::string MakeStrOperation(const std::vector<float>& nums, const std::vector<OPERATION_TYPE>& ops) {
    std::string output;
    output += std::to_string(static_cast<int>(nums[0]));
    output += " " + op2str[ops[0]] + " ";
    output += std::to_string(static_cast<int>(nums[1]));
    output += " " + op2str[ops[1]] + " ";
    output += std::to_string(static_cast<int>(nums[2]));
    output += " " + op2str[ops[2]] + " ";
    output += std::to_string(static_cast<int>(nums[3]));
    return output;
}

std::string PolandToStr(const std::vector<NotationPtr>& notations) {
    std::string output;
    for (auto notation : notations) {
        if (notation->GetType() == NOTATION_TYPE::BRACKET) {
            auto bracket = std::dynamic_pointer_cast<Bracket>(notation);
            if (bracket->IsRight()) {
                output += ')';
            } else {
                output += '(';
            }
        } else if (notation->GetType() == NOTATION_TYPE::OPERATION) {
            auto op = std::dynamic_pointer_cast<Operation>(notation);
            switch (op->GetOperationType()) {
                case OPERATION_TYPE::PLUS:
                    output += '+';
                    break;
                case OPERATION_TYPE::MINUS:
                    output += '-';
                    break;
                case OPERATION_TYPE::MULT:
                    output += '*';
                    break;
                case OPERATION_TYPE::DIV:
                    output += '/';
                    break;
            }
        } else if (notation->GetType() == NOTATION_TYPE::NUM) {
            auto num = std::dynamic_pointer_cast<Number>(notation);
            output += std::to_string(static_cast<int>(num->GetNum()));
        }

        output += " ";
    }
    return output;
}

std::vector<NotationPtr> MakePolandNotation(const std::string& str) {
    std::vector<NotationPtr> output;
    std::stack<NotationPtr> stack;

    for (size_t i = 0; i < str.size(); ++i) {
        char symbol = str[i];
        if (symbol == ' ') {
            continue;
        }
        auto notation = BuildNotation(symbol);
        if (notation->GetType() == NOTATION_TYPE::NUM) {
            output.push_back(notation);
        } else if (notation->GetType() == NOTATION_TYPE::OPERATION) {
            if (not stack.empty() && stack.top()->GetType() == NOTATION_TYPE::OPERATION) { // TODO: while??
                auto op = std::dynamic_pointer_cast<Operation>(notation);
                auto otherOp = std::dynamic_pointer_cast<Operation>(stack.top());
                if (op->GetPriority() <= otherOp->GetPriority()) {
                    output.push_back(otherOp);
                    stack.pop();
                }
            }
            stack.push(notation);
        } else if (notation->GetType() == NOTATION_TYPE::BRACKET) {
            auto bracket = std::dynamic_pointer_cast<Bracket>(notation);
            if (bracket->IsRight()) {
                while (stack.top()->GetType() != NOTATION_TYPE::BRACKET) {
                    output.push_back(stack.top());
                    stack.pop();
                }
                stack.pop();
            } else {
                stack.push(notation);
            }
        }
    }
    while (not stack.empty()) {
        auto notation = stack.top();
        if (notation->GetType() == NOTATION_TYPE::OPERATION) {
            output.push_back(notation);
        }
        stack.pop();
    }
    return output;
}

struct PolandEssenseHash;

class PolandEssense {
public:
    using Argumens = std::pair<float, float>;
    bool operator==(const PolandEssense& other) const {
        if (other._actions.size() != _actions.size()) {
            return false;
        }

        for (int i = 0; i < _actions.size(); ++i) {
            auto& pair1 = _actions[i];
            auto& pair2 = other._actions[i];
            if (pair1.first != pair2.first) {
                return false;
            }

            if (pair2.second != pair2.second) {
                return false;
            }
        }
        return true;
    }

    void Add(std::pair<OPERATION_TYPE, Argumens> action) {
        _actions.push_back(action);
    }

friend PolandEssenseHash;

private:
    std::vector<std::pair<OPERATION_TYPE, Argumens>> _actions;
};

struct PolandEssenseHash {
    std::size_t operator()(const PolandEssense& obj) const {
        int sum = 0.;
        int multiplier = 1;
        for (auto action : obj._actions) {
            sum += multiplier * static_cast<int>(action.first);
            sum += multiplier * (action.second.first + action.second.second);
            multiplier *= 10;
        }
        return std::hash<int>{}(sum);
    }
};

PolandEssense ComputePolandEssence(const std::vector<NotationPtr>& notations) {
    std::stack<NotationPtr> stack;
    PolandEssense essense;

    for (auto notation : notations) {
        if (notation->GetType() == NOTATION_TYPE::NUM) {
            stack.push(notation);
        } else if (notation->GetType() == NOTATION_TYPE::OPERATION) {
            auto op = std::dynamic_pointer_cast<Operation>(notation);
            auto b = std::dynamic_pointer_cast<Number>(stack.top());
            stack.pop();
            auto a = std::dynamic_pointer_cast<Number>(stack.top());
            stack.pop();
            stack.push(op->Operate(a, b));
            if (a > b and (op->GetOperationType() == OPERATION_TYPE::MULT or op->GetOperationType() == OPERATION_TYPE::PLUS)) {
                std::swap(a, b);
            }
            essense.Add({op->GetOperationType(), {a->GetNum(), b->GetNum()}});
        }
    }
    if (stack.size() != 1) {
        std::cout << "ERRRRRRRRROR" << std::endl;
    }
    auto number = std::dynamic_pointer_cast<Number>(stack.top());
    return essense;
}

float ComputePoland(const std::vector<NotationPtr>& notations) {
    std::stack<NotationPtr> stack;
    for (auto notation : notations) {
        if (notation->GetType() == NOTATION_TYPE::NUM) {
            stack.push(notation);
        } else if (notation->GetType() == NOTATION_TYPE::OPERATION) {
            auto op = std::dynamic_pointer_cast<Operation>(notation);
            auto b = std::dynamic_pointer_cast<Number>(stack.top());
            stack.pop();
            auto a = std::dynamic_pointer_cast<Number>(stack.top());
            stack.pop();
            stack.push(op->Operate(a, b));
        }
    }
    if (stack.size() != 1) {
        std::cout << "ERRRRRRRRROR" << std::endl;
    }
    auto number = std::dynamic_pointer_cast<Number>(stack.top());
    return number->GetNum();
}


std::vector<std::pair<int, int>> bracketsVariants = {
    {0, 5},
    {4, 9},
    {8, 13},
    {0, 9},
    {4, 13}
};

std::unordered_set<PolandEssense, PolandEssenseHash> answers;

void SearchSolutions(const std::vector<float> nums) {
    int all_op_n = pow(MAX_OP_N, 3);
    int i = 0;

    std::vector<OPERATION_TYPE> ops(3);

    while (i < all_op_n) {
        // std::cout << "i: " << i << std::endl;

        auto str = MakeStrOperation(nums, ops);
        // std::cout << str << std::endl;

        auto polNotation = MakePolandNotation(str);
        // auto polStr = PolandToStr(polNotation);
        // std::cout << polStr << std::endl;
        auto ans = ComputePoland(polNotation);
        if (abs(10. - ans) < 0.000001) {
            auto polandEssense = ComputePolandEssence(polNotation);
            if (answers.find(polandEssense) == answers.end()) {
                std::cout << solutions++ << ":\t" << str << std::endl;
                // std::cout << PolandToStr(polNotation) << std::endl;
                answers.insert(polandEssense);
            }
        }
        // std::cout << "Result :" << ans << std::endl;

        for (auto bracketPair : bracketsVariants) {
            std::string bracketEq = str;
            bracketEq.insert(bracketEq.begin() + bracketPair.second, ')');
            bracketEq.insert(bracketEq.begin() + bracketPair.first, '(');
            auto bracketPN = MakePolandNotation(bracketEq);
            auto bracketPolStr = PolandToStr(bracketPN);
            // if (bracketEq == "(1 - 8) * 1 / 9") {
            //     std::cout << "THERE" << std::endl;
            // }

            auto brAns = ComputePoland(bracketPN);
            if (abs(ans - brAns) < 0.000001) {
                continue;
            }
            if (abs(10. - brAns) < 0.000001) {
                auto polandEssense = ComputePolandEssence(bracketPN);
                if (answers.find(polandEssense) == answers.end()) {
                    std::cout << solutions++ << ":\t" << bracketEq << std::endl;
                    // std::cout << bracketPolStr << std::endl;
                    answers.insert(polandEssense);
                }
            }
        }

        // next permutation
        ++i;
        if (i >= all_op_n) {
            return;
        }
        int incr = 1, incr_index = 2;
        while (incr > 0) {
            int op_num = static_cast<int>(ops[incr_index]);
            if (op_num < MAX_OP_N - 1) {
                ops[incr_index] = static_cast<OPERATION_TYPE>(op_num + 1);
                incr = 0;
            } else {
                ops[incr_index] = static_cast<OPERATION_TYPE>(0);
                --incr_index;
            }
        }
    }
}

int main() {
    std::cout << "4=10 Solver" << std::endl;

    Permutation<4> perm;

    // std::vector<float> nums = {7, 7, 9, 4};
    std::vector<float> nums;
    int buffer;
    for (int i = 0; i < 4; ++i) {
        std::cin >> buffer;
        nums.push_back(buffer);
    }

    for (int i = 0; i < 24; ++i) {
        std::vector<float> permNums;
        for (auto ind : perm.GetNums()) {
            permNums.push_back(nums[ind]);
        }
        SearchSolutions(permNums);
        perm.Next();
    }

    return 0;
}
