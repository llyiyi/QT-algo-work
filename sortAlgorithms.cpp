#include "sortAlgorithms.h"

sortAlgorithms::sortAlgorithms()
{
    stop = 1;   
    cmpCnt = 0; 
    oprtCnt = 0;
    oneStep = 0; 
    pause = 0;
}

sortAlgorithms::~sortAlgorithms()
{
}

// 是否正在排序
bool sortAlgorithms::isSorting()
{
    if (stop || sortingDone)
        return false;
    return true;
}

// 是否处于单步执行状态
bool sortAlgorithms::isOneStep()
{
    return oneStep;
}

// 是否处于暂停状态
bool sortAlgorithms::isPause()
{
    return pause;
}

// 设置排序状态
void sortAlgorithms::setSortingState(bool state)
{
    sortingDone = !state;
}

// 设置是否单步排序
void sortAlgorithms::setOneStepState(bool state)
{
    oneStep = state;
}

// 设置排序演示速度
void sortAlgorithms::setSpeed(int speed)
{
    this->speed = speed;
}

// 设置排序属性
void sortAlgorithms::setAttribute(int dataNum, int *numbers, int algorithmSelected, int speed)
{
    this->dataNum = dataNum;
    this->numbers = numbers;
    this->algorithmSelected = algorithmSelected;
    this->speed = speed;
    //    this->stop = false;     // 排序即将开始，若当前处于停止状态需要置成false
}

// 获取比较次数
int sortAlgorithms::getCmpCnt()
{
    return cmpCnt;
}

// 获取交换次数
int sortAlgorithms::getOprtCnt()
{
    return oprtCnt;
}

// 停止排序线程
void sortAlgorithms::stopSorting()
{
    stop = true;
}

// 暂停排序
void sortAlgorithms::pauseSorting()
{
    // qDebug()<<"暂停排序";
    if (!pause)
    {
        this->pauseLock.lock();
        pause = true;
    }
}

// 继续排序
void sortAlgorithms::resumeSorting()
{
    // qDebug()<<"继续排序";
    if (pause)
    {
        this->pauseLock.unlock();
        pause = false;
    }
}

void sortAlgorithms::run()
{
    stop = false;
    while (!stop && !sortingDone)
    {
        process();
    }
    quit();
    wait();
}

void sortAlgorithms::process()
{
    // 重置比较和交换次数
    cmpCnt = 0;
    oprtCnt = 0;

    // 调用快速排序的函数
    switch (algorithmSelected)
    {
    case 0: // 基数排序
        while (!stop && !sortingDone)
        {
            radixSort();
            if (stop)
                return;
            sortingDone = 1;
        }
        break;

    case 1: // 快速排序
        while (!stop && !sortingDone)
        {
            quickSort(0, dataNum - 1);
            if (stop)
                return;
            sortingDone = 1;
        }
        break;

    case 2: // 归并排序
        while (!stop && !sortingDone)
        {
            mergeSort(0, dataNum - 1);
            if (stop)
                return;
            sortingDone = 1;
        }
        break;

    case 3: // 堆排序
        while (!stop && !sortingDone)
        {
            heapSort();
            if (stop)
                return;
            sortingDone = 1;
        }
        break;

    case 4: // 希尔排序
        while (!stop && !sortingDone)
        {
            shellSort();
            if (stop)
                return;
            sortingDone = 1;
        }
        break;

    case 5: // 插入排序
        while (!stop && !sortingDone)
        {
            insertSort();
            if (stop)
                return;
            sortingDone = 1;
        }
        break;

    case 6: // 冒泡排序
        while (!stop && !sortingDone)
        {
            bubbleSort();
            if (stop)
                return;
            sortingDone = 1;
        }
        break;

    case 7: // 选择排序
        while (!stop && !sortingDone)
        {
            selectSort();
            if (stop)
                return;
            sortingDone = 1;
        }
        break;
    }

    // 排序结束，发送结束的信号
    emit showSBMsg(1, QString("排序已完成"));
    emit showSBMsg(2, QString("")); 
    emit showSBMsg(3, QString(""));
    emit showSBMsg(4, QString(""));
    emit sortFinish();
    stop = true;
}

