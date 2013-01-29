#include "torinstancelistmodel.h"
#include <QStringList>

TorInstanceListModel::TorInstanceListModel():
    torrifySettings_(QSettings::IniFormat, QSettings::UserScope, "Skoll", "Torrify"),
    counter_(0)
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
    torrcList_.clear();

    auto groups = torrifySettings_.childGroups();
    QString group;
    foreach (group, groups)
    {
        torrcList_[group] = torrifySettings_.value("path").value<QString>();
    }
}

void TorInstanceListModel::SaveSettings()
{
    QString group;
    foreach (group, torrcList_.keys())
    {
        torrifySettings_.beginGroup(group);
        torrifySettings_.setValue("path", torrcList_[group]);
        torrifySettings_.endGroup();
    }
}

//
// public
//

QModelIndex TorInstanceListModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent == QModelIndex())
    {
        return createIndex(row, column);
    }

    return QModelIndex();
}

QModelIndex TorInstanceListModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
}


int TorInstanceListModel::rowCount(const QModelIndex &parent) const
{
    if (parent == QModelIndex())
        return torrcList_.size();

    return 0;
}

int TorInstanceListModel::columnCount(const QModelIndex &parent) const
{
    return 2;
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
    while (torrcList_.find(name) != torrcList_.end())
        name = prefix + QString::number(++counter_);

    beginInsertRows(QModelIndex(), torrcList_.count(), torrcList_.count());
    torrcList_[name] = "";
    SaveSettings();
    endInsertRows();

    emit dataChanged(QModelIndex(), QModelIndex());
}

QVariant TorInstanceListModel::data(const QModelIndex &index, int role) const
{
    if (index != QModelIndex() && index.row() < torrcList_.size())
    {
        switch(index.column())
        {
        case 0:
            if (role == Qt::DisplayRole)
                return (torrcList_.begin() + index.row()).key();
            break;
        case 1:
            if (role == Qt::DisplayRole)
                return (torrcList_.begin() + index.row()).value();
            break;

        default:
            return QVariant();
        }
    }
    return QVariant();
}

