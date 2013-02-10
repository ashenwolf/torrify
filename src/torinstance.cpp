#include "torinstance.h"
#include "torsettings.h"
#include "torcontrol.h"
#include "torlocationfinder.h"

#include <QProcess>
#include <QtNetwork>
#include <QtXml>

class TorInstanceImpl: public QObject
{
    Q_OBJECT

private:
    const TorSettings*  settings_;
    TorControl          control_;
    TorLocationFinder   locationFinder_;
    QProcess            launcher_;

    QString     ip_;
    QString     country_;
    QString     location_;
    QPixmap     map_;

public:
    TorInstanceImpl(const TorSettings* settings):
      settings_(settings),
      locationFinder_(settings_),
      control_(settings_)
    {
        connect(&launcher_, SIGNAL(started()), SLOT(onStarted()));
        connect(&control_, SIGNAL(connected()), SLOT(onConnected()));
        connect(&control_, SIGNAL(newIdentity()), SLOT(onNewIdentity()));
        connect(&locationFinder_,
                SIGNAL(locationFound(QString,QString,QString,QPixmap)),
                SLOT(onLocationFound(QString,QString,QString,QPixmap)));

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
        locationFinder_.checkLocation();
    }

    // identity info
    QString ip()        { return ip_; }
    QString country()   { return country_; }
    QString location()  { return location_;  }
    QPixmap map()       { return map_; }
    QString port()      { return settings_->attr("SocksPort", "localhost:9050"); }

private slots:
    void onStarted();
    void onConnected();
    void onNewIdentity();
    void onLocationFound(const QString&, const QString&, const QString&, const QPixmap&);
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

void TorInstanceImpl::onNewIdentity()
{
    checkIdentity();
}

void TorInstanceImpl::onLocationFound(const QString& ip, const QString& country, const QString& geo, const QPixmap& map)
{
    ip_ = ip;
    country_ = country;
    location_ = geo;
    map_ = map;
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
QString TorInstance::port()     { return impl_->port(); }
