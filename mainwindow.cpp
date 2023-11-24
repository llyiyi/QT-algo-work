#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sortwindow.h"
#include "bestsortwin.h"
#include "statistics.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("排序算法性能比较与可视化及应用"));
    this->setWindowIcon(QIcon(":/icon.ico"));

    // 添加需要的字体
    int fontId = QFontDatabase::addApplicationFont(QStringLiteral(":/font/hgy3.ttf"));

    // 设置字体
    QFont font;
    QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);

    font.setFamily(fontFamilies[0]);
    font.setPixelSize(14);
    ui->label_static_algorithmShow->setFont(font);
    font.setPixelSize(13);
    ui->lable_static_sortingDataNum->setFont(font);
    ui->lable_static_sortingCompareCnt->setFont(font);
    ui->lable_static_sortingSwapCnt->setFont(font);
    ui->lable_static_sortingTime->setFont(font);

    // 添加菜单栏和对应槽
    initMenuBar();

    // 状态栏设置
    sortName = new QLabel;
    oprtContent = new QLabel;
    sortDscrb = new QLabel;
    keyValue = new QLabel;

    sortName->setStyleSheet("font: 12px \"宋体\";");
    oprtContent->setStyleSheet("font: 12px \"宋体\";");
    sortDscrb->setStyleSheet("font: 12px \"宋体\";");
    keyValue->setStyleSheet("font: 12px \"宋体\";");

    sortName->setFixedWidth(300); // 初始显示欢迎语，宽度后面会改
    oprtContent->setFixedWidth(190);
    sortDscrb->setFixedWidth(310);
    keyValue->setFixedWidth(250);
    ui->statusbar->addWidget(sortName);
    ui->statusbar->addWidget(oprtContent);
    ui->statusbar->addWidget(sortDscrb);
    ui->statusbar->addWidget(keyValue);
    ui->statusbar->clearMessage();
    sortName->setText("欢迎旅行者们，请开始你的排序！");

    // 新建排序线程
    sortCtrl = new sortAlgorithms();
    timer = new QTimer(this);

    // 排序数据量默认为100，再次添加数据时默认为上次排序数据量
    dataNum = 100;
    numbers = new int[10];

    // 随机数种子
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));

    // 速度滑块设置 通过sortAlgorithms类中msleep()睡眠设置的时间调节快慢
    ui->slider_sortingSpeed->setMinimum(1);
    ui->slider_sortingSpeed->setMaximum(100);
    ui->slider_sortingSpeed->setValue(50);

    // 鼠标经过时手型图标
    ui->btn_dataGenerate->setCursor(QCursor(Qt::PointingHandCursor));
    ui->btn_sortingStart->setCursor(QCursor(Qt::PointingHandCursor));
    ui->btn_sortingPause->setCursor(QCursor(Qt::PointingHandCursor));
    ui->btn_sortingContinue->setCursor(QCursor(Qt::PointingHandCursor));
    ui->btn_oneStep->setCursor(QCursor(Qt::PointingHandCursor));
    ui->slider_sortingSpeed->setCursor(QCursor(Qt::PointingHandCursor));
    ui->comboBox_sortingalgorithm->setCursor(QCursor(Qt::PointingHandCursor));

    // 给下拉框添加内容
    ui->comboBox_sortingalgorithm->addItem(tr("基数排序  O(N)     稳定"));   //  Index 0      O(n)
    ui->comboBox_sortingalgorithm->addItem(tr("快速排序  O(NlogN) 不稳定")); //  Index 1      O(nlogn)
    ui->comboBox_sortingalgorithm->addItem(tr("归并排序  O(NlogN) 稳定"));   //  Index 2      O(nlogn)
    ui->comboBox_sortingalgorithm->addItem(tr(" 堆排序   O(NlogN) 不稳定")); //  Index 3      O(nlogn)
    ui->comboBox_sortingalgorithm->addItem(tr("希尔排序  O(N^1.5) 不稳定")); //  Index 4      O(n^1.5)
    ui->comboBox_sortingalgorithm->addItem(tr("插入排序  O(N^2)   稳定"));   //  Index 5      O(n^2)
    ui->comboBox_sortingalgorithm->addItem(tr("冒泡排序  O(N^2)   稳定"));   //  Index 6      O(n^2)
    ui->comboBox_sortingalgorithm->addItem(tr("选择排序  O(N^2)   不稳定")); //  Index 7      O(n^2)
    ui->comboBox_sortingalgorithm->setCurrentIndex(1);

    // 设置样式表
    QFile styleFile(":/qdarkstyle/style.qss");
    QString styleSheet;
    if (styleFile.open(QIODevice::ReadOnly))
    {

        styleSheet = QLatin1String(styleFile.readAll());
        ui->centralwidget->setStyleSheet(styleSheet);
        ui->menubar->setStyleSheet(styleSheet);
        ui->statusbar->setStyleSheet(styleSheet);
        menu_algorithm->setStyleSheet(styleSheet);
        menu_windows->setStyleSheet(styleSheet);
    }
    styleFile.close();

    ui->ShowSortingVisulizaion->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #6495ED ,stop:1 #4682B4);"
                                              "border-color: qlineargradient(spread:pad, x1:0.767, y1:0, x2:1, y2:1, stop:0 rgba(47, 79, 79, 255), stop:1 rgba(119, 178, 205, 255));"
                                              "border-width: 1px;"
                                              "border-style: solid;");

    // 连接信号和槽
    connect(sortCtrl, SIGNAL(sortFinish()), this, SLOT(showFinish()));                                      // 完成动画
    connect(sortCtrl, SIGNAL(swapColumn(int, int, int *)), this, SLOT(updateColumns(int, int, int *)));     // 更新两个柱子的值，与第三个参数数组中的一致。并不是真正的交换两个柱子
    connect(sortCtrl, SIGNAL(updtColumn(int, bool, int *)), this, SLOT(updateOneColumn(int, bool, int *))); // 更新一个柱子的值
    connect(sortCtrl, SIGNAL(compareColumn(int, int)), this, SLOT(highLightColumns(int, int)));             // 高亮比较元素
    connect(sortCtrl, SIGNAL(baseColumn(int)), this, SLOT(highLightBase(int)));                             // 高亮排序基准值（对于需要基准值的算法）
    connect(sortCtrl, SIGNAL(showSBMsg(int, QString)), this, SLOT(sbShow(int, QString)));

    connect(ui->slider_sortingSpeed, SIGNAL(valueChanged(int)), this, SLOT(setSpeed(int)));            // 通过滑块设置速度
    connect(ui->comboBox_sortingalgorithm, SIGNAL(activated(int)), this, SLOT(sortingAlgorithm(int))); // 通过下拉框选择排序算法
    connect(timer, SIGNAL(timeout()), this, SLOT(updateSortingTime()));
}

