#include "torinstancelistmodel.h"
#include "torinstancemanager.h"

#include <QStringList>
#include <QIcon>

TorInstanceListModel::TorInstanceListModel():
    torrifySettings_(QSettings::IniFormat, QSettings::UserScope, "Skoll", "Torrify"),
    counter_(1)
{
    LoadSettings();
}

TorInstanceListModel::~TorInstanceListModel()
{
    SaveSettings();
}

//
// private
//

void TorInstanceListModel::LoadSettings()
{
    foreach (QString group, torrifySettings_.childGroups())
    {
        torInstances_.append(new TorInstanceManager());
        torInstances_.last()->Deserialize(torrifySettings_, group);
        torInstances_.last()->GetLocation();
    }
}

void TorInstanceListModel::SaveSettings()
{
    foreach (TorInstanceManager* torInstance, torInstances_)
    {
        torInstance->Serialize(torrifySettings_);
    }
}

bool TorInstanceListModel::isValidIndex(uint i) const
{
    return (i >= 0 && i < torInstances_.size());
}

TorInstanceManager *TorInstanceListModel::torInstance(uint i)
{
    if (isValidIndex(i))
        return torInstances_.at(i);
    return NULL;
}

QStringList TorInstanceListModel::getKeys() const
{
    QStringList result;
    foreach (auto tor, torInstances_)
        result.append(tor->name());
    return result;
}

//
// public
//

QModelIndex TorInstanceListModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent == QModelIndex())
        return createIndex(row, column);

    return QModelIndex();
}

QModelIndex TorInstanceListModel::parent(const QModelIndex &) const
{
    return QModelIndex();
}


int TorInstanceListModel::rowCount(const QModelIndex &parent) const
{
    if (parent == QModelIndex())
        return torInstances_.size();

    return 0;
}

int TorInstanceListModel::columnCount(const QModelIndex &parent) const
{
    if (parent == QModelIndex())
        return 2;

    return 0;
}

QVariant TorInstanceListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal) {
            switch (section)
            {
            case 0:
                return QString("Tor");
            case 1:
                return QString("Config");
            }
        }
    }
    return QVariant();
}

void TorInstanceListModel::AddTorInstance()
{
    auto prefix = QString("Tor ");

    auto name = prefix + QString::number(counter_);
    while (getKeys().contains(name, Qt::CaseInsensitive))
        name = prefix + QString::number(++counter_);

    beginInsertRows(QModelIndex(), torInstances_.size(), torInstances_.size());
    torInstances_.append(new TorInstanceManager());
    torInstances_.last()->setName(name);
    //connect(torInstances_.last(), SIGNAL(onLocationDetected(QString,QString,QString)), SLOT(LocationDetected(QString,QString,QString)));
    endInsertRows();

    emit dataChanged(QModelIndex(), QModelIndex());
}

void TorInstanceListModel::DeleteTorInstance(uint i)
{
    if (isValidIndex(i))
    {
        beginRemoveRows(QModelIndex(), i, i);
        auto tor = (torInstances_.begin() + i);
        torInstances_.erase(tor);
        torrifySettings_.remove((*tor)->name());
        delete *tor;
        endRemoveRows();
    }
}

QString TorInstanceListModel::GetName(uint i)
{
    if (isValidIndex(i))
        return torInstances_[i]->name();
    return QString();
}

QString TorInstanceListModel::GetPath(uint i)
{
    if (isValidIndex(i))
        return torInstances_[i]->path();
    return QString();
}

QString TorInstanceListModel::GetPort(uint i)
{
    if (isValidIndex(i))
        return torInstances_[i]->port();
    return QString();
}

QString TorInstanceListModel::GetIP(uint i)
{
    if (isValidIndex(i))
        return torInstances_[i]->torEndpointIP();
    return QString();
}

void TorInstanceListModel::SetPath(uint i, QString path)
{
    if (isValidIndex(i))
    {
        torInstances_[i]->setPath(path);
        emit dataChanged(QModelIndex(), QModelIndex());
    }
}

void TorInstanceListModel::SetPort(uint i, QString port)
{
    if (isValidIndex(i))
    {
        torInstances_[i]->setPort(port);
        emit dataChanged(QModelIndex(), QModelIndex());
    }
}

void TorInstanceListModel::TestConnection(uint i)
{
    if (isValidIndex(i))
    {
        torInstances_[i]->GetLocation();
    }
}

void TorInstanceListModel::RunTorInstance(uint i)
{
    if (isValidIndex(i))
    {
        torInstances_[i]->StartTor();
        emit dataChanged(QModelIndex(), QModelIndex());
    }
}

void TorInstanceListModel::StopTorInstance(uint i)
{
    if (isValidIndex(i))
    {
        torInstances_[i]->StopTor();
        emit dataChanged(QModelIndex(), QModelIndex());
    }
}

QVariant TorInstanceListModel::data(const QModelIndex &index, int role) const
{
    if (index != QModelIndex() && isValidIndex(index.row()))
    {
        switch(index.column())
        {
        case 0:
            if (role == Qt::DisplayRole)
                return torInstances_[index.row()]->name();
            else if (role == Qt::DecorationRole)
                return QVariant::fromValue(QIcon(torInstances_[index.row()]->torIsRunning()
                        ? QString(":/images/icons/run.png")
                        : QString(":/images/icons/stop.png")));
            break;
        case 1:
            if (role == Qt::DisplayRole)
                return torInstances_[index.row()]->path();
            break;

        default:
            return QVariant();
        }
    }
    return QVariant();
}
