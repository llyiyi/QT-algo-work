#ifndef SORTWINDOW_H
#define SORTWINDOW_H

#include <vector>
#include <QInputDialog>
#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QTime>
#include "sortalgotime.h"
#include "esort.h"

namespace Ui
{
    class SortWindow;
}

class SortWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SortWindow(QWidget *parent = nullptr);
    ~SortWindow();

private slots:
    void on_pushButton_clicked();

    void on_comboBox_algo_activated(int index);

    void on_pushButton_2_clicked();

    void on_pushButton_genData_clicked();

private:
    Ui::SortWindow *ui;
    QString fileName = "";     // 用于存储文件名
    int algorithmSelected = 1; // 用于存储用户选择的排序算法
    int dataNum = 0;           // 用于存储数据数量
    int *numbers;              // 用于存储数据
    bool sorted = false;       // 用于存储是否已经排序完成
    sortalgotime *sortCtrl;    // 排序控制器
    esort *esortCtrl;          // 外部排序控制器
};

#endif // SORTWINDOW_H
