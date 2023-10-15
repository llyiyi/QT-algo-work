#include "esort.h"

esort::esort(const char *unsort_file, const char *sort_file)
{
#define MIN -1       
#define MAX 10000000 
    typedef int *LoserTree;
    typedef int *External;

    const unsigned int count1 = 10000000;
    const unsigned int number_to_sort = 100000;


    void init_data(unsigned int num); // 随机生成数据文件
    class ExternSort
    {
    public:
        void sort()
        {
            time_t start = time(NULL);
            // 将文件内容分块在内存中排序，并写入临时文件
            k = memory_sort();

            ls = new int[k];
            b = new int[k + 1];

            K_Merge();
            delete[] ls;
            delete[] b;

            time_t end = time(NULL);
            cout << "total time:" << (end - start) * 1000.0 / CLOCKS_PER_SEC << endl;
        }

        // input_file:输入文件名
        // out_file：输出文件名
        // count：每次在内存中排序的整数个数

        ExternSort(const char *input_file, const char *out_file, int count)
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
        int m_count;      
        char *m_in_file;  
        char *m_out_file; 
        int k;           
        LoserTree ls;     
        External b;      

    protected:
        int read_data(FILE *f, int a[], int n)
        {
            int i = 0;
            while (i < n && (fscanf(f, "%d", &a[i]) != EOF))
                i++;
            cout << "read:" << i << " integer" << endl;
            return i;
        }

        void write_data(FILE *f, int a[], int n)
        {
            for (int i = 0; i < n; ++i)
                fprintf(f, "%d ", a[i]);
            fprintf(f, "%d", MAX); 
        }

        char *temp_filename(int index)
        {
            char *tempfile = new char[100];
            sprintf(tempfile, "temp%d.txt", index);
            return tempfile;
        }

        static int cmp_int(const void *a, const void *b)
        {
            return *(int *)a - *(int *)b;
        }

        int memory_sort()
        {
            FILE *fin = fopen(m_in_file, "rt");
            int n = 0, file_count = 0;
            int *array = new int[m_count];

            // 每读入m_count个整数就在内存中做一次排序，并写入临时文件
            while ((n = read_data(fin, array, m_count)) > 0)
            {
                qsort(array, n, sizeof(int), cmp_int);
                char *fileName = temp_filename(file_count++);
                FILE *tempFile = fopen(fileName, "w");
                free(fileName);
                write_data(tempFile, array, n);
                fclose(tempFile);
            }
            delete[] array;
            fclose(fin);

            return file_count;
        }

        void Adjust(int s) 
        {
           
            int t = (s + k) / 2; 
            while (t > 0)
            {
                if (b[s] > b[ls[t]]) 
                {
                    int tmp = s;
                    s = ls[t];
                    ls[t] = tmp;
                }

                t = t / 2;
            }

            ls[0] = s; 
        }

        void CreateLoserTree()
        {
            b[k] = MIN; 
            for (int i = 0; i < k; i++)
                ls[i] = k; 

            for (int i = k - 1; i >= 0; i--)
                Adjust(i); 
        }

        void K_Merge()
        {

            FILE *fout = fopen(m_out_file, "wt");
            FILE **farray = new FILE *[k]; // farray中存储的是文件指针
            int i;
            for (i = 0; i < k; ++i) 
            {
                char *fileName = temp_filename(i);
                farray[i] = fopen(fileName, "rt");
                free(fileName);
            }

            for (i = 0; i < k; ++i)
            {
                if (fscanf(farray[i], "%d", &b[i]) == EOF) 
                {
                    cout << "there is no" << k << " file to merge!" << endl;
                    return;
                }
            }

            CreateLoserTree();
            int q;
            while (b[ls[0]] != MAX)
            {
                q = ls[0]; 
                
                fprintf(fout, "%d ", b[q]);
               
                fscanf(farray[q], "%d", &b[q]);
                Adjust(q);
            }

            
            fprintf(fout, "%d ", b[ls[0]]);

            for (i = 0; i < k; ++i) // 清理工作
            {
                fclose(farray[i]);
            }

            delete[] farray;
            fclose(fout);

            
            for (i = 0; i < k; ++i)
            {
                char *fileName = temp_filename(i);
                remove(fileName);
                free(fileName);
            }
        }
    };

    ExternSort es(unsort_file, sort_file, number_to_sort);
    es.sort();
    return;
}
