#ifndef TORINSTANCEMANAGER_H
#define TORINSTANCEMANAGER_H

#include "torinstancelistmodel.h"

class TorInstanceManagerImpl;

class TorInstanceManager: public ITorInstanceManager
{
private:
    TorInstanceManagerImpl* impl_;

public:
    TorInstanceManager();
    ~TorInstanceManager();

    void load();
    TorInstanceListModel* getModel();
    void add(const QString& name, const QString& path);
    void remove(const QString& name);
    int  count();
    TorInstance* at(int i);

public slots:

signals:
    void updateLocaltion();
};

#endif // TORINSTANCEMANAGER_H
