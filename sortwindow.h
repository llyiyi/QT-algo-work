#ifndef SORTWINDOW_H
#define SORTWINDOW_H

#include <QMainWindow>

namespace Ui {
class SortWindow;
}

class SortWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SortWindow(QWidget *parent = nullptr);
    ~SortWindow();

private:
    Ui::SortWindow *ui;
};

#endif // SORTWINDOW_H
