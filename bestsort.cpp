#include "bestsort.h"

bestsort::bestsort()
{
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

}

