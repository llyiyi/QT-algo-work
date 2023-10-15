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

    sortCtrl = new sortAlgorithms(); // 实例化排序控制器

    this->setWindowIcon(QIcon(":/icon.ico"));   // 选择图标
}

SortWindow::~SortWindow()
{
    delete ui;
}

// 导入数据按钮
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
    ui->label_status->setText(tr("文件装填状态：已填入文件 %1").arg(fileName));
    QTextStream in(&file);
    QString line;
    std::vector<int> temp;
    dataNum = 0;
    while (!in.atEnd())
    {
        line = in.readLine();
        temp.push_back(line.toInt());
        dataNum++;
    }
    numbers = new int[dataNum + 1];
    for (int i = 0; i < dataNum; i++)
        numbers[i] = temp[i];
    file.close();
    // 显示数据
}

void SortWindow::on_comboBox_algo_activated(int index)
{
    algorithmSelected = index;
    qDebug() << algorithmSelected;
}

// 开始排序按钮
void SortWindow::on_pushButton_2_clicked()
{
    if (dataNum == 0)
    {
        QMessageBox::warning(this, tr("警告"), tr("请先装填数据！"));
        return;
    }
    qDebug() << "开始排序，排序算法：" << algorithmSelected << "；文件名：" << fileName << endl;
    for (int i = 0; i < dataNum; i++)
        qDebug() << numbers[i];
    qDebug() << endl;
    sortCtrl->setOneStepState(false);
    sortCtrl->setSortingState(true);
    sortCtrl->setAttribute(dataNum, numbers, algorithmSelected, 0);
    sortCtrl->start();
    // 计时
    QTime t;
    t.start();
    sortCtrl->wait();
    qDebug() << "排序用时：" << t.elapsed() << "ms" << endl;
    ui->label_timer->setText(tr("耗时：%1ms").arg(t.elapsed()));
    qDebug() << "排序结束" << endl;
    QMessageBox::information(this, tr("排序完成"), tr("排序完成！选择保存文件。"));
    fileName = QFileDialog::getSaveFileName(this, tr("保存文件"), "", tr("Text Files (*.txt)"));
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, tr("写入文件失败"), tr("无法保存文件：\n%1").arg(fileName));
        return;
    }
    QTextStream out(&file);
    if (ui->checkBox_dec->isChecked())
        for (int i = dataNum - 1; i >= 0; i--)
            out << numbers[i] << endl;
    else
        for (int i = 0; i < dataNum; i++)
            out << numbers[i] << endl;
    file.close();
    ui->label_status->setText(tr("文件保存状态：已保存文件到 %1").arg(fileName));
}
