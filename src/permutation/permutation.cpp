#include "permutation.h"

template<std::size_t N>
Permutation<N>::Permutation() {
    for (std::size_t i = 0; i < N; ++i) {
        _nums[i] = i;
    }
}

template<std::size_t N>
void Permutation<N>::Next() {
    std::size_t biggestInd = 0;
    for (std::size_t i = 0; i < N - 1; ++i) {
        if (_nums[i] < _nums[i + 1]) {
            biggestInd = i;
        }
    }

    std::size_t l = biggestInd + 1;
    for (std::size_t j = biggestInd + 1; j < N; ++j) {
        if (_nums[biggestInd] < _nums[j]) {
            l = j;
        }
    }

    std::swap(_nums[biggestInd], _nums[l]);

    std::size_t k = biggestInd + 1;
    std::size_t length = N - k;
    for (std::size_t i = k; i < k + length / 2; ++i) {
        std::swap(_nums[i], _nums[N + k - i - 1]);
    }
}

template class Permutation<4>;
