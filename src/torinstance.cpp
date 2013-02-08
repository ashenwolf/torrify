#include "torinstance.h"
#include "torsettings.h"
#include "torcontrol.h"

#include <QProcess>

class TorInstanceImpl;

class TorInstance: public QObject
{
    Q_OBJECT

private:
    TorSettings*    settings_;
    TorControl*     control_;
    QProcess*       launcher_;

    QString         ip_;
    QString         country_;
    QString         location_;
    QPixmap         map_;

public:
    TorInstanceImpl(const TorSettings* settings):
      settings_(settings),
      control_(new TorControl("localhost:" + settings_->attr("ControlPort"))),
      launcher_(new QProcess())
    {
        //connect(control_, SIGNAL(connected()), SLOT(onConnected()));
        control_->connectToTor();
    }

    ~TorInstanceImpl()
    {
        delete control_;
        delete launcher_;
    }

    // tor instance
    void start()            { if (!isRunning()) launcher_->start("tor.exe -f " + settings_->torrc()); }
    void stop()             { if (isRunning())  control_->shutdown(); }
    bool isRunning()        { control_->isConnected(); }

    // tor identity control
    void updateIdentity()   { if (isRunning()) control_->updateIdentity(); }
    void checkIdentity() {}

    // identity info
    QString ip()        { return ip_; }
    QString country()   { return country_; }
    QString location()  { return location_;  }
    QPixmap map()       { return map_; }

private slots:
    void onConnected()  { checkIdentity(); }
};


//////////////

TorInstance::TorInstance(const TorSettings* settings):
    impl_(new TorInstanceImpl(settings)) {}

// tor instance
void TorInstance::start()       { impl_->start(); }
void TorInstance::stop()        { impl_->stop(); }
bool TorInstance::isRunning()   { return impl_->isRunning(); }

// tor identity control
void TorInstance::updateIdentity()  { impl_->updateIdentity(); }
void TorInstance::checkIdentity()   { impl_->checkIdendity(); }

// identity info
QString TorInstance::ip()       { return impl_->ip(); }
QString TorInstance::country()  { return impl_->country(); }
QString TorInstance::location() { return impl_->location(); }
QPixmap TorInstance::map()      { return impl_->map(); }
