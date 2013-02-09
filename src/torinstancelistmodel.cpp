#include "torinstancelistmodel.h"
#include "torinstancemanager.h"

#include <QStringList>
#include <QIcon>

TorInstanceListModel::TorInstanceListModel(ITorInstanceManager *manager):
    manager_(manager)
{}

TorInstanceListModel::~TorInstanceListModel() {}

bool TorInstanceListModel::isValidIndex(int i) const
{
    return (i >= 0 && i < manager_->count());
}

QModelIndex TorInstanceListModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent == QModelIndex() && isValidIndex(row))
        return createIndex(row, column, reinterpret_cast<void*>(manager_->at(row)));

    return QModelIndex();
}

QModelIndex TorInstanceListModel::parent(const QModelIndex &) const
{
    return QModelIndex();
}


int TorInstanceListModel::rowCount(const QModelIndex &parent) const
{
    if (parent == QModelIndex())
        return manager_->count();

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
                return QString("Tor Name");
            case 1:
                return QString("Tor Config File Location");
            }
        }
    }
    return QVariant();
}

QVariant TorInstanceListModel::data(const QModelIndex &index, int role) const
{
    if (index != QModelIndex() && isValidIndex(index.row()))
    {
        auto tor = reinterpret_cast<TorInstance*>(index.internalPointer());
        switch(index.column())
        {
        case 0:
            if (role == Qt::DisplayRole)
                return tor->settings()->name();
            else if (role == Qt::DecorationRole)
                return QVariant::fromValue(QIcon(tor->isRunning()
                        ? QString(":/images/icons/run.png")
                        : QString(":/images/icons/stop.png")));
            break;
        case 1:
            if (role == Qt::DisplayRole)
                return tor->settings()->torrc();
            break;

        default:
            return QVariant();
        }
    }
    return QVariant();
}

void TorInstanceListModel::add()
{
    beginInsertRows(QModelIndex(), manager_->count(), manager_->count());
    endInsertRows();
}

void TorInstanceListModel::remove(int i)
{
    beginRemoveRows(QModelIndex(), i, i);
    endRemoveRows();
}