// 初始化菜单栏和对应的槽
void MainWindow::initMenuBar()
{
    // 添加菜单栏
    menu_algorithm = new QMenu(tr("算法可视化演示"), this);
    menu_windows = new QMenu(tr("排序数据"), this);

    ui->menubar->addMenu(menu_algorithm);
    ui->menubar->addMenu(menu_windows);

    // 定义菜单中的动作
    actRadixSort = new QAction("基数排序", this);
    actQuickSort = new QAction("快速排序", this);
    actMergeSort = new QAction("归并排序", this);
    actHeapSort = new QAction("堆排序", this);
    actShellSort = new QAction("希尔排序", this);
    actInsertSort = new QAction("插入排序", this);
    actBubbleSort = new QAction("冒泡排序", this);
    actSelectSort = new QAction("选择排序", this);

    actSortWindow = new QAction("其他排序", this);
    actBestSort = new QAction("最强排序", this);
    actStatistics = new QAction("统计信息", this);

    // 添加动作
    menu_algorithm->addAction(actRadixSort);
    menu_algorithm->addAction(actQuickSort);
    menu_algorithm->addAction(actMergeSort);
    menu_algorithm->addAction(actHeapSort);
    menu_algorithm->addAction(actShellSort);
    menu_algorithm->addAction(actInsertSort);
    menu_algorithm->addAction(actBubbleSort);
    menu_algorithm->addAction(actSelectSort);
    menu_windows->addAction(actSortWindow);
    menu_windows->addAction(actBestSort);
    menu_windows->addAction(actStatistics);

    // 连接动作
    connect(actRadixSort, SIGNAL(triggered()), this, SLOT(menuAct_actRadixSort()));
    connect(actQuickSort, SIGNAL(triggered()), this, SLOT(menuAct_actQuickSort()));
    connect(actMergeSort, SIGNAL(triggered()), this, SLOT(menuAct_actMergeSort()));
    connect(actHeapSort, SIGNAL(triggered()), this, SLOT(menuAct_actHeapSort()));
    connect(actShellSort, SIGNAL(triggered()), this, SLOT(menuAct_actShellSort()));
    connect(actInsertSort, SIGNAL(triggered()), this, SLOT(menuAct_actInsertSort()));
    connect(actBubbleSort, SIGNAL(triggered()), this, SLOT(menuAct_actBubbleSort()));
    connect(actSelectSort, SIGNAL(triggered()), this, SLOT(menuAct_actSelectSort()));
    connect(actSortWindow, SIGNAL(triggered()), this, SLOT(menuAct_newWindow()));
    connect(actBestSort, SIGNAL(triggered()), this, SLOT(menuAct_bestwin()));
    connect(actStatistics, SIGNAL(triggered()), this, SLOT(menuAct_statistics()));
}

