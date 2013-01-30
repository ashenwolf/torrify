#ifndef TORINSTANCELISTMODEL_H
#define TORINSTANCELISTMODEL_H

#include <QAbstractItemModel>

#include <QSettings>
#include <QMap>

class TorInstanceListModel : public QAbstractItemModel
{
    Q_OBJECT
private:
    uint counter_;
    QSettings torrifySettings_;
    QMap<QString, QString>  torrcList_;

private:
    void LoadSettings();
    void SaveSettings();

public:
    TorInstanceListModel();
    ~TorInstanceListModel();

    // model
    QModelIndex index ( int row, int column, const QModelIndex & parent = QModelIndex() ) const;
    QModelIndex parent ( const QModelIndex & index ) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    void AddTorInstance();
    QString GetPath(uint i);
    void SetPath(uint i, QString path);
};

#endif // TORINSTANCELISTMODEL_H
