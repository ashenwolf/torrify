#ifndef TORINSTANCEMANAGER_H
#define TORINSTANCEMANAGER_H

#include <QSettings>

class TorInstanceManagerImpl;

class TorInstanceManager
{
private:
    TorInstanceManagerImpl* impl_;

public:
    TorInstanceManager();
    ~TorInstanceManager();

    // Saving loading
    void Serialize(QSettings &storage);
    void Deserialize(QSettings &storage, const QString& group);

    // Properties
    QString name();
    void setName(const QString&);
    QString path();
    void setPath(const QString&);

    // Tor state properties
    bool torIsRunning();

    // Tor endpoint properties
    QString torEndpointIP();
    QString torEndpointCountry();
    QString torEndpointLocation();

    // Tor actions
    void StartTor();
    void StopTor();

    // Casts
    operator QString();
};

#endif // TORINSTANCEMANAGER_H
