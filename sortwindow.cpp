#include "sortwindow.h"
#include "ui_sortwindow.h"

SortWindow::SortWindow(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::SortWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("其他排序"));
    this->setWindowIcon(QIcon(":/icon.ico"));

    int fontId = QFontDatabase::addApplicationFont(QStringLiteral(":/font/hgy3.ttf"));

    QFont font;
    QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);

    font.setFamily(fontFamilies[0]);
    font.setPixelSize(12);
    ui->label_status->setFont(font);
    font.setPixelSize(20);
    ui->label_timer->setFont(font);

    QFile styleFile(":/qdarkstyle/style.qss");
    QString styleSheet;
    if (styleFile.open(QIODevice::ReadOnly))
    {

        styleSheet = QLatin1String(styleFile.readAll());
        ui->centralwidget->setStyleSheet(styleSheet);
        ui->menubar->setStyleSheet(styleSheet);
        ui->statusbar->setStyleSheet(styleSheet);
    }
    styleFile.close();

    // 给下拉框添加内容
    ui->comboBox_algo->addItem(tr("基数排序  O(N)     稳定"));   //  Index 0      O(n)
    ui->comboBox_algo->addItem(tr("快速排序  O(NlogN) 不稳定")); //  Index 1      O(nlogn)
    ui->comboBox_algo->addItem(tr("归并排序  O(NlogN) 稳定"));   //  Index 2      O(nlogn)
    ui->comboBox_algo->addItem(tr(" 堆排序   O(NlogN) 不稳定")); //  Index 3      O(nlogn)
    ui->comboBox_algo->addItem(tr("希尔排序  O(N^1.5) 不稳定")); //  Index 4      O(n^1.5)
    ui->comboBox_algo->addItem(tr("插入排序  O(N^2)   稳定"));   //  Index 5      O(n^2)
    ui->comboBox_algo->addItem(tr("冒泡排序  O(N^2)   稳定"));   //  Index 6      O(n^2)
    ui->comboBox_algo->addItem(tr("选择排序  O(N^2)   不稳定")); //  Index 7      O(n^2)
    ui->comboBox_algo->addItem(tr("外部排序"));                  //  Index 8
    ui->comboBox_algo->setCurrentIndex(1);

    sortCtrl = new sortalgotime(); // 实例化排序控制器

    this->setWindowIcon(QIcon(":/icon.ico")); // 选择图标
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
    ui->label_status->setText(tr("装填状态：已填入文件 %1").arg(fileName));
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
    sorted = false;
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
    if (sorted)
    {
        QMessageBox::warning(this, tr("警告"), tr("请先重新生成数据！"));
        return;
    }
    qDebug() << "开始排序，排序算法：" << algorithmSelected << "；文件名：" << fileName << endl;
    if (algorithmSelected == 8)
    {
        if (fileName.isEmpty())
        {
            QMessageBox::warning(this, tr("警告"), tr("请先装填数据！"));
            return;
        }
        QTime t;
        t.start();
        esortCtrl = new esort(fileName.toStdString().c_str(), "out.txt");
        qDebug() << "外部排序用时：" << t.elapsed() << "ms" << endl;
        ui->label_timer->setText(tr("耗时：%1ms").arg(t.elapsed()));
        return;
    }
    sortCtrl->setSortingState(true);
    sortCtrl->setAttribute(dataNum, numbers, algorithmSelected);
    sortCtrl->start();
    // 计时
    QTime t;
    t.start();
    sortCtrl->wait();
    qDebug() << "排序用时：" << t.elapsed() << "ms" << endl;
    ui->label_timer->setText(tr("耗时：%1ms\n比较次数：%2\n操作次数：%3").arg(t.elapsed()).arg(sortCtrl->getCmpCnt()).arg(sortCtrl->getOprtCnt()));
    qDebug() << "排序结束" << endl;
    sorted = true;
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

// 生成数据按钮
void SortWindow::on_pushButton_genData_clicked()
{
    if (algorithmSelected == 8)
    {
        QMessageBox::warning(this, tr("警告"), tr("外部排序不需要生成数据！"));
        return;
    }
    srand(time(NULL));
    // 要求输入数据量
    bool ok;
    int temp = QInputDialog::getInt(this, tr("输入数据量"), tr("请输入数据量："), 100, 1, 1000000, 1, &ok);
    if (ok)
        dataNum = temp;
    else
        return;
    // 生成随机数据，装填到numbers数组中
    numbers = new int[dataNum + 1];
    for (int i = 0; i < dataNum; i++)
        numbers[i] = rand() % 1000000;
    sorted = false;
    // 显示数据
    ui->label_status->setText(tr("装填状态：已生成随机数据%1个").arg(QString::number(dataNum)));
}
