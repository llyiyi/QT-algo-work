#ifndef STATISTICS_H
#define STATISTICS_H

#include <QMainWindow>
#include <vector>
#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QTime>
#include <QFontDatabase>
#include "sortalgotime.h"
#include "bestsort.h"

namespace Ui
{
    class statistics;
}

class statistics : public QMainWindow
{
    Q_OBJECT

public:
    explicit statistics(QWidget *parent = nullptr);
    ~statistics();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void outputChart();

private:
    Ui::statistics *ui;
    QString fileName = "";
    int algorithmSelected = 0;
    int dataNum = 0;
    int sorttimes[10];
    int *numbers;
    bool sorted = false;
    std::vector<int> numint;
    Bestsort<int> *intsort;
    sortalgotime *sortCtrl;
};

#endif // STATISTICS_H
