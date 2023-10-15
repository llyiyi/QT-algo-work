#ifndef BESTSORTWIN_H
#define BESTSORTWIN_H

#include <QMainWindow>
#include <vector>
#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QTime>
#include <QFontDatabase>
#include "bestsort.h"

namespace Ui {
class bestSortWin;
}

class bestSortWin : public QMainWindow
{
    Q_OBJECT

public:
    explicit bestSortWin(QWidget *parent = nullptr);
    ~bestSortWin();



private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();


    void on_comboBox_activated(int index);

private:
    Ui::bestSortWin *ui;
    QString fileName = "";
    int dataNum = 0;
    int type=0;
    bool sorted = false;
    std::vector<int> numint;
    std::vector<float> numfloat;
    std::vector<double> numdouble;
    Bestsort<int> *intsort;
    Bestsort<float> *floatsort;
    Bestsort<double> *doublesort;
};

#endif // BESTSORTWIN_H
