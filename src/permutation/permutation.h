#pragma once

#include <array>

template<std::size_t N>
class Permutation {
public:
    Permutation();
    const std::array<int, N>& GetNums() const {
        return _nums;
    }
    void Next();
private:
    std::array<int, N> _nums;
};
