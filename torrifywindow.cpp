#include "torrifywindow.h"
#include "ui_torrifywindow.h"
#include "src/torinstancemanager.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QProcess>
#include <QTimer>

TorInstanceManager manager;
QTimer             timer;

TorrifyWindow::TorrifyWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TorrifyWindow)
{
    ui->setupUi(this);
    ui->treeView->setModel(manager.getModel());

    connect(ui->actionAdd_Tor_instance, SIGNAL(triggered()), SLOT(addTorInstance()));
    connect(ui->actionDelete_Tor_instance, SIGNAL(triggered()), SLOT(deleteTorInstance()));
    connect(ui->actionRun_Tor_instance, SIGNAL(triggered()), SLOT(runTorInstance()));
    connect(ui->actionStop_Tor_instance, SIGNAL(triggered()), SLOT(stopTorInstance()));
    connect(ui->actionChange_identity, SIGNAL(triggered()), SLOT(changeTorIdentity()));

    connect(ui->treeView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(changeSelectedTor(QItemSelection,QItemSelection)));

    connect(&timer, SIGNAL(timeout()), SLOT(updateUI()));

    manager.load();
    timer.setInterval(1000);
    timer.start(1000);
}

TorrifyWindow::~TorrifyWindow()
{
    timer.stop();
    delete ui;
}

void TorrifyWindow::addTorInstance()
{
    auto torrc = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "",
                                                    tr("Files (*.*)"));

    if (!torrc.isNull())
    {
        QFileInfo info(torrc);
        manager.add(info.fileName(), torrc);
        updateUI();
    }
}

void TorrifyWindow::deleteTorInstance()
{
    auto index = ui->treeView->currentIndex();
    if (index != QModelIndex() && index.parent() == QModelIndex())
    {
        auto tor = reinterpret_cast<TorInstance*>(ui->treeView->currentIndex().internalPointer());
        if (tor)
        {
            manager.remove(tor->settings()->name());
            updateUI();
        }
    }
}

void TorrifyWindow::runTorInstance()
{
    auto index = ui->treeView->currentIndex();
    if (index != QModelIndex() && index.parent() == QModelIndex())
    {
        auto tor = reinterpret_cast<TorInstance*>(ui->treeView->currentIndex().internalPointer());
        if (tor)
            tor->start();
        updateUI();
    }
}

void TorrifyWindow::stopTorInstance()
{
    auto index = ui->treeView->currentIndex();
    if (index != QModelIndex() && index.parent() == QModelIndex())
    {
        auto tor = reinterpret_cast<TorInstance*>(ui->treeView->currentIndex().internalPointer());
        if (tor)
            tor->stop();
        updateUI();
    }

}

void TorrifyWindow::changeTorIdentity()
{
    auto index = ui->treeView->currentIndex();
    if (index != QModelIndex() && index.parent() == QModelIndex())
    {
        auto tor = reinterpret_cast<TorInstance*>(ui->treeView->currentIndex().internalPointer());
        if (tor)
            tor->updateIdentity();
        updateUI();
    }
}

void TorrifyWindow::changeSelectedTor(const QItemSelection&, const QItemSelection&)
{
    updateUI();
}

void TorrifyWindow::updateUI()
{
    bool isRunning = false;
    bool isSelected = false;
    auto index = ui->treeView->currentIndex();
    if (index != QModelIndex() && index.parent() == QModelIndex())
    {
        auto tor = reinterpret_cast<TorInstance*>(ui->treeView->currentIndex().internalPointer());
        if (tor)
        {
            isSelected = true;
            isRunning = tor->isRunning();

            ui->valueIP->setText(tor->ip());
            ui->valueCountry->setText(tor->country());
            ui->valueGeo->setText(tor->location());
            ui->googleMaps->setPixmap(tor->map());
        }
    }
    else
    {
        ui->valueIP->setText("N/A");
        ui->valueCountry->setText("N/A");
        ui->valueGeo->setText("N/A");
        ui->googleMaps->setPixmap(QPixmap());
    }

    ui->treeView->dataChanged(QModelIndex(), QModelIndex());

    ui->actionDelete_Tor_instance->setEnabled(isSelected);
    ui->actionRun_Tor_instance->setEnabled(!isRunning && isSelected);
    ui->actionStop_Tor_instance->setEnabled(isRunning && isSelected);
    ui->actionChange_identity->setEnabled(isRunning && isSelected);
    ui->settingsFrame->setEnabled(isSelected);
}