MainWindow::~MainWindow()
{
    delete ui;
    sortCtrl->stopSorting();
    sortCtrl->quit();
    sortCtrl->wait();
    delete sortCtrl;
    delete[] numbers;
    delete[] columns;
    delete timer;
    delete oprtContent;
    delete sortDscrb;
}

// 用户选择了排序算法
void MainWindow::sortingAlgorithm(int index)
{
    algorithmSelected = index;
    qDebug() << "用户当前选择算法为：" << algorithmSelected << " " << ui->comboBox_sortingalgorithm->currentIndex() << " " << ui->comboBox_sortingalgorithm->currentText() << endl;
}

// 设置排序速度
void MainWindow::setSpeed(int speed)
{
    this->sortSpeed = 101 - speed;
    sortCtrl->setSpeed(this->sortSpeed);
    qDebug() << "当前速度：" << sortSpeed << endl;
}

// 生成数据按钮
void MainWindow::on_btn_dataGenerate_clicked()
{
    if (sortCtrl->isSorting()) // 正在排序中，询问是否用新数据排序
    {
        if (QMessageBox::Ok == QMessageBox::question(this, "提示", "正在排序中，\n是否停止本次排序，并重新生成数据？", QMessageBox::Ok | QMessageBox::Cancel))
        {
            if (sortCtrl->isPause())
                sortCtrl->resumeSorting(); // 处于暂停状态时先结束暂停，否则新一轮排序无法开始
            if (sortCtrl->isOneStep())
                sortCtrl->setOneStepState(false); // 处于单步执行状态时先结束单步执行
            sortCtrl->stopSorting();              // 需要生成数据，先停止排序
        }
        else
            return;
    }

    if (dataGenerating || dataImporting) // 正在生成数据
    {
        QMessageBox::information(this, "提示", "正在生成数据，请等待！");
        return;
    }

    bool bOk = false;
    dataNum = QInputDialog::getInt(this, "设置数据量", "请输入演示的数据量(2-300):", dataNum, 2, 300, 10, &bOk);
    if (bOk && dataNum >= 0)
    {
        qDebug() << "数据量：" << dataNum << endl;
        dataGenerating = 1; // 修改状态，防止生成过程中再次点击生成/导入数据按钮

        dataGenerator(); // 生成排序数据

        // 数据生成过程结束，修改各种状态和界面显示
        initVar();
        initLabel();
        timer->stop();
        prevDataNum = dataNum; // 数据添加完毕，记录本次数据量
        dataGenerating = 0;
        datahaved = 1;

        // 设置可以调整窗口大小（每次开始排序时禁止改变窗口大小，点击生成/导入数据会停止上次排序，数据添加完毕后恢复窗口大小设置）
        this->setMinimumSize(800, 600);
        this->setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));

        // 修改窗口标题
        this->setWindowTitle(QString("排序算法可视化系统"));
    }
}

