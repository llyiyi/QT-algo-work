#include "sortwindow.h"
#include "ui_sortwindow.h"

SortWindow::SortWindow(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::SortWindow)
{
    ui->setupUi(this);
    // 给下拉框添加内容
    ui->comboBox_algo->addItem(tr("基数排序  O(N)     稳定"));   //  Index 0      O(n)
    ui->comboBox_algo->addItem(tr("快速排序  O(NlogN) 不稳定")); //  Index 1      O(nlogn)
    ui->comboBox_algo->addItem(tr("归并排序  O(NlogN) 稳定"));   //  Index 2      O(nlogn)
    ui->comboBox_algo->addItem(tr(" 堆排序   O(NlogN) 不稳定")); //  Index 3      O(nlogn)
    ui->comboBox_algo->addItem(tr("希尔排序  O(N^1.5) 不稳定")); //  Index 4      O(n^1.5)
    ui->comboBox_algo->addItem(tr("插入排序  O(N^2)   稳定"));   //  Index 5      O(n^2)
    ui->comboBox_algo->addItem(tr("冒泡排序  O(N^2)   稳定"));   //  Index 6      O(n^2)
    ui->comboBox_algo->addItem(tr("选择排序  O(N^2)   不稳定")); //  Index 7      O(n^2)
    ui->comboBox_algo->setCurrentIndex(1);
}

SortWindow::~SortWindow()
{
    delete ui;
}

void SortWindow::on_pushButton_clicked()
{
    // 打开选择文件对话框
    fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Text Files (*.txt)"));
    if (fileName.isEmpty())
        return;
    qDebug() << fileName;
    // 读取文件
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, tr("Read File"), tr("Cannot open file:\n%1").arg(fileName));
        return;
    }
    QTextStream in(&file);
    QString line = in.readLine();
    dataNum = line.toInt();
    numbers = new int[dataNum];
    for (int i = 0; i < dataNum; i++)
    {
        line = in.readLine();
        numbers[i] = line.toInt();
    }
    file.close();
    // 显示数据
}

void SortWindow::on_comboBox_algo_activated(int index)
{
    algorithmSelected = index;
    qDebug() << algorithmSelected;
}

void SortWindow::on_pushButton_2_clicked()
{
    if (dataNum == 0)
    {
        QMessageBox::warning(this, tr("Error"), tr("Please import data first!"));
        return;
    }
    qDebug() << "开始排序，排序算法：" << algorithmSelected << "；文件名：" << fileName << endl;
    sortCtrl->setSortingState(true);
    sortCtrl->setAttribute(dataNum, numbers, algorithmSelected, sortSpeed);
    sortCtrl->start();
}
