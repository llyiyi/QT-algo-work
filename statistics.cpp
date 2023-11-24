#include "statistics.h"
#include "ui_statistics.h"
#include "sortAlgorithms.h"
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
using namespace QtCharts;

statistics::statistics(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::statistics)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("统计信息"));
    this->setWindowIcon(QIcon(":/icon.ico"));

    int fontId = QFontDatabase::addApplicationFont(QStringLiteral(":/font/hgy3.ttf"));

    QFont font;
    QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);

    font.setFamily(fontFamilies[0]);
    font.setPixelSize(12);
    ui->label->setFont(font);

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

    sortCtrl = new sortalgotime();
    this->setWindowIcon(QIcon(":/icon.ico"));
}

statistics::~statistics()
{
    delete ui;
}
// 装填按钮
void statistics::on_pushButton_clicked()
{
    fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Text Files (*.txt)"));
    if (fileName.isEmpty())
        return;
    qDebug() << fileName;
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, tr("Read File"), tr("Cannot open file:\n%1").arg(fileName));
        return;
    }
    ui->label->setText(tr("装填状态：已填入文件 %1").arg(fileName));
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
    numint.clear();
    for (int i = 0; i < dataNum; i++)
    {
        numint.push_back(numbers[i]);
    }
    sorted = false;
}

// 输出到chart
void statistics::outputChart()
{
    QBarSet *set0 = new QBarSet("耗费时间");

    // *set0 << sorttimes[0] << sorttimes[1] << sorttimes[2] << sorttimes[3] << sorttimes[4] << sorttimes[5] << sorttimes[6] << sorttimes[7] << sorttimes[8] << sorttimes[9];
    for (int i = 0; i < 9; i++)
        *set0 << sorttimes[i];

    QBarSeries *series = new QBarSeries();

    series->setLabelsPosition(QAbstractBarSeries::LabelsOutsideEnd);
    series->setLabelsVisible(true);
    series->append(set0);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("排序耗时统计");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    // 背景颜色
    chart->setBackgroundBrush(QBrush(QColor(0, 0, 0)));

    QStringList categories;
    categories << "基数排序"
               << "快速排序"
               << "归并排序"
               << "堆排序"
               << "希尔排序"
               << "插入排序"
               << "冒泡排序"
               << "选择排序"
               << "最强排序";
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(-1, std::max(sorttimes[0], std::max(sorttimes[1], std::max(sorttimes[2], std::max(sorttimes[3], std::max(sorttimes[4], std::max(sorttimes[5], std::max(sorttimes[6], std::max(sorttimes[7], std::max(sorttimes[8], sorttimes[9]))))))))));
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setParent(ui->widget);
    chartView->resize(ui->widget->size());
    chartView->show();

    // 弹出信息框
    QMessageBox::information(this, tr("统计信息"), tr("排序耗时统计已输出至图表！"));
}

// 排序按钮
void statistics::on_pushButton_2_clicked()
{
    if (dataNum == 0)
    {
        QMessageBox::warning(this, tr("警告"), tr("请先装填数据！"));
        return;
    }
    // 计时
    QTime t;
    t.start();
    intsort->sort(numint);
    sorttimes[8] = t.elapsed(); // 最强排序耗时
    sortCtrl = new sortalgotime();
    for (int i = 0; i < 8; i++)
    {
        try
        {
            int *temp = new int[dataNum + 1];
            memcpy(temp, numbers, sizeof(int) * (dataNum + 1));
            algorithmSelected = i;
            sortCtrl->setSortingState(true);
            sortCtrl->setAttribute(dataNum, temp, algorithmSelected);
            QTime t0;
            t0.start();
            sortCtrl->start();
            sortCtrl->wait();
            sorttimes[i] = t0.elapsed();
        }
        catch (std::exception &e)
        {
            // 置零
            sorttimes[i] = -1;
            continue;
        }
    }
    // 输出到chart
    statistics::outputChart();
}
