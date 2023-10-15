#include "sortalgotime.h"

sortalgotime::sortalgotime()
{
    cmpCnt = 0;
    oprtCnt = 0;
}

sortalgotime::~sortalgotime()
{
}

// 是否正在排序
bool sortalgotime::isSorting()
{
    if (sortingDone)
        return false;
    return true;
}

// 设置排序状态
void sortalgotime::setSortingState(bool state)
{
    sortingDone = !state;
}


// 设置排序属性
void sortalgotime::setAttribute(int dataNum, int *numbers, int algorithmSelected)
{
    this->dataNum = dataNum;
    this->numbers = numbers;
    this->algorithmSelected = algorithmSelected;
}

// 获取比较次数
int sortalgotime::getCmpCnt()
{
    return cmpCnt;
}

// 获取交换次数
int sortalgotime::getOprtCnt()
{
    return oprtCnt;
}


void sortalgotime::run()
{
    while (!sortingDone)
    {
        process();
    }
    quit();
    wait();
}

void sortalgotime::process()
{
    // 重置比较和交换次数
    cmpCnt = 0;
    oprtCnt = 0;

    // 调用快速排序的函数
    switch (algorithmSelected)
    {
    case 0: // 基数排序
        while (!sortingDone)
        {
            radixSort();
            sortingDone = 1;
        }
        break;

    case 1: // 快速排序
        while (!sortingDone)
        {
            quickSort(0, dataNum - 1);
            sortingDone = 1;
        }
        break;

    case 2: // 归并排序
        while (!sortingDone)
        {
            mergeSort(0, dataNum - 1);
            sortingDone = 1;
        }
        break;

    case 3: // 堆排序
        while (!sortingDone)
        {
            heapSort();
            sortingDone = 1;
        }
        break;

    case 4: // 希尔排序
        while (!sortingDone)
        {
            shellSort();
            sortingDone = 1;
        }
        break;

    case 5: // 插入排序
        while (!sortingDone)
        {
            insertSort();
            sortingDone = 1;
        }
        break;

    case 6: // 冒泡排序
        while (!sortingDone)
        {
            bubbleSort();
            sortingDone = 1;
        }
        break;

    case 7: // 选择排序
        while (!sortingDone)
        {
            selectSort();
            sortingDone = 1;
        }
        break;
    }

    // 排序结束，发送结束的信号
    emit showSBMsg(1, QString("排序已完成"));
    emit showSBMsg(2, QString("")); // 清空操作和描述
    emit showSBMsg(3, QString(""));
    emit showSBMsg(4, QString(""));
    emit sortFinish();
}

// 交换a b两个位置的数
void sortalgotime::swap(int a, int b)
{
    oprtCnt++;
    int temp = numbers[a];
    numbers[a] = numbers[b];
    numbers[b] = temp;
}

// 0 基数排序
void sortalgotime::radixSort()
{
    int maximum = numbers[0];
    int max_flag = 0;

    for (int i = 1; i < dataNum; i++)
    {
        cmpCnt++;
        maximum = std::max(maximum, numbers[i]);
        if (maximum == numbers[i])
            max_flag = i;
    }


    int digits = 0;

    while (maximum > 0)
    {
        digits++;
        maximum /= 10;
    }

    for (int i = 0; i < digits; i++)
    {
        int power = pow(10, i);

        int *temp = new int[dataNum];
        int count[10];
        memset(count, 0, sizeof(count));


        for (int j = 0; j < dataNum; j++)
        {
            int num = (numbers[j] / power) % 10;
            count[num]++;

            cmpCnt++;
        }


        for (int j = 1; j < 10; j++)
        {
            count[j] += count[j - 1];
        }


        for (int j = dataNum - 1; j >= 0; j--)
        {

            int num = (numbers[j] / power) % 10;
            temp[count[num] - 1] = numbers[j];
            count[num]--;

            cmpCnt++;

        }


        for (int j = 0; j < dataNum; j++)
        {
            numbers[j] = temp[j];

            oprtCnt++;
        }
    }
}

