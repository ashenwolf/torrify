#ifndef TORINSTANCELISTMODEL_H
#define TORINSTANCELISTMODEL_H

#include "torinstance.h"

#include <QAbstractItemModel>

#include <QSettings>
#include <QList>

struct ITorInstanceManager
{
    virtual void         add(const QString& name, const QString& path) = 0;
    virtual void         remove(const QString& name) = 0;
    virtual int          count() = 0;
    virtual TorInstance* at(int i) = 0;
};

class TorInstanceListModel : public QAbstractItemModel
{
    Q_OBJECT
private:
    ITorInstanceManager* manager_;

private:
    bool isValidIndex(int i) const;

public:
    TorInstanceListModel(ITorInstanceManager* manager);
    ~TorInstanceListModel();

    // model
    QModelIndex index ( int row, int column, const QModelIndex & parent = QModelIndex() ) const;
    QModelIndex parent ( const QModelIndex & index ) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    void add();
    void remove(int i);
};

#endif // TORINSTANCELISTMODEL_H
