#include "permutation.h"

template<size_t N>
Permutation<N>::Permutation() {
    for (size_t i = 0; i < N; ++i) {
        _nums[i] = i;
    }
}

template<size_t N>
void Permutation<N>::Next() {
    size_t biggestInd = 0;
    for (size_t i = 0; i < N - 1; ++i) {
        if (_nums[i] < _nums[i + 1]) {
            biggestInd = i;
        }
    }

    size_t l = biggestInd + 1;
    for (size_t j = biggestInd + 1; j < N; ++j) {
        if (_nums[biggestInd] < _nums[j]) {
            l = j;
        }
    }

    std::swap(_nums[biggestInd], _nums[l]);

    size_t k = biggestInd + 1;
    size_t length = N - k;
    for (size_t i = k; i < k + length / 2; ++i) {
        std::swap(_nums[i], _nums[N + k - i - 1]);
    }
}

template class Permutation<4>;
