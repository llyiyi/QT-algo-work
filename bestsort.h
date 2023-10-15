#ifndef BESTSORT_H
#define BESTSORT_H

#include <vector>
#include <type_traits>

//最强排序，当数组为int型采用基数排序，当数组为其他类型时采用introsort
template <typename T>
class Bestsort {
public:
    Bestsort(int maxDepth = 0);
    void sort(std::vector<T>& arr);
private:
    int maxDepth_;
    int depthLimit();
    void introsort(std::vector<T>& arr, int begin, int end, int depth);
    int partition(std::vector<T>& arr, int begin, int end);
    void heapsort(std::vector<T>& arr, int begin, int end);
    void radixSort(std::vector<int>& arr);

};

#endif


