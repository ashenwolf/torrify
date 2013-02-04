#include "torinstancemanager.h"

#include <QProcess>
#include <QtNetwork>
#include <QtXml>
#include <QPixmap>

#include "torcontrol.h"

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

    QPixmap googleMap_;

    TorControl tor_;

    QProcess process_;
    QNetworkAccessManager net_;

// methods
private:
    void testConnection()
    {
        QNetworkProxy proxy;
        proxy.setType(QNetworkProxy::Socks5Proxy);
        proxy.setHostName("127.0.0.1");
        proxy.setPort(port_.toInt());

        net_.setProxy(proxy);
        //net_.get(QNetworkRequest(QUrl("http://freegeoip.net/xml/")));
        net_.get(QNetworkRequest(QUrl("http://ua.smart-ip.net/geoip-xml")));
    }

// methods
public:
    TorInstanceManagerImpl():
        endpointIP_("N/A"),
        endpointCountry_("N/A"),
        endpointGeo_("N/A")
    {
        connect(&net_, SIGNAL(finished(QNetworkReply*)), SLOT(onTestConnection(QNetworkReply*)));
        connect(&tor_, SIGNAL(newIdentity()), SLOT(onNewIdentity()));
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

        tor_.connectToTor("localhost:" + QString::number(port().toInt() + 1));
    }

    void GetLocation() { testConnection(); }

    // Properties
    QString name() { return name_; }
    void setName(const QString& name) { name_ = name; }
    QString path() { return path_; }
    void setPath(const QString& path) { path_ = path; }
    QString port() { return port_; }
    void setPort(const QString& port) { port_ = port; }
    QPixmap& googleMap() { return googleMap_; }

    // Tor state properties
    //bool torIsRunning() { return process_.state() == QProcess::Running; }
    bool torIsRunning() { return tor_.isConnected(); }

    // Tor endpoint properties
    QString torEndpointIP() { return endpointIP_; }
    QString torEndpointCountry() { return endpointCountry_; }
    QString torEndpointLocation() { return endpointGeo_; }

    // Tor actions
    void StartTor() { process_.start("tor.exe -f " + path()); }
    void StopTor() { process_.kill(); }

    void newIdentity() { tor_.updateIdentity(); }

private slots:
    void onTestConnection(QNetworkReply* reply);
    void onNewIdentity();
};

#include "torinstancemanager.moc"

void TorInstanceManagerImpl::onTestConnection(QNetworkReply *reply)
{
    auto error = reply->error();

    if (error == QNetworkReply::NoError)
    {
        if (reply->url().host() == QString("maps.googleapis.com"))
        {
            const QByteArray data(reply->readAll());
            googleMap_.loadFromData(data);
        }
        else
        {
            QDomDocument doc("mydocument");
            QString s(reply->readAll());
            doc.setContent(s);
            endpointIP_ = doc.elementsByTagName("host").at(0).toElement().text();
            endpointCountry_ = doc.elementsByTagName("countryCode").at(0).toElement().text();
            endpointGeo_ = doc.elementsByTagName("latitude").at(0).toElement().text() +  "," +
                           doc.elementsByTagName("longitude").at(0).toElement().text();

            net_.get(QNetworkRequest(QUrl("http://maps.googleapis.com/maps/api/staticmap?center=" + endpointGeo_ + "&zoom=3&size=300x300&sensor=false&markers=color:blue|label:E|" + endpointGeo_)));
        }
    }
}

void TorInstanceManagerImpl::onNewIdentity()
{
    testConnection();
}

////

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

QPixmap& TorInstanceManager::googleMap() { return impl_->googleMap(); }

bool TorInstanceManager::torIsRunning() { return impl_->torIsRunning(); }

QString TorInstanceManager::torEndpointIP() { return impl_->torEndpointIP(); }
QString TorInstanceManager::torEndpointCountry() { return impl_->torEndpointCountry(); }
QString TorInstanceManager::torEndpointLocation() { return impl_->torEndpointLocation(); }

void TorInstanceManager::StartTor() { impl_->StartTor(); }
void TorInstanceManager::StopTor() { impl_->StopTor(); }

void TorInstanceManager::newIdentity() { impl_->newIdentity(); }

TorInstanceManager::operator QString() { return impl_->name(); }

void TorInstanceManager::GetLocation() { impl_->GetLocation(); }
