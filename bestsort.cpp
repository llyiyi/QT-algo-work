#pragma GCC optimize(3,"Ofast","inline")

#include "bestsort.h"
#include <algorithm>
#include <cmath>

template <typename T>
Bestsort<T>::Bestsort(int maxDepth) : maxDepth_(maxDepth) {}


template <typename T>
void Bestsort<T>::sort(std::vector<T>& arr) {
    if (std::is_integral<T>::value) {
        std::vector<int> intArr(arr.begin(), arr.end());
        radixSort(intArr);
        for (size_t i = 0; i < arr.size(); i++) {
            arr[i] = intArr[i];
        }
    } else {
        introsort(arr, 0, arr.size() - 1, depthLimit());
    }
}


template <typename T>
int Bestsort<T>::depthLimit() {
    if (maxDepth_ == 0) {
        return 2 * log2(100);
    }
    return maxDepth_;
}

template <typename T>
void Bestsort<T>::introsort(std::vector<T>& arr, int begin, int end, int depth) {
    if (begin < end) {
        if (depth == 0) {
            heapsort(arr, begin, end);
        } else {
            int partitionIndex = partition(arr, begin, end);
            introsort(arr, begin, partitionIndex, depth - 1);
            introsort(arr, partitionIndex + 1, end, depth - 1);
        }
    }
}

template <typename T>
int Bestsort<T>::partition(std::vector<T>& arr, int begin, int end) {
    T pivot = arr[begin];
    int left = begin - 1;
    int right = end + 1;

    while (true) {
        do {
            left++;
        } while (arr[left] < pivot);

        do {
            right--;
        } while (arr[right] > pivot);

        if (left >= right) {
            return right;
        }

        std::swap(arr[left], arr[right]);
    }
}

template <typename T>
void Bestsort<T>::heapsort(std::vector<T>& arr, int begin, int end) {
    std::make_heap(arr.begin() + begin, arr.begin() + end + 1);
    std::sort_heap(arr.begin() + begin, arr.begin() + end + 1);
}

template <typename T>
void Bestsort<T>::radixSort(std::vector<int>& arr) {
    int maximum = arr[0];

    for (size_t i = 1; i < arr.size(); i++) {
        if (arr[i] > maximum) {
            maximum = arr[i];
        }
    }

    int digits = 0;
    int divisor = 1;

    while (maximum > 0) {
        digits++;
        maximum /= 10;
    }

    std::vector<int> temp(arr.size());

    for (int i = 0; i < digits; i++) {
        int count[10] = {0};

        for (size_t j = 0; j < arr.size(); j++) {
            int digit = (arr[j] / divisor) % 10;
            count[digit]++;
        }

        for (int j = 1; j < 10; j++) {
            count[j] += count[j - 1];
        }

        for (int j = arr.size() - 1; j >= 0; j--) {
            int digit = (arr[j] / divisor) % 10;
            temp[count[digit] - 1] = arr[j];
            count[digit]--;
        }

        for (size_t j = 0; j < arr.size(); j++) {
            arr[j] = temp[j];
        }
        divisor *= 10;
    }
}


// 显式实例化 Bestsort 类模板以支持 int、float 和 double
template class Bestsort<int>;
template class Bestsort<float>;
template class Bestsort<double>;