// 开始排序按钮
void MainWindow::on_btn_sortingStart_clicked()
{
    // 数据已被排序，需要生成新数据
    if (sorted)
    {
        QMessageBox::information(this, "提示", "数据已被排序，请重新生成数据！");
        return;
    }

    // 排序期间不能再次开始排序
    if (sortCtrl->isSorting())
    {
        QMessageBox::warning(this, "提示", "程序正在排序中！");
        return;
    }

    // 没有数据不能开始排序
    if (!datahaved)
    {
        QMessageBox::warning(this, "提示", "请先生成或导入数据！");
        return;
    }

    // 状态栏显示排序算法   不直接调用comboBox的currentText，避免时间复杂度和稳定性间距太大，阅读不舒服
    if (algorithmSelected == 0)
        sbShow(1, "基数排序");
    else if (algorithmSelected == 1)
        sbShow(1, "快速排序");
    else if (algorithmSelected == 2)
        sbShow(1, "归并排序");
    else if (algorithmSelected == 3)
        sbShow(1, "堆排序");
    else if (algorithmSelected == 4)
        sbShow(1, "希尔排序");
    else if (algorithmSelected == 5)
        sbShow(1, "插入排序");
    else if (algorithmSelected == 6)
        sbShow(1, "冒泡排序");
    else if (algorithmSelected == 7)
        sbShow(1, "选择排序");

    // label显示数据
    if (algorithmSelected == 0)
        ui->label_algorithmShow->setText(tr("基数排序 - O(N) - 稳定")); // 左上角显示算法名称
    else if (algorithmSelected == 1)
        ui->label_algorithmShow->setText(tr("快速排序 - O(NlogN) - 不稳定"));
    else if (algorithmSelected == 2)
        ui->label_algorithmShow->setText(tr("归并排序 - O(NlogN) - 稳定"));
    else if (algorithmSelected == 3)
        ui->label_algorithmShow->setText(tr("堆排序  - O(NlogN) - 不稳定"));
    else if (algorithmSelected == 4)
        ui->label_algorithmShow->setText(tr("希尔排序 - O(N^1.5) - 不稳定"));
    else if (algorithmSelected == 5)
        ui->label_algorithmShow->setText(tr("插入排序 - O(N^2) - 稳定"));
    else if (algorithmSelected == 6)
        ui->label_algorithmShow->setText(tr("冒泡排序 - O(N^2) - 稳定"));
    else if (algorithmSelected == 7)
        ui->label_algorithmShow->setText(tr("选择排序 - O(N^2) - 不稳定"));
    ui->label_dataNum->setNum(dataNum); // 数据量

    // 开始计时
    timer->start(10); // 计时器每10ms即0.01秒更新一次

    // 排序结束前不可调整窗口大小
    this->setMinimumSize(this->width(), this->height());
    this->setMaximumSize(this->width(), this->height());
    //    this->setWindowFlags(this->windowFlags()& ~Qt::WindowMaximizeButtonHint);
    //    this->show();

    // 修改窗口标题
    this->setWindowTitle(QString("排序算法可视化系统   --正在排序 （此时无法调整窗口大小）"));

    // 设置单步执行不开启
    sortCtrl->setOneStepState(false);

    // 开启排序线程
    qDebug() << algorithmSelected << endl;
    sortCtrl->setSortingState(true);
    sortCtrl->setAttribute(dataNum, numbers, algorithmSelected, sortSpeed);
    sortCtrl->start(); // 调用start()自动执行其中的run()
}

// 暂停排序按钮
void MainWindow::on_btn_sortingPause_clicked()
{
    if (!sortCtrl->isSorting())
    {
        QMessageBox::warning(this, "提示", "当前没有排序正在进行！");
        return;
    }

    timer->stop();
    sortCtrl->pauseSorting();
}

// 继续排序按钮
void MainWindow::on_btn_sortingContinue_clicked()
{
    if (!sortCtrl->isSorting())
    {
        QMessageBox::warning(this, "提示", "当前没有排序正在进行！");
        return;
    }

    timer->start(10);

    sortCtrl->setOneStepState(false); // 单步执行后可通过此按钮恢复
    sortCtrl->resumeSorting();
}

// 单步执行按钮
void MainWindow::on_btn_oneStep_clicked()
{
    if (!sortCtrl->isSorting()) // 没开始排序时
    {
        on_btn_sortingStart_clicked(); // 开始排序
        sortCtrl->setOneStepState(true);
    }
    else
    {
        sortCtrl->setOneStepState(true);
        sortCtrl->resumeSorting();
    }
}

// 菜单栏-算法
void MainWindow::menuAct_actRadixSort()
{

    ui->comboBox_sortingalgorithm->setCurrentIndex(0);
    algorithmSelected = 0;
    qDebug() << "用户当前选择算法为：" << algorithmSelected << " " << ui->comboBox_sortingalgorithm->currentIndex() << " " << ui->comboBox_sortingalgorithm->currentText() << endl;
}

void MainWindow::menuAct_actQuickSort()
{

    ui->comboBox_sortingalgorithm->setCurrentIndex(1);
    algorithmSelected = 1;
    qDebug() << "用户当前选择算法为：" << algorithmSelected << " " << ui->comboBox_sortingalgorithm->currentIndex() << " " << ui->comboBox_sortingalgorithm->currentText() << endl;
}

void MainWindow::menuAct_actMergeSort()
{

    ui->comboBox_sortingalgorithm->setCurrentIndex(2);
    algorithmSelected = 2;
    qDebug() << "用户当前选择算法为：" << algorithmSelected << " " << ui->comboBox_sortingalgorithm->currentIndex() << " " << ui->comboBox_sortingalgorithm->currentText() << endl;
}

