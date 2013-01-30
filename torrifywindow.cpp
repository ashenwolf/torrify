#include "torrifywindow.h"
#include "ui_torrifywindow.h"
#include "src/torinstancelistmodel.h"

#include <QFileDialog>
#include <QFileInfo>

TorrifyWindow::TorrifyWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TorrifyWindow)
{
    ui->setupUi(this);
    ui->treeView->setModel(new TorInstanceListModel());

    connect(ui->actionAdd_Tor_instance, SIGNAL(triggered()), SLOT(addTorInstance()));
    connect(ui->treeView, SIGNAL(activated(QModelIndex)), SLOT(changeSelectedTor(QModelIndex)));
    connect(ui->browseButton, SIGNAL(clicked()), SLOT(browseForTorrc()));
    connect(ui->torrcPath, SIGNAL(textChanged(QString)), SLOT(torrcLocationChanged(QString)));
}

TorrifyWindow::~TorrifyWindow()
{
    auto model = ui->treeView->model();
    delete model;
    delete ui;
}

void TorrifyWindow::addTorInstance()
{
    auto model = dynamic_cast<TorInstanceListModel*>(ui->treeView->model());
    model->AddTorInstance();
    ui->treeView->update(QModelIndex());
}

void TorrifyWindow::browseForTorrc()
{
    auto torrc = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "",
                                                    tr("Files (*.*)"));

    if (!torrc.isEmpty())
        ui->torrcPath->setText(torrc);
}

void TorrifyWindow::changeSelectedTor(const QModelIndex &index)
{
    if (index != QModelIndex() && index.parent() == QModelIndex())
    {
        auto model = dynamic_cast<TorInstanceListModel*>(ui->treeView->model());
        ui->settingsFrame->setEnabled(true);
        ui->torrcPath->setText(model->GetPath(index.row()));
    }
    else
    {
        ui->settingsFrame->setEnabled(false);
    }
}

void TorrifyWindow::torrcLocationChanged(const QString &path)
{
    auto model = dynamic_cast<TorInstanceListModel*>(ui->treeView->model());
    auto torrcPath = QFileInfo(path);
    if (torrcPath.exists())
        model->SetPath(ui->treeView->currentIndex().row(), path);
}