// 交换a b两个位置的数
void sortAlgorithms::swap(int a, int b)
{
    oprtCnt++;
    int temp = numbers[a];
    numbers[a] = numbers[b];
    numbers[b] = temp;
}

// 0 基数排序
void sortAlgorithms::radixSort()
{
    // 找出最大元素
    int maximum = numbers[0];
    int max_flag = 0;

    for (int i = 1; i < dataNum; i++)
    {

        if (stop)
            return;

        pauseLock.lock();
        pauseLock.unlock();
        emit compareColumn(i, max_flag);
        emit baseColumn(max_flag);
        msleep(speed);

        cmpCnt++;
        maximum = std::max(maximum, numbers[i]);
        if (maximum == numbers[i])
            max_flag = i;
    }

    // 计算最大数的位数
    int digits = 0;

    while (maximum > 0)
    {
        if (stop)
            return;

        digits++;
        maximum /= 10;
    }

    // 根据最低有效数字排列
    for (int i = 0; i < digits; i++)
    {
        if (stop)
            return;

        int power = pow(10, i); 

        int *temp = new int[dataNum]; 
        int count[10];               
        memset(count, 0, sizeof(count));

        // 按位大小统计个数
        for (int j = 0; j < dataNum; j++)
        {
            if (stop)
                return;

            int num = (numbers[j] / power) % 10;
            count[num]++;

            pauseLock.lock();
            pauseLock.unlock();
            cmpCnt++;                
            emit compareColumn(j, j); 
            msleep(speed);
        }

        // 累计计数个数
        for (int j = 1; j < 10; j++)
        {
            if (stop)
                return;

            count[j] += count[j - 1];
        }

        // 按位大小有序放到辅助数组中
        for (int j = dataNum - 1; j >= 0; j--)
        {
            if (stop)
                return;

            int num = (numbers[j] / power) % 10;
            temp[count[num] - 1] = numbers[j];
            count[num]--;

            cmpCnt++; 
        }

        // 用位有序的辅助数组更新原数组
        for (int j = 0; j < dataNum; j++)
        {
            if (stop)
                return;

            numbers[j] = temp[j];

            pauseLock.lock();
            pauseLock.unlock();
            oprtCnt++;                         
            emit updtColumn(j, true, numbers); 
            msleep(speed);
        }
    }
}

// 1 快速排序
void sortAlgorithms::quickSort(int left, int right)
{
    if (stop)
        return; 
    if (left >= right)
        return;

    int i, j, base; 
    i = left, j = right;
    base = numbers[left]; 
    if (oneStep)
    {
        emit baseColumn(left);
        msleep(speed);
    }

    while (i < j)
    {
        if (stop)
            return;
        pauseLock.lock(); 
        pauseLock.unlock();

        if (oneStep)
        {
            emit baseColumn(left);
            msleep(speed);
            resumeSorting();
        } 

        while (numbers[j] >= base && i < j) 
        {
            j--;
            cmpCnt++; 
            pauseLock.lock();
            pauseLock.unlock();
            if (oneStep)
            {
                emit baseColumn(left);
                emit compareColumn(i, j);
                if (i != left)
                {
                    emit baseColumn(left);
                    resumeSorting();
                }
                msleep(speed);
            }
        }
        while (numbers[i] <= base && i < j)
        {
            i++;
            cmpCnt++;
            pauseLock.lock();
            pauseLock.unlock();
            if (oneStep)
            {
                emit baseColumn(left);
                emit compareColumn(i, j);
                if (i != left)
                {
                    emit baseColumn(left);
                    resumeSorting();
                }
                msleep(speed);
            }
        }

        if (i < j)
        {
            swap(i, j);

            pauseLock.lock();
            pauseLock.unlock();
            
            emit swapColumn(i, j, numbers);
            emit baseColumn(left);
            resumeSorting();
            msleep(speed);
        }
    }

    numbers[left] = numbers[i];
    numbers[i] = base;
    oprtCnt++;
    emit swapColumn(left, i, numbers); 
    msleep(speed);

    quickSort(left, i - 1);
    quickSort(i + 1, right);
}

