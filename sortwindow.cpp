#include "sortwindow.h"
#include "ui_sortwindow.h"

SortWindow::SortWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SortWindow)
{
    ui->setupUi(this);
}

SortWindow::~SortWindow()
{
    delete ui;
}