// 1 快速排序
void sortalgotime::quickSort(int left, int right)
{
    if (left >= right)
        return;

    int i, j, base;
    i = left, j = right;
    base = numbers[left];

    while (i < j)
    {



        while (numbers[j] >= base && i < j)
        {
            j--;
            cmpCnt++;
        }
        while (numbers[i] <= base && i < j)
        {
            i++;
            cmpCnt++;
        }

        if (i < j)
        {
            swap(i, j);

        }
    }

    numbers[left] = numbers[i];
    numbers[i] = base;
    oprtCnt++;

    quickSort(left, i - 1);
    quickSort(i + 1, right);
}

// 2 归并排序
void sortalgotime::mergeSort(int left, int right)
{
    if (left < right)
    {
        int mid = (left + right) / 2;
        mergeSort(left, mid);
        mergeSort(mid + 1, right);
        merge(left, mid, right);
    }
}

// 归并排序_合并函数
void sortalgotime::merge(int left, int mid, int right)
{

    int *temp = new int[right - left + 1];
    int i = left, j = mid + 1, k = 0;
    while (i <= mid && j <= right)
    {
        cmpCnt++;

        if (numbers[i] <= numbers[j])
        {
            temp[k++] = numbers[i++];
            oprtCnt++;
        }
        else
        {
            temp[k++] = numbers[j++];
            oprtCnt++;
        }
    }
    while (i <= mid)
    {
        temp[k++] = numbers[i++];
    }
    while (j <= right)
    {
        temp[k++] = numbers[j++];
    }

    k = 0;
    for (int i = left; i <= right; i++)
    {
        numbers[i] = temp[k++];
        oprtCnt++;
    }

    delete[] temp;
}

// 3 堆排序
void sortalgotime::heapSort()
{

    for (int i = dataNum / 2 - 1; i >= 0; i--)
    {
        max_heapify(i, dataNum - 1);
    }


    for (int i = dataNum - 1; i > 0; i--)
    {
        swap(0, i);

        oprtCnt++;
        max_heapify(0, i - 1);
    }
}

// 堆排序_构建大根堆
void sortalgotime::max_heapify(int start, int end)
{
    int fa = start;
    int son = fa * 2 + 1;

    while (son <= end)
    {
        if (son + 1 <= end && numbers[son] < numbers[son + 1])
        {
            cmpCnt++;

            son++;
        }

        cmpCnt++;

        if (numbers[fa] > numbers[son])
            return;
        else
        {

            swap(fa, son);

            fa = son;
            son = son * 2 + 1;

            oprtCnt++;
        }
    }
}

// 4 希尔排序
void sortalgotime::shellSort()
{

    int i, j;
    for (int gap = dataNum >> 1; gap >= 1; gap = gap >> 1)
    {
        for (i = gap; i < dataNum; i++)
        {
            cmpCnt++;

            if (numbers[i] < numbers[i - gap])
            {

                int temp = numbers[i];
                for (j = i - gap; j >= 0 && numbers[j] > temp; j = j - gap)
                {
                    if (j != i - gap)
                    {
                        cmpCnt++;
                    }
                    numbers[j + gap] = numbers[j];

                    oprtCnt++;
                }
                numbers[j + gap] = temp;

                oprtCnt++;
            }
        }
    }
}

// 5 插入排序
void sortalgotime::insertSort()
{
    for (int i = 1; i < dataNum; i++)
    {
        int key = numbers[i];
        int j = i - 1;


        while (j >= 0 && numbers[j] > key)
        {
            cmpCnt++;


            numbers[j + 1] = numbers[j];

            oprtCnt++;

            j--;
        }
        numbers[j + 1] = key;

        oprtCnt++;
    }
}

// 6 冒泡排序
void sortalgotime::bubbleSort()
{
    for (int i = 0; i < dataNum; i++)
    {
        for (int j = 0; j < dataNum - 1 - i; j++)
        {
            cmpCnt++;

            if (numbers[j] > numbers[j + 1])
            {
                swap(j, j + 1);

            }
        }
    }
}

// 7 选择排序
void sortalgotime::selectSort()
{
    for (int i = 0; i < dataNum - 1; i++)
    {
        int min = numbers[i], min_flag = i;


        for (int j = i + 1; j < dataNum; j++)
        {


            cmpCnt++;


            if (numbers[j] < min)
            {
                min = numbers[j];
                min_flag = j;

            }
        }

        swap(i, min_flag);

        oprtCnt++;
    }
}
