#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <time.h>

class Introsort {
public:
    Introsort(int maxDepth = 0) : maxDepth_(maxDepth) {}

    void sort(std::vector<int>& arr) {
        introsort(arr, 0, arr.size() - 1, depthLimit());
    }

private:
    int maxDepth_;

    int depthLimit() {
        if (maxDepth_ == 0) {
            return 2 * log2(100);
        }
        return maxDepth_;
    }

    void introsort(std::vector<int>& arr, int begin, int end, int depth) {
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

    int partition(std::vector<int>& arr, int begin, int end) {
        int pivot = arr[begin];
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

    void heapsort(std::vector<int>& arr, int begin, int end) {
        std::make_heap(arr.begin() + begin, arr.begin() + end + 1);
        std::sort_heap(arr.begin() + begin, arr.begin() + end + 1);
    }
};

int main() {
    // 生成随机整数数组
    const int arrSize = 10000;
    std::vector<int> arr(arrSize);

    srand(time(0));  // 初始化随机数种子

    for (int i = 0; i < arrSize; i++) {
        arr[i] = std::rand() % 10000 + 1; // 修改范围以适应你的需求
    }

    // 使用 Introsort 对数组进行排序
    Introsort introsort;
    introsort.sort(arr);

    // 打印排序后的所有数据
    std::cout << "Sorted array: ";
    for (int i = 0; i < arrSize; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}
