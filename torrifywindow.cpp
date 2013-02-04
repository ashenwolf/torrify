#include "torrifywindow.h"
#include "ui_torrifywindow.h"
#include "src/torinstancelistmodel.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QProcess>
#include <QTimer>

TorrifyWindow::TorrifyWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TorrifyWindow)
{
    ui->setupUi(this);
    ui->treeView->setModel(new TorInstanceListModel());

    connect(ui->actionAdd_Tor_instance, SIGNAL(triggered()), SLOT(addTorInstance()));
    connect(ui->actionDelete_Tor_instance, SIGNAL(triggered()), SLOT(deleteTorInstance()));
    connect(ui->actionRun_Tor_instance, SIGNAL(triggered()), SLOT(runTorInstance()));
    connect(ui->actionStop_Tor_instance, SIGNAL(triggered()), SLOT(stopTorInstance()));
    connect(ui->actionChange_identity, SIGNAL(triggered()), SLOT(changeTorIdentity()));
    connect(ui->actionBrowseForTorrc, SIGNAL(triggered()), SLOT(browseForTorrc()));
    connect(ui->actionRefresh, SIGNAL(triggered()), SLOT(refreshLocationInfo()));

    connect(ui->treeView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(changeSelectedTor(QItemSelection,QItemSelection)));
    connect(ui->torrcPath, SIGNAL(textChanged(QString)), SLOT(torrcLocationChanged(QString)));
    connect(ui->portEdit, SIGNAL(textChanged(QString)), SLOT(torrcPortChanged(QString)));

    ui->googleMaps->setPixmap(QPixmap("http://maps.googleapis.com/maps/api/staticmap?center=Brooklyn+Bridge,New+York,NY&zoom=13&size=600x300&maptype=roadmap&markers=color:blue%7Clabel:S%7C40.702147,-74.015794&markers=color:green%7Clabel:G%7C40.711614,-74.012318&markers=color:red%7Ccolor:red%7Clabel:C%7C40.718217,-73.998284&sensor=false"));

    QTimer::singleShot(500, this, SLOT(updateUI()));
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
    updateUI();
}

void TorrifyWindow::deleteTorInstance()
{
    auto model = dynamic_cast<TorInstanceListModel*>(ui->treeView->model());
    model->DeleteTorInstance(ui->treeView->currentIndex().row());
    updateUI();
}

void TorrifyWindow::runTorInstance()
{
    auto model = dynamic_cast<TorInstanceListModel*>(ui->treeView->model());
    model->RunTorInstance(ui->treeView->currentIndex().row());
    updateUI();
}

void TorrifyWindow::stopTorInstance()
{
    auto model = dynamic_cast<TorInstanceListModel*>(ui->treeView->model());
    model->StopTorInstance(ui->treeView->currentIndex().row());
    updateUI();
}

void TorrifyWindow::browseForTorrc()
{
    auto torrc = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "",
                                                    tr("Files (*.*)"));

    if (!torrc.isEmpty())
        ui->torrcPath->setText(torrc);
}

void TorrifyWindow::refreshLocationInfo()
{
    auto model = dynamic_cast<TorInstanceListModel*>(ui->treeView->model());
    model->TestConnection(ui->treeView->currentIndex().row());
}

void TorrifyWindow::changeSelectedTor(const QItemSelection&, const QItemSelection&)
{
    auto index = ui->treeView->currentIndex();
    if (index != QModelIndex() && index.parent() == QModelIndex())
    {
        auto model = dynamic_cast<TorInstanceListModel*>(ui->treeView->model());
        ui->torrcPath->setText(model->GetPath(index.row()));
        ui->portEdit->setText(model->GetPort(index.row()));
    }
    updateUI();
}

void TorrifyWindow::changeTorIdentity()
{
    auto index = ui->treeView->currentIndex();
    if (index != QModelIndex() && index.parent() == QModelIndex())
    {
        auto model = dynamic_cast<TorInstanceListModel*>(ui->treeView->model());
        model->changeTorIdentity(index.row());
    }
}

void TorrifyWindow::torrcLocationChanged(const QString &path)
{
    auto model = dynamic_cast<TorInstanceListModel*>(ui->treeView->model());
    auto torrcPath = QFileInfo(path);
    if (torrcPath.exists())
        model->SetPath(ui->treeView->currentIndex().row(), path);
}

void TorrifyWindow::torrcPortChanged(const QString &port)
{
    auto model = dynamic_cast<TorInstanceListModel*>(ui->treeView->model());
    if (port.toInt() > 0)
        model->SetPort(ui->treeView->currentIndex().row(), port);
}

void TorrifyWindow::updateUI()
{
    bool isRunning = false;
    bool isSelected = false;
    auto index = ui->treeView->currentIndex();
    auto model = dynamic_cast<TorInstanceListModel*>(ui->treeView->model());
    if (index != QModelIndex() && index.parent() == QModelIndex())
    {
        static QString ip;
        static QNetworkAccessManager manager;

        uint i = ui->treeView->currentIndex().row();
        isSelected = true;
        isRunning = model->TorIsRunning(i);

        ui->valueIP->setText(model->GetEndpointIP(i));
        ui->valueCountry->setText(model->GetEndpointCountry(i));
        ui->valueGeo->setText(model->GetEndpointGeo(i));

        //if (ip != model->GetEndpointIP(i))
            ui->googleMaps->setPixmap(model->GetMap(i));

        ip = model->GetEndpointIP(i);
    }

    ui->actionDelete_Tor_instance->setEnabled(isSelected);
    ui->actionRun_Tor_instance->setEnabled(!isRunning);
    ui->actionStop_Tor_instance->setEnabled(isRunning);
    ui->actionChange_identity->setEnabled(isRunning);
    ui->settingsFrame->setEnabled(isSelected);

    QTimer::singleShot(500, this, SLOT(updateUI()));
}
