#ifndef TORINSTANCELISTMODEL_H
#define TORINSTANCELISTMODEL_H

#include <QAbstractItemModel>

#include <QSettings>
#include <QList>

class TorInstanceManager;

class TorInstanceListModel : public QAbstractItemModel
{
    Q_OBJECT
private:
    uint counter_;
    QSettings torrifySettings_;
    QList<TorInstanceManager*>  torInstances_;

private:
    void LoadSettings();
    void SaveSettings();
    bool isValidIndex(uint i) const;
    TorInstanceManager* torInstance(uint i);
    QStringList getKeys() const;

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
    void DeleteTorInstance(uint i);
    QString GetName(uint i);
    QString GetPath(uint i);
    QString GetPort(uint i);
    QString GetIP(uint i);
    void SetPath(uint i, QString path);
    void SetPort(uint i, QString port);

    void RunTorInstance(uint i);
    void StopTorInstance(uint i);
    void TestConnection(uint i);
};

#endif // TORINSTANCELISTMODEL_H