// 2 归并排序
void sortAlgorithms::mergeSort(int left, int right)
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
void sortAlgorithms::merge(int left, int mid, int right)
{

    int *temp = new int[right - left + 1]; 
    int i = left, j = mid + 1, k = 0;      
    while (i <= mid && j <= right)
    {
        if (stop)
            return;

        pauseLock.lock();
        pauseLock.unlock();
        cmpCnt++;
        emit compareColumn(i, j);
        if (oneStep)
            emit baseColumn(mid);
        msleep(speed);

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
        if (stop)
            return;

        pauseLock.lock();
        pauseLock.unlock();
        if (oneStep)
            emit baseColumn(mid);
        emit compareColumn(i, j); 

        msleep(speed);

        temp[k++] = numbers[i++];
    }
    while (j <= right) 
    {
        if (stop)
            return;

        pauseLock.lock();
        pauseLock.unlock();
        if (oneStep)
            emit baseColumn(mid);
        emit compareColumn(i, j); 
        msleep(speed);

        temp[k++] = numbers[j++];
    }

    k = 0;                              
    for (int i = left; i <= right; i++) 
    {
        if (stop)
            return;

        pauseLock.lock();
        pauseLock.unlock();

        numbers[i] = temp[k++];
        oprtCnt++;

        emit updtColumn(i, true, numbers);
        msleep(speed);
    }

    delete[] temp; 
}

// 3 堆排序
void sortAlgorithms::heapSort()
{
    
    for (int i = dataNum / 2 - 1; i >= 0; i--)
    {
        if (stop)
            return;
        max_heapify(i, dataNum - 1);
    }


    for (int i = dataNum - 1; i > 0; i--)
    {
        if (stop)
            return;

        swap(0, i);

        oprtCnt++;
        pauseLock.lock();
        pauseLock.unlock();
        emit swapColumn(0, i, numbers);
        msleep(speed);
        pauseLock.lock(); 
        pauseLock.unlock();
        max_heapify(0, i - 1);
    }
}

// 堆排序_构建大根堆
void sortAlgorithms::max_heapify(int start, int end) 
{
    int fa = start;       
    int son = fa * 2 + 1; 

    while (son <= end) 
    {
        if (stop)
            return;

        if (son + 1 <= end && numbers[son] < numbers[son + 1]) 
        {
            pauseLock.lock();
            pauseLock.unlock();
            cmpCnt++;
            emit compareColumn(son, son + 1);
            msleep(speed);

            son++; 
        }

        pauseLock.lock();
        pauseLock.unlock();
        cmpCnt++;
        emit compareColumn(son, fa);
        msleep(speed);

        if (numbers[fa] > numbers[son]) 
            return;
        else
        {
            pauseLock.lock();
            pauseLock.unlock();

            swap(fa, son);
            emit swapColumn(fa, son, numbers);

            fa = son;          
            son = son * 2 + 1; 

            oprtCnt++;

            msleep(speed);
        }
    }
}

