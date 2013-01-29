#include "torrifywindow.h"
#include "ui_torrifywindow.h"
#include "src/torinstancelistmodel.h"

TorrifyWindow::TorrifyWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TorrifyWindow)
{
    ui->setupUi(this);
    ui->treeView->setModel(new TorInstanceListModel());

    connect(ui->actionAdd_Tor_instance, SIGNAL(triggered()), SLOT(addTorInstance()));
}

TorrifyWindow::~TorrifyWindow()
{
    auto model = ui->treeView->model();
    delete model;
    delete ui;
}

void TorrifyWindow::addTorInstance()
{
    auto model = reinterpret_cast<TorInstanceListModel*>(ui->treeView->model());
    model->AddTorInstance();
    ui->treeView->update(QModelIndex());
}