void MainWindow::menuAct_actHeapSort()
{

    ui->comboBox_sortingalgorithm->setCurrentIndex(3);
    algorithmSelected = 3;
    qDebug() << "用户当前选择算法为：" << algorithmSelected << " " << ui->comboBox_sortingalgorithm->currentIndex() << " " << ui->comboBox_sortingalgorithm->currentText() << endl;
}

void MainWindow::menuAct_actShellSort()
{

    ui->comboBox_sortingalgorithm->setCurrentIndex(4);
    algorithmSelected = 4;
    qDebug() << "用户当前选择算法为：" << algorithmSelected << " " << ui->comboBox_sortingalgorithm->currentIndex() << " " << ui->comboBox_sortingalgorithm->currentText() << endl;
}

void MainWindow::menuAct_actInsertSort()
{

    ui->comboBox_sortingalgorithm->setCurrentIndex(5);
    algorithmSelected = 5;
    qDebug() << "用户当前选择算法为：" << algorithmSelected << " " << ui->comboBox_sortingalgorithm->currentIndex() << " " << ui->comboBox_sortingalgorithm->currentText() << endl;
}

void MainWindow::menuAct_actBubbleSort()
{

    ui->comboBox_sortingalgorithm->setCurrentIndex(6);
    algorithmSelected = 6;
    qDebug() << "用户当前选择算法为：" << algorithmSelected << " " << ui->comboBox_sortingalgorithm->currentIndex() << " " << ui->comboBox_sortingalgorithm->currentText() << endl;
}

void MainWindow::menuAct_actSelectSort()
{

    ui->comboBox_sortingalgorithm->setCurrentIndex(7);
    algorithmSelected = 7;
    qDebug() << "用户当前选择算法为：" << algorithmSelected << " " << ui->comboBox_sortingalgorithm->currentIndex() << " " << ui->comboBox_sortingalgorithm->currentText() << endl;
}

void MainWindow::menuAct_newWindow()
{
    // 打开新窗体
    SortWindow *w = new SortWindow();
    w->show();
}

void MainWindow::menuAct_bestwin()
{
    // 打开新窗体
    bestSortWin *w = new bestSortWin();
    w->show();
}

void MainWindow::menuAct_statistics()
{
    // 打开新窗体
    statistics *w = new statistics();
    w->show();
}

// 更新a和b两个位置的柱子的值，与排序后的新值newHeights保持一致。与交换信号关联，可表示交换操作
void MainWindow::updateColumns(int a, int b, int *newHeights)
{
    if (a < 0 || a >= dataNum)
        return;
    if (b < 0 || b >= dataNum)
        return;

    qDebug() << "update-col: " << a << " " << b << " " << sortCtrl->isOneStep() << endl;

    // 重置上次交换的元素的颜色
    if (prevUpdtA >= 0 && prevUpdtB >= 0)
    {
        columns[prevUpdtA].setStyleSheet("background-color: white;"
                                         "border-color: qlineargradient(spread:pad, x1:0.767, y1:0, x2:1, y2:1, stop:0 rgba(76, 128, 209, 255), stop:1 rgba(119, 178, 205, 255));"
                                         "border-width: 1px;"
                                         "border-style: solid;");
        columns[prevUpdtB].setStyleSheet("background-color: white;"
                                         "border-color: qlineargradient(spread:pad, x1:0.767, y1:0, x2:1, y2:1, stop:0 rgba(76, 128, 209, 255), stop:1 rgba(119, 178, 205, 255));"
                                         "border-width: 1px;"
                                         "border-style: solid;");
    }
    // 重置上次比较的元素的颜色
    if (prevCmpA >= 0 && prevCmpB >= 0)
    {
        columns[prevCmpA].setStyleSheet("background-color: white;"
                                        "border-color: qlineargradient(spread:pad, x1:0.767, y1:0, x2:1, y2:1, stop:0 rgba(76, 128, 209, 255), stop:1 rgba(119, 178, 205, 255));"
                                        "border-width: 1px;"
                                        "border-style: solid;");
        columns[prevCmpB].setStyleSheet("background-color: white;"
                                        "border-color: qlineargradient(spread:pad, x1:0.767, y1:0, x2:1, y2:1, stop:0 rgba(76, 128, 209, 255), stop:1 rgba(119, 178, 205, 255));"
                                        "border-width: 1px;"
                                        "border-style: solid;");
    }
    // 用红色指示当前交换的元素
    columns[a].setStyleSheet("background-color: red;"
                             "border-color: red;"
                             "border-width: 1px;"
                             "border-style: solid;");
    columns[b].setStyleSheet("background-color: red;"
                             "border-color: red;"
                             "border-width: 1px;"
                             "border-style: solid;");

    // 交换元素
    columns[a].setFixedHeight(multiHigh * newHeights[a]);
    columns[b].setFixedHeight(multiHigh * newHeights[b]);
    prevUpdtA = a, prevUpdtB = b;

    // 更新统计并显示比较和交换次数
    ui->label_oprtCnt->setNum((int)sortCtrl->getOprtCnt());
    ui->label_cmpCnt->setNum((int)sortCtrl->getCmpCnt());

    // 单步执行暂停
    if (sortCtrl->isOneStep())
        sortCtrl->pauseSorting();
}

