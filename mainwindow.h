#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QTime>
#include <QInputDialog>
#include <QMessageBox>
#include <QTimer>
#include <vector>
#include <QMouseEvent>
#include <QFontDatabase>
#include "sortAlgorithms.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    const int HLAYOUTWIDTH = 780;
    const int HLAYOUTHEIGHT = 410;
    double multiWidth = 1.0;
    double multiHigh = 1.0;
    double columnWidth;

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // 按钮
    void on_btn_dataGenerate_clicked();
    void on_btn_sortingStart_clicked();
    void on_btn_sortingPause_clicked();
    void on_btn_sortingContinue_clicked();
    void on_btn_oneStep_clicked();

    // 菜单栏
    void menuAct_actRadixSort();
    void menuAct_actQuickSort();
    void menuAct_actMergeSort();
    void menuAct_actHeapSort();
    void menuAct_actShellSort();
    void menuAct_actInsertSort();
    void menuAct_actBubbleSort();
    void menuAct_actSelectSort();
    void menuAct_newWindow();
    void menuAct_bestwin();

    // 其他
    void setSpeed(int speed);
    void sortingAlgorithm(int index);
    void updateColumns(int a, int b, int *newHeights);
    void updateOneColumn(int c, bool resetPrev, int *newHeights);
    void highLightColumns(int a, int b);
    void highLightBase(int base);
    void showFinish();
    void updateSortingTime();
    void sbShow(int num, QString text);

private:
    void initMenuBar();
    void dataGenerator();
    void addDataColumns();
    void deleteColumns();
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;
    void initVar();
    void initLabel();
    void debug_outNumber();

    // 对象
    Ui::MainWindow *ui;
    sortAlgorithms *sortCtrl;
    QLabel *columns;
    QTimer *timer;
    QLabel *sortName;
    QLabel *oprtContent;
    QLabel *sortDscrb;
    QLabel *keyValue;

    // 菜单栏
    QMenu *menu_algorithm;
    QMenu *menu_windows;
    QAction *actRadixSort;
    QAction *actQuickSort;
    QAction *actMergeSort;
    QAction *actHeapSort;
    QAction *actShellSort;
    QAction *actInsertSort;
    QAction *actBubbleSort;
    QAction *actSelectSort;
    QAction *actSortWindow;
    QAction *actBestSort;

    // 变量
    bool sorted = 0;
    bool dataGenerating = 0;
    bool dataImporting = 0;
    bool datahaved = 0;
    int algorithmSelected = 1;
    int sortSpeed = 50;
    int prevDataNum = 0;
    int dataNum;
    int *numbers;
    float sortingTime = 0.00;
    int prevUpdtA = -1, prevUpdtB = -1;
    std::vector<int> prevUpdtC;
    int prevCmpA = -1, prevCmpB = -1;
    int prevBase = -1;
};
#endif // MAINWINDOW_H
