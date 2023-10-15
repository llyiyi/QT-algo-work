#include "esort.h"

esort::esort()
{
#define MIN -1	//这里开始的时候出现了一个BUG，如果定义的MIN大于等于待排序的数，则会是算法出现错误
#define MAX 10000000	//最大值，附加在归并文件结尾
    typedef int* LoserTree;
    typedef int* External;

    const unsigned int count1 = 10000000;
    const unsigned int number_to_sort = 100000;
    const char* unsort_file = "unsort_data.txt";
    const char* sort_file = "sort_data.txt";

    void init_data(unsigned int num); //随机生成数据文件
    class ExternSort
    {
    public:
        void sort()
        {
            time_t start = time(NULL);
            //将文件内容分块在内存中排序，并写入临时文件
            k = memory_sort();

            ls = new int[k];
            b = new int[k + 1];

            K_Merge();
            delete[] ls;
            delete[] b;

            time_t end = time(NULL);
            cout << "total time:" << (end - start)*1000.0 / CLOCKS_PER_SEC << endl;
        }

        //input_file:输入文件名
        //out_file：输出文件名
        //count：每次在内存中排序的整数个数

        ExternSort(const char* input_file, const char* out_file, int count)
        {
            m_count = count;
            m_in_file = new char[strlen(input_file) + 1];
            strcpy(m_in_file, input_file);
            m_out_file = new char[strlen(out_file) + 1];
            strcpy(m_out_file, out_file);

        }

        virtual ~ExternSort()
        {
            delete[] m_in_file;
            delete[] m_out_file;
        }


    private:
        int m_count;//数组长度
        char* m_in_file; //输入文件的路径
        char* m_out_file; //输出文件的路径
        int k; //归并数，此数必须要内排序之后才能得到，所以下面的ls和b都只能定义为指针
        LoserTree ls;//定义成为指针，之后动态生成数组
        External b;//定义成为指针，在成员函数中可以把它当成数组使用

    protected:
        int read_data(FILE* f, int a[], int n)
        {
            int i = 0;
            while (i < n && (fscanf(f, "%d", &a[i]) != EOF))
                i++;
            cout << "read:" << i << " integer" << endl;
            return i;
        }

        void write_data(FILE* f, int a[], int n)
        {
            for (int i = 0; i < n; ++i)
                fprintf(f, "%d ", a[i]);
            fprintf(f, "%d", MAX);//在最后写上一个最大值
        }

        char* temp_filename(int index)
        {
            char *tempfile = new char[100];
            sprintf(tempfile, "temp%d.txt", index);
            return tempfile;
        }

        static int cmp_int(const void *a, const void* b)
        {
            return *(int*)a - *(int*)b;
        }

        int memory_sort()
        {
            FILE* fin = fopen(m_in_file, "rt");
            int n = 0, file_count = 0;
            int* array = new int[m_count];

            //每读入m_count个整数就在内存中做一次排序，并写入临时文件
            while ((n = read_data(fin, array, m_count)) > 0)
            {
                qsort(array, n, sizeof(int), cmp_int);
                char* fileName = temp_filename(file_count++);
                FILE* tempFile = fopen(fileName, "w");
                free(fileName);
                write_data(tempFile, array, n);
                fclose(tempFile);


            }
            delete[] array;
            fclose(fin);

            return file_count;

        }


        void Adjust(int s) //此处可通过画出败者树的图来理解参考博客http://blog.csdn.net/whz_zb/article/details/7425152
        {
            //沿从叶子节点b[s]到根节点ls[0]的路径调整败者树
            int t = (s + k) / 2;  //ls[t]是b[s]的双亲节点
            while (t > 0)
            {
                if (b[s] > b[ls[t]])//如果失败，则失败者位置s留下，s指向新的胜利者
                {
                    int tmp = s;
                    s = ls[t];
                    ls[t] = tmp;
                }

                t = t / 2;
            }

            ls[0] = s; //ls[0]存放调整后最大值的位置

        }

        void CreateLoserTree()
        {
            b[k] = MIN;//额外的存储一个最小值
            for (int i = 0; i < k; i++)
                ls[i] = k; //先初始化为指向最小值，这样后面的调整才是正确的,这样能保证非叶子节点都是子树中的“二把手”

            for (int i = k - 1; i >= 0; i--)
                Adjust(i);//依次从b[k-1],b[k-2]...b[0]出发调整败者树
        }

        void K_Merge()
        {
            //利用败者数把k个输入归并段归并到输出段中
            //b中前k个变量存放k个输入段中当前记录的元素
            //归并临时文件

            FILE* fout = fopen(m_out_file, "wt");
            FILE** farray = new FILE*[k]; //farray中存储的是文件指针
            int i;
            for (i = 0; i < k; ++i)  //打开所有k路输入文件
            {
                char* fileName = temp_filename(i);
                farray[i] = fopen(fileName, "rt");
                free(fileName);
            }

            for (i = 0; i < k; ++i)//初始读取
            {
                if (fscanf(farray[i], "%d", &b[i]) == EOF) //读每个文件的第一个数到data数组
                {
                    cout << "there is no" << k << " file to merge!" << endl;
                    return;
                }
            }

            CreateLoserTree();
            int q;
            while (b[ls[0]] != MAX)
            {
                q = ls[0];//q用来存储b中最小值的位置，同时也对应一路文件
                //output(q)
                fprintf(fout, "%d ", b[q]);
                //input(b[q],q);
                fscanf(farray[q], "%d", &b[q]);
                Adjust(q);
            }

            //output(ls[0]);
            fprintf(fout, "%d ", b[ls[0]]);

            for (i = 0; i < k; ++k)  //清理工作
            {
                fclose(farray[i]);
            }

            delete[] farray;
            fclose(fout);

        }

    };

}