// 4 希尔排序
void sortAlgorithms::shellSort()
{

    int i, j;
    for (int gap = dataNum >> 1; gap >= 1; gap = gap >> 1)
    {
        if (stop)
            return;

        pauseLock.lock();
        pauseLock.unlock();
        for (i = gap; i < dataNum; i++)
        {
            if (stop)
                return;

            pauseLock.lock();
            pauseLock.unlock();
            cmpCnt++;
            emit compareColumn(i, i - gap);
            msleep(speed);

            if (numbers[i] < numbers[i - gap]) 
            {

                int temp = numbers[i];
                for (j = i - gap; j >= 0 && numbers[j] > temp; j = j - gap)
                {
                    if (stop)
                        return;

                    if (j != i - gap) 
                    {
                        pauseLock.lock();
                        pauseLock.unlock();
                        cmpCnt++;
                        emit compareColumn(i, j);
                        msleep(speed);
                    }
                    numbers[j + gap] = numbers[j]; 

                    pauseLock.lock();
                    pauseLock.unlock();
                    oprtCnt++;
                    emit compareColumn(j, j + gap); 
                    pauseLock.lock();
                    pauseLock.unlock();
                    emit updtColumn(j + gap, false, numbers); 
                    msleep(speed);
                }
                numbers[j + gap] = temp;

                oprtCnt++;
                pauseLock.lock();
                pauseLock.unlock();
                emit compareColumn(i, j + gap); 
                emit updtColumn(j + gap, true, numbers);
                msleep(speed);
            }
        }
    }
}

// 5 插入排序
void sortAlgorithms::insertSort()
{
    for (int i = 1; i < dataNum; i++)
    {
        if (stop)
            return;

        int key = numbers[i];
        int j = i - 1;

        pauseLock.lock();
        pauseLock.unlock();
        emit baseColumn(i);
        msleep(speed);

        while (j >= 0 && numbers[j] > key)
        {
            if (stop)
                return;

            pauseLock.lock();
            pauseLock.unlock();
            cmpCnt++;
            emit compareColumn(i, j);
            emit baseColumn(i);
            msleep(speed);

            numbers[j + 1] = numbers[j];

            oprtCnt++;
            emit updtColumn(j + 1, true, numbers);
            emit compareColumn(i, j); 
            emit baseColumn(i);
            msleep(speed);
            if (oneStep)
                resumeSorting(); 

            j--;
        }
        numbers[j + 1] = key;

        oprtCnt++;
        emit updtColumn(j + 1, true, numbers);
        msleep(speed);
    }
}

// 6 冒泡排序
void sortAlgorithms::bubbleSort()
{
    for (int i = 0; i < dataNum; i++)
    {
        for (int j = 0; j < dataNum - 1 - i; j++)
        {
            if (stop)
                return;

            pauseLock.lock();
            pauseLock.unlock();

            cmpCnt++;
            if (oneStep)
            {
                pauseLock.lock();
                pauseLock.unlock();
                emit compareColumn(j, j + 1);
                emit baseColumn(j); 
                msleep(speed);
            }

            if (numbers[j] > numbers[j + 1])
            {
                swap(j, j + 1);

                pauseLock.lock();
                pauseLock.unlock();
                emit swapColumn(j, j + 1, numbers);
                msleep(speed);
            }
        }
    }
}

// 7 选择排序
void sortAlgorithms::selectSort()
{
    for (int i = 0; i < dataNum - 1; i++) 
    {
        if (stop)
            return;

        int min = numbers[i], min_flag = i;

        pauseLock.lock();
        pauseLock.unlock();
        emit baseColumn(min_flag); // 指示最小值
        msleep(speed);

        for (int j = i + 1; j < dataNum; j++) // 遍历未排好序的序列，找出最小值
        {
            if (stop)
                return;

            if (!oneStep) 
            {
                pauseLock.lock();
                pauseLock.unlock();
            }

            cmpCnt++;                        
            emit compareColumn(min_flag, j); 
            emit baseColumn(min_flag);      
            msleep(speed);

            if (numbers[j] < min) 
            {
                min = numbers[j];
                min_flag = j;

                pauseLock.lock(); 
                pauseLock.unlock();
                emit baseColumn(min_flag); 
                msleep(speed);
            }
        }

        swap(i, min_flag); 

        oprtCnt++;
        pauseLock.lock();
        pauseLock.unlock();
        emit swapColumn(i, min_flag, numbers);
        msleep(speed);
    }
}
