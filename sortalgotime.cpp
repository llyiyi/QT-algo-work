#include "sortalgotime.h"

sortalgotime::sortalgotime()
{
    cmpCnt = 0; // 初始化比较次数
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
    // 找出最大元素
    int maximum = numbers[0];
    int max_flag = 0;

    for (int i = 1; i < dataNum; i++)
    {
        cmpCnt++;
        maximum = std::max(maximum, numbers[i]);
        if (maximum == numbers[i])
            max_flag = i;
    }

    // 计算最大数的位数
    int digits = 0;

    while (maximum > 0)
    {
        digits++;
        maximum /= 10;
    }

    // 根据最低有效数字排列
    for (int i = 0; i < digits; i++)
    {
        int power = pow(10, i); // 用于下面取出每一位
        //int temp[dataNum];      // 辅助数组
        int *temp = new int[dataNum];
        int count[10];          // 计数数组，用于排列0-9
        memset(count, 0, sizeof(count));

        // 按位大小统计个数
        for (int j = 0; j < dataNum; j++)
        {
            int num = (numbers[j] / power) % 10;
            count[num]++;

            cmpCnt++;                 // 算访问次数，虽然没有比较，但读取了这个数
        }

        // 累计计数个数
        for (int j = 1; j < 10; j++)
        {
            count[j] += count[j - 1];
        }

        // 按位大小有序放到辅助数组中
        for (int j = dataNum - 1; j >= 0; j--)
        {

            int num = (numbers[j] / power) % 10;
            temp[count[num] - 1] = numbers[j];
            count[num]--;

            cmpCnt++; // 算访问次数，虽然没有比较，但读取了这个数

        }

        // 用位有序的辅助数组更新原数组
        for (int j = 0; j < dataNum; j++)
        {
            numbers[j] = temp[j];

            oprtCnt++;                         // 算访问次数，虽然没有比较，但读取了这个数
        }
    }
}

// 1 快速排序
void sortalgotime::quickSort(int left, int right)
{
    if (left >= right)
        return;

    int i, j, base; // 左右指针，基准
    i = left, j = right;
    base = numbers[left]; // 第一个值设为基准

    while (i < j)
    {



        while (numbers[j] >= base && i < j) // 先j后i，保证一轮之后基准值左侧都是小于基准值的数，先i后j不能保证
        {
            j--;
            cmpCnt++; // 只统计元素数值比较次数，不统计指针i j自身的大小比较次数
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
        mergeSort(left, mid);      // 对 a[left,mid]进行排序
        mergeSort(mid + 1, right); // 对 a[mid+1,right]进行排序
        merge(left, mid, right);   // 进行合并操作
    }
}

// 归并排序_合并函数
void sortalgotime::merge(int left, int mid, int right)
{

    int *temp = new int[right - left + 1]; // 用 new 申请一个辅助函数
    int i = left, j = mid + 1, k = 0;      // k为 temp 数组的下标
    while (i <= mid && j <= right)
    {
        cmpCnt++;

        if (numbers[i] <= numbers[j])
        {
            temp[k++] = numbers[i++]; // 按从小到大存放在 temp 数组里面
            oprtCnt++;
        }
        else
        {
            temp[k++] = numbers[j++];
            oprtCnt++;
        }
    }
    while (i <= mid) // j 序列结束，将剩余的 i 序列补充在 temp 数组中
    {
        temp[k++] = numbers[i++];
    }
    while (j <= right) // i 序列结束，将剩余的 j 序列补充在 temp 数组中
    {
        temp[k++] = numbers[j++];
    }

    k = 0;                              // 从下标为 0 开始传送
    for (int i = left; i <= right; i++) // 将 temp 数组 的值传递给数组 a
    {
        numbers[i] = temp[k++];
        oprtCnt++;
    }

    delete[] temp; // 释放辅助数组
}

// 3 堆排序
void sortalgotime::heapSort()
{
    // 初始化，i从最后一个父节点开始调整，从下到上 0 ~ dataNum-1
    for (int i = dataNum / 2 - 1; i >= 0; i--)
    {
        max_heapify(i, dataNum - 1);
    }

    // 此时已符合大根堆结构
    // 使顶堆跟最后一个元素交换，i--达到弹出的效果，即i+1到len-1的范围都已经排好序，再重新调整使符合大根堆，直到排序完毕
    for (int i = dataNum - 1; i > 0; i--)
    {
        swap(0, i);

        oprtCnt++;
        max_heapify(0, i - 1);
    }
}

// 堆排序_构建大根堆
void sortalgotime::max_heapify(int start, int end) // 此函数是判断大小并执行交换
{
    int fa = start;       // 获取父节点的下标
    int son = fa * 2 + 1; // 左节点，因为下标0开始所以需要+1

    while (son <= end) // 判断若无子节点直接退出
    {
        if (son + 1 <= end && numbers[son] < numbers[son + 1]) // 先比较两个子节点大小，选择最大的
        {
            cmpCnt++;

            son++; // 指向右节点
        }

        cmpCnt++;

        if (numbers[fa] > numbers[son]) // 如果父节点大于子节点代表调整完毕，直接跳出函数
            return;
        else // 否则交换父子节点再继续比较
        {

            swap(fa, son);

            fa = son;          // 父节点指向子节点
            son = son * 2 + 1; // 子节点指向孙节点 ，即这个点还需要跟孙节点比较

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

            if (numbers[i] < numbers[i - gap]) // i小于同组前面的数，把i用冒泡交换至同组前面合适的位置
            {

                int temp = numbers[i];
                for (j = i - gap; j >= 0 && numbers[j] > temp; j = j - gap)
                {
                    if (j != i - gap) // 第一次进入循环不暂停和计数，否则与上面的暂停计数重复
                    {
                        cmpCnt++;
                    }
                    numbers[j + gap] = numbers[j]; // 冒泡逐个更新值

                    oprtCnt++;
                }
                numbers[j + gap] = temp; // 原来的i移动到合适的位置

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
    for (int i = 0; i < dataNum - 1; i++) // 控制排序的轮数，每一轮新增一个排好序的元素
    {
        int min = numbers[i], min_flag = i;


        for (int j = i + 1; j < dataNum; j++) // 遍历未排好序的序列，找出最小值
        {


            cmpCnt++;                        // 下面j与最小值比较


            if (numbers[j] < min) // 记录最小值以及下标
            {
                min = numbers[j];
                min_flag = j;

            }
        }

        swap(i, min_flag); // 交换i和最小值

        oprtCnt++;
    }
}