// 更新一个柱子的值，与排序后的新值newHeights保持一致
void MainWindow::updateOneColumn(int c, bool resetPrev, int *newHeights) // newHeights与numbers是一个数组。为了含义清晰，所以重新传回来作为newHeights
{
    if (c < 0 || c >= dataNum)
        return;

    qDebug() << "update-1-col: " << c << sortCtrl->isOneStep() << endl;

    // 重置前面所有交换后未重置元素的颜色
    if (resetPrev)
    {
        for (int i = 0; i < prevUpdtC.size(); i++)
            columns[prevUpdtC.at(i)].setStyleSheet("background-color: white;"
                                                   "border-color: qlineargradient(spread:pad, x1:0.767, y1:0, x2:1, y2:1, stop:0 rgba(76, 128, 209, 255), stop:1 rgba(119, 178, 205, 255));"
                                                   "border-width: 1px;"
                                                   "border-style: solid;");
        prevUpdtC.clear(); // 清空没重置的
    }
    // 用红色指示当前更新的元素
    columns[c].setStyleSheet("background-color: red;"
                             "border-color: red;"
                             "border-width: 1px;"
                             "border-style: solid;");

    // 更新元素
    columns[c].setFixedHeight(multiHigh * newHeights[c]);
    prevUpdtC.push_back(c);

    // 更新统计并显示比较和交换次数
    ui->label_oprtCnt->setNum((int)sortCtrl->getOprtCnt());
    ui->label_cmpCnt->setNum((int)sortCtrl->getCmpCnt());

    // 单步执行暂停
    if (sortCtrl->isOneStep())
        sortCtrl->pauseSorting();
}

// 高亮两个当前正在比较的柱子
void MainWindow::highLightColumns(int a, int b)
{
    if (a < 0 || a >= dataNum)
        return;
    if (b < 0 || b >= dataNum)
        return;

    qDebug() << "cmp: " << a << " " << b << " " << sortCtrl->isOneStep() << endl;

    // 重置上次比较的元素的颜色
    if (prevCmpA >= 0 && prevCmpB >= 0) // 防止将快排中第一次0位置的基准重置
    {
        columns[prevCmpA].setStyleSheet("background-color: white;"
                                        "border-color: qlineargradient(spread:pad, x1:0.767, y1:0, x2:1, y2:1, stop:0 rgba(76, 128, 209, 255), stop:1 rgba(119, 178, 205, 255));"
                                        "border-width: 1px;"
                                        "border-style: solid;");
        columns[prevCmpB].setStyleSheet("background-color: white;"
                                        "border-color: qlineargradient(spread:pad, x1:0.767, y1:0, x2:1, y2:1, stop:0 rgba(76, 128, 209, 255), stop:1 rgba(119, 178, 205, 255));"
                                        "border-width: 1px;"
                                        "border-style: solid;");
    }

    // 用天蓝色指示当前比较的元素
    columns[a].setStyleSheet("background-color: rgb(128, 245, 255);"
                             "border-color: rgb(128, 245, 255);"
                             "border-width: 1px;"
                             "border-style: solid;");
    columns[b].setStyleSheet("background-color: rgb(128, 245, 255);"
                             "border-color: rgb(128, 245, 255);"
                             "border-width: 1px;"
                             "border-style: solid;");
    prevCmpA = a, prevCmpB = b;

    // 更新显示比较次数
    ui->label_cmpCnt->setNum((int)sortCtrl->getCmpCnt());

    // 单步执行暂停
    if (sortCtrl->isOneStep())
        sortCtrl->pauseSorting();
}

