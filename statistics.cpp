#include "statistics.h"
#include "ui_statistics.h"

statistics::statistics(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::statistics)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("最强排序"));
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
    for (int i = 0; i < dataNum; i++) {
        numint.push_back(numbers[i]);
    }
    sorted = false;
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
    QMessageBox::information(this, tr("排序完成"), tr("排序完成！选择保存文件。"));
    fileName = QFileDialog::getSaveFileName(this, tr("保存文件"), "", tr("Text Files (*.txt)"));
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, tr("写入文件失败"), tr("无法保存文件：\n%1").arg(fileName));
        return;
    }
    QTextStream out(&file);
    for (int i = 0; i < dataNum; i++)
    out << numbers[i] << endl;
    file.close();
    ui->label->setText(tr("文件保存状态：已保存文件到 %1").arg(fileName));
}
