#include "torinstancemanager.h"

#include <QProcess>
#include <QtNetwork>
#include <QtXml>

class TorInstanceManagerImpl: public QObject
{
    Q_OBJECT

// members
private:
    QString name_;
    QString path_;
    QString port_;
    QString controlPort_;

    QString endpointIP_;
    QString endpointCountry_;
    QString endpointGeo_;

    QProcess process_;
    QNetworkProxy proxy_;
    QNetworkAccessManager net_;

// methods
private:
    void testConnection()
    {
        proxy_.setType(QNetworkProxy::Socks5Proxy);
        proxy_.setHostName("192.168.1.7");
        proxy_.setPort(port_.toInt());

        net_.setProxy(proxy_);
        net_.get(QNetworkRequest(QUrl("http://freegeoip.net/xml/")));
    }

// methods
public:
    TorInstanceManagerImpl()
    {
        connect(&net_, SIGNAL(finished(QNetworkReply*)), SLOT(onTestConnection(QNetworkReply*)));
    }

    // Saving loading
    void Serialize(QSettings &storage)
    {
        storage.beginGroup(name());
        storage.setValue("path", path());
        storage.setValue("port", port());
        storage.endGroup();
    }

    void Deserialize(QSettings &storage, const QString& group)
    {
        storage.beginGroup(group);
        setName(group);
        setPath(storage.value("path").toString());
        setPort(storage.value("port").toString());
        storage.endGroup();
    }

    void GetLocation()
    {
        testConnection();
    }

    // Properties
    QString name() { return name_; }
    void setName(const QString& name) { name_ = name; }
    QString path() { return path_; }
    void setPath(const QString& path) { path_ = path; }
    QString port() { return port_; }
    void setPort(const QString& port) { port_ = port; }

    // Tor state properties
    bool torIsRunning() { return process_.state() == QProcess::Running; }

    // Tor endpoint properties
    QString torEndpointIP() { return endpointIP_; }
    QString torEndpointCountry() { return endpointCountry_; }
    QString torEndpointLocation() { return endpointGeo_; }

    // Tor actions
    void StartTor() { process_.start("tor.exe -f " + path()); }
    void StopTor() { process_.kill(); }

private slots:
    void onTestConnection(QNetworkReply* reply);
};

#include "torinstancemanager.moc"

void TorInstanceManagerImpl::onTestConnection(QNetworkReply *reply)
{
    QDomDocument doc("mydocument");
    QString s(reply->readAll());
    doc.setContent(s);
    endpointIP_ = doc.elementsByTagName("Ip").at(0).toElement().text();
    endpointCountry_ = doc.elementsByTagName("CountryName").at(0).toElement().text();
    //emit TorInstanceManager::onLocationDetected(endpointIP_, endpointCountry_, endpointGeo_);
}


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
QString TorInstanceManager::port() { return impl_->port(); }
void TorInstanceManager::setPort(const QString& port) { impl_->setPort(port); }

bool TorInstanceManager::torIsRunning() { return impl_->torIsRunning(); }

QString TorInstanceManager::torEndpointIP() { return impl_->torEndpointIP(); }

void TorInstanceManager::StartTor() { impl_->StartTor(); }
void TorInstanceManager::StopTor() { impl_->StopTor(); }

TorInstanceManager::operator QString() { return impl_->name(); }

void TorInstanceManager::GetLocation() { impl_->GetLocation(); }