// 高亮基准值
void MainWindow::highLightBase(int base)
{
    if (base < 0 || base >= dataNum)
        return;

    qDebug() << "high base: " << base << " " << prevBase << endl;

    // 重置上次基准值
    if (prevBase >= 0)
        columns[prevBase].setStyleSheet("background-color: white;"
                                        "border-color: qlineargradient(spread:pad, x1:0.767, y1:0, x2:1, y2:1, stop:0 rgba(76, 128, 209, 255), stop:1 rgba(119, 178, 205, 255));"
                                        "border-width: 1px;"
                                        "border-style: solid;");

    // 用黄绿色指示当前基准值
    columns[base].setStyleSheet("background-color: rgb(202, 255, 151);"
                                "border-color: rgb(202, 255, 151);"
                                "border-width: 1px;"
                                "border-style: solid;");

    prevBase = base;

    if (sortCtrl->isOneStep())
        sortCtrl->pauseSorting();
}

// 接受排序完成信号，执行完成动画
void MainWindow::showFinish()
{
    // 数据排序标记，先修改标记再执行动画。当用户在播放动画时点击生成数据保证新数据不会被标记已排序
    sorted = true;

    // 计时器停止
    timer->stop();

    // 允许调整窗口大小
    this->setMinimumSize(800, 600);
    this->setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));

    // 修改窗口标题
    this->setWindowTitle(QString("排序算法可视化系统"));

    // 动画效果
    for (int i = 0; i < dataNum; i++)
    {
        if (dataGenerating || sorted == false)
            break; // 用户在播放动画时点击了生成数据，保证新的未排序数据颜色正确

        columns[i].setStyleSheet("background-color: green;"
                                 "border-color: rgb(202, 255, 151);"
                                 "border-width: 1px;"
                                 "border-style: solid;");

        QEventLoop eventloop;
        int speed = sortSpeed < 5 ? 5 : sortSpeed; // 防止动画太快，最快只能间隔5ms
        if (speed > 25)
            speed = 25;                                      // 防止动画太慢，最慢只能间隔25ms
        QTimer::singleShot(speed, &eventloop, SLOT(quit())); // 每个柱子变色后暂停speed ms后再继续执行，实现逐个变绿效果
        eventloop.exec();
    }

    // 更新排序控制线程状态，休眠线程
    sortCtrl->setSortingState(false);

    // 输出排好序的数组，若有问题可看出是排序算法or交换视图出了问题
    debug_outNumber();
}

// 更新排序时间
void MainWindow::updateSortingTime()
{
    sortingTime += 0.01;
    QString timeNum = QString::number(sortingTime, 'f', 2); // 时间转化成字符串
    ui->label_sortingTime->setText(timeNum + "s");
    // qDebug() << sortingTime << endl;
}

// 状态栏显示
void MainWindow::sbShow(int num, QString text)
{
    sortName->setFixedWidth(75); // 不用显示欢迎语，缩小宽度

    if (num == 1)
    {
        sortName->setText(text);
    }

    if (num == 2)
    {
        oprtContent->setText(text);
    }

    if (num == 3)
    {
        sortDscrb->setText(text);
    }

    if (num == 4)
    {
        keyValue->setText(text);
    }
}

// 生成排序数据打乱后显示到界面
void MainWindow::dataGenerator()
{
    if (sortCtrl->isSorting())
    {
        QMessageBox::warning(this, "错误", "正在排序中！\n请在排序结束后添加数据！");
        return;
    }

    // 把之前的“柱子”全部删除
    deleteColumns();
    sorted = false;
    numbers = new int[dataNum + 10];

    // 设置排序后高度差均等
    qDebug() << "dataNum: " << dataNum << endl;
    int dif = (HLAYOUTHEIGHT - 80) / dataNum; // 数据显示区域高度,留80个单位空白
    qDebug() << "dif: " << dif << endl;

    numbers[dataNum - 1] = HLAYOUTHEIGHT - 80; // 从高到低生成数据，保证不同数据量时数据始终处于显示区域中部
    for (int i = dataNum - 2; i >= 0; i--)
    {
        numbers[i] = numbers[i + 1] - dif;
    }

    // 打乱有序数组的顺序 每个位置交换一次
    bool flag[1000]; // 每个位置交换标记
    memset(flag, 0, sizeof flag);
    for (int i = 0; i < dataNum; i++)
    {
        if (flag[i])
            continue;
        int j = qrand() % dataNum; // 随机生成位置换
        // qDebug() << i << " " << j << endl;
        int temp = numbers[i];
        numbers[i] = numbers[j];
        numbers[j] = temp;
        flag[i] = 1;
        flag[j] = 1;
    }

    // 输出检查生成的数据
    qDebug() << "生成数据如下，共" << dataNum << "个数据" << endl;
    debug_outNumber();

    // 将数据添加到视图
    addDataColumns();
}

