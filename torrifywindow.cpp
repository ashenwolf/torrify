#include "torrifywindow.h"
#include "ui_torrifywindow.h"

TorrifyWindow::TorrifyWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TorrifyWindow)
{
    ui->setupUi(this);
}

TorrifyWindow::~TorrifyWindow()
{
    delete ui;
}
