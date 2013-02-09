#include "torinstance.h"
#include "torsettings.h"
#include "torcontrol.h"

#include <QProcess>
#include <QtNetwork>
#include <QtXml>

class TorInstanceImpl: public QObject
{
    Q_OBJECT

private:
    const TorSettings*    settings_;
    TorControl      control_;
    QProcess        launcher_;
    QNetworkAccessManager net_;

    QString         ip_;
    QString         country_;
    QString         location_;
    QPixmap         map_;

public:
    TorInstanceImpl(const TorSettings* settings):
      settings_(settings),
      control_("localhost:" + settings_->attr("ControlPort"))
    {
//        connect(&net_, SIGNAL(finished(QNetworkReply*)), SLOT(onReply(QNetworkReply*)));
//        connect(&tor_, SIGNAL(newIdentity()), SLOT(onNewIdentity()));
        connect(&launcher_, SIGNAL(started()), SLOT(onStarted()));
        connect(&control_, SIGNAL(connected()), SLOT(onConnected()));

        control_.connectToTor();
    }

    ~TorInstanceImpl()
    {
        delete settings_;
    }

    // tor instance
    void start()            { if (!isRunning()) launcher_.start("tor.exe -f " + settings_->torrc()); }
    void stop()             { if (isRunning())  control_.shutdown(); }
    bool isRunning()        { return control_.isConnected(); }

    const TorSettings* settings() { return settings_; }

    // tor identity control
    void updateIdentity()   { if (isRunning()) control_.updateIdentity(); }
    void checkIdentity()
    {
/*        QNetworkProxy proxy;
        proxy.setType(QNetworkProxy::Socks5Proxy);
        proxy.setHostName("127.0.0.1");
        proxy.setPort(port_.toInt());

        net_.setProxy(proxy);
        //net_.get(QNetworkRequest(QUrl("http://freegeoip.net/xml/")));
        net_.get(QNetworkRequest(QUrl("http://ua.smart-ip.net/geoip-xml")));*/
    }
/*
    void onReply(QNetworkReply *reply)
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
*/
    // identity info
    QString ip()        { return ip_; }
    QString country()   { return country_; }
    QString location()  { return location_;  }
    QPixmap map()       { return map_; }

private slots:
    void onStarted();
    void onConnected();
};

#include "torinstance.moc"

void TorInstanceImpl::onStarted()
{
    control_.connectToTor();
}

void TorInstanceImpl::onConnected()
{
    checkIdentity();
}

//////////////

TorInstance::TorInstance(const TorSettings* settings):
    impl_(new TorInstanceImpl(settings)) {}

// tor instance
void TorInstance::start()       { impl_->start(); }
void TorInstance::stop()        { impl_->stop(); }
bool TorInstance::isRunning()   { return impl_->isRunning(); }

const TorSettings *TorInstance::settings() { return impl_->settings(); }

// tor identity control
void TorInstance::updateIdentity()  { impl_->updateIdentity(); }
void TorInstance::checkIdentity()   { impl_->checkIdentity(); }

// identity info
QString TorInstance::ip()       { return impl_->ip(); }
QString TorInstance::country()  { return impl_->country(); }
QString TorInstance::location() { return impl_->location(); }
QPixmap TorInstance::map()      { return impl_->map(); }