void MainWindow::addDataColumns()
{
    // 设置“柱子”的样式并依次加入到布局中
    columns = new QLabel[dataNum + 10];
    qDebug() << "大小：" << ui->ShowSortingVisulizaion->frameGeometry().width() << " " << ui->ShowSortingVisulizaion->frameGeometry().height() << endl;

    columnWidth = (HLAYOUTWIDTH - 0.5 * dataNum) / dataNum;
    multiWidth = (double)ui->ShowSortingVisulizaion->frameGeometry().width() / HLAYOUTWIDTH; // 缩放倍数
    multiHigh = (double)ui->ShowSortingVisulizaion->frameGeometry().height() / HLAYOUTHEIGHT;
    qDebug() << "添加数据时缩放倍数 宽 高：" << multiWidth << " " << multiHigh << endl;

    for (int i = 0; i < dataNum; i++)
    {
        columns[i].setMinimumSize(columnWidth, numbers[i]);

        columns[i].setStyleSheet("background-color: white;"
                                 "border-radius: 1px;");
        ui->hLayout_sortingVisulization->addWidget(&columns[i], 0, Qt::AlignBottom); // 拉伸系数0 底部对齐
        columns[i].setVisible(true);

        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(1);
        columns[i].setSizePolicy(sizePolicy);
        columns[i].setFixedHeight(multiHigh * numbers[i]);
        columns[i].setFixedWidth(multiWidth * columnWidth);
    }
    ui->label_dataNum->setNum(dataNum); // 右上Label数据量显示
}

// 删除排序视图柱子
void MainWindow::deleteColumns()
{
    // 排序期间不能删除
    if (sortCtrl->isSorting())
        return;

    qDebug() << "deleteColumns-prevDataNum: " << prevDataNum << endl;

    for (int i = 0; i < prevDataNum; i++)
    {
        ui->hLayout_sortingVisulization->removeWidget(&columns[i]);
        columns[i].deleteLater();
    }

    delete[] numbers;
}

// 按下鼠标左键移动时输出鼠标坐标
void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    setWindowState(Qt::WindowActive);

    QPoint p_ab = event->globalPos(); // 整个桌面位置
    QPoint p_re = event->pos();       // 窗口内位置

    QString str;
    str = QString("鼠标相对窗口位置：%1 , %2").arg(p_re.x()).arg(p_re.y());
    // str = QString("鼠标相对屏幕位置：%1 , %2").arg(p_ab.x()).arg(p_ab.y());
    qDebug() << str << endl;
}

// 重载窗口大小调整事件，修改柱状图到合适高度
void MainWindow::resizeEvent(QResizeEvent *event)
{
    if (!datahaved)
        return;
    if (sortCtrl->isSorting()) // 排序过程中不调整高度
    {
        QMessageBox::information(this, "提示", "抱歉，排序过程中不可以调整大小！");
        return;
    }

    // 改变柱状图高度
    multiWidth = (double)ui->ShowSortingVisulizaion->frameGeometry().width() / HLAYOUTWIDTH; // 缩放倍数
    multiHigh = (double)ui->ShowSortingVisulizaion->frameGeometry().height() / HLAYOUTHEIGHT;
    qDebug() << "新缩放倍数 宽 高：" << multiWidth << " " << multiHigh << endl;

    for (int i = 0; i <= dataNum; i++) // 多调整一个 保险（columns多生成了10个）
    {
        columns[i].setFixedHeight(multiHigh * numbers[i]);
        columns[i].setFixedWidth(multiWidth * columnWidth);
    }
}

// 初始化变量
void MainWindow::initVar()
{
    sortingTime = 0.00;
    prevUpdtA = -1;
    prevUpdtB = -1;
    prevCmpA = -1;
    prevCmpB = -1;
    prevBase = -1;
}

// 初始化标签显示
void MainWindow::initLabel()
{
    ui->label_sortingTime->setText("0.00s");
    ui->label_cmpCnt->setNum(0);
    ui->label_oprtCnt->setNum(0);

    sbShow(1, QString("排序未开始"));
    sbShow(2, QString(""));
    sbShow(3, QString(""));
    sbShow(4, QString(""));
}

// 输出数据查看
void MainWindow::debug_outNumber()
{
    for (int i = 0; i < dataNum; i++)
        qDebug() << numbers[i] << endl;
}
