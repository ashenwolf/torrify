#include "torinstancemanager.h"

#include <QProcess>

class TorInstanceManagerImpl
{
// members
private:
    QString name_;
    QString path_;

    QProcess process_;

// methods
private:

// methods
public:
    // Saving loading
    void Serialize(QSettings &storage)
    {
        storage.beginGroup(name());
        storage.setValue("path", path());
        storage.endGroup();
    }

    void Deserialize(QSettings &storage, const QString& group)
    {
        storage.beginGroup(group);
        setName(group);
        setPath(storage.value("path").toString());
        storage.endGroup();
    }

    // Properties
    QString name() { return name_; }
    void setName(const QString& name) { name_ = name; }
    QString path() { return path_; }
    void setPath(const QString& path) { path_ = path; }

    // Tor state properties
    bool torIsRunning() { return process_.state() == QProcess::Running; }

    // Tor endpoint properties
    QString torEndpointIP() { return "127.0.0.1"; }
    QString torEndpointCountry() { return "UA"; }
    QString torEndpointLocation() { return "10.5, 11.6"; }

    // Tor actions
    void StartTor() { process_.start("tor.exe -f " + path()); }
    void StopTor() { process_.kill(); }

};

TorInstanceManager::TorInstanceManager()
{
    impl_ = new TorInstanceManagerImpl();
}

TorInstanceManager::~TorInstanceManager()
{
    delete impl_;
}

//
// Methods
//
void TorInstanceManager::Serialize(QSettings &storage) { impl_->Serialize(storage); }
void TorInstanceManager::Deserialize(QSettings &storage, const QString& group) { impl_->Deserialize(storage, group); }

QString TorInstanceManager::name() { return impl_->name(); }
void TorInstanceManager::setName(const QString& name) { impl_->setName(name); }

QString TorInstanceManager::path() { return impl_->path(); }
void TorInstanceManager::setPath(const QString& path) { impl_->setPath(path); }

bool TorInstanceManager::torIsRunning() { return impl_->torIsRunning(); }
void TorInstanceManager::StartTor() { impl_->StartTor(); }
void TorInstanceManager::StopTor() { impl_->StopTor(); }

TorInstanceManager::operator QString() { return impl_->name(); }
