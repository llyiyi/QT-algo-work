#include "bestsortwin.h"
#include "ui_bestsortwin.h"

bestSortWin::bestSortWin(QWidget *parent) : QMainWindow(parent),
                                            ui(new Ui::bestSortWin)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("最强排序"));
    this->setWindowIcon(QIcon(":/icon.ico"));

    int fontId = QFontDatabase::addApplicationFont(QStringLiteral(":/font/hgy3.ttf"));

    QFont font;
    QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);

    font.setFamily(fontFamilies[0]);
    font.setPixelSize(12);
    ui->label1->setFont(font);
    font.setPixelSize(20);
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

    ui->comboBox->addItem(tr("int"));
    ui->comboBox->addItem(tr("float"));
    ui->comboBox->addItem(tr("double"));

    intsort = new Bestsort<int>;
    floatsort = new Bestsort<float>;
    doublesort = new Bestsort<double>;

    this->setWindowIcon(QIcon(":/icon.ico"));
}
bestSortWin::~bestSortWin()
{
    delete ui;
}

void bestSortWin::on_comboBox_activated(int index)
{
    type = index;
}

// 装填按钮
void bestSortWin::on_pushButton_clicked()
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
    ui->label1->setText(tr("装填状态：已填入文件 %1").arg(fileName));
    QTextStream in(&file);
    QString line;
    switch (type)
    {
    case 0:
    {
        dataNum = 0;
        numint.clear();
        while (!in.atEnd())
        {
            line = in.readLine();
            numint.push_back(line.toInt());
            dataNum++;
        }
        file.close();
        sorted = false;
        break;
    }
    case 1:
    {
        dataNum = 0;
        numfloat.clear();
        while (!in.atEnd())
        {
            line = in.readLine();
            numfloat.push_back(line.toFloat());
            dataNum++;
        }
        file.close();
        sorted = false;
        break;
    }
    case 2:
    {
        dataNum = 0;
        numdouble.clear();
        while (!in.atEnd())
        {
            line = in.readLine();
            numdouble.push_back(line.toDouble());
            dataNum++;
        }
        file.close();
        sorted = false;
        break;
    }
    }
}

// 排序按钮
void bestSortWin::on_pushButton_2_clicked()
{
    if (dataNum == 0)
    {
        QMessageBox::warning(this, tr("警告"), tr("请先装填数据！"));
        return;
    }
    // 计时
    QTime t;
    t.start();

    switch (type)
    {
    case 0:
        intsort->sort(numint);
        break;
    case 1:
        floatsort->sort(numfloat);
        break;
    case 2:
        doublesort->sort(numdouble);
        break;
    }
    qDebug() << "排序用时：" << t.elapsed() << "ms" << endl;
    ui->label->setText(tr("耗时：%1ms").arg(t.elapsed()));
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
    if (ui->checkBox->isChecked())
        for (int i = dataNum - 1; i >= 0; i--)
            switch (type)
            {
            case 0:
                out << numint[i] << endl;
                break;
            case 1:
                out << QString::number(numfloat[i], 'f', 8) << endl;
                break;
            case 2:
                out << QString::number(numdouble[i], 'f', 12) << endl;
                break;
            }
    else
        for (int i = 0; i < dataNum; i++)
            switch (type)
            {
            case 0:
                out << numint[i] << endl;
                break;
            case 1:
                out << QString::number(numfloat[i], 'f', 8) << endl;
                break;
            case 2:
                out << QString::number(numdouble[i], 'f', 12) << endl;
                break;
            }
    file.close();
    ui->label1->setText(tr("文件保存状态：已保存文件到 %1").arg(fileName));
}
