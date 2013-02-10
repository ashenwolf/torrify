#include "torlocationfinder.h"

#include <QtNetwork>
#include <QtXml>

class TorLocationFinderImpl: public QObject
{
    Q_OBJECT

private:
    QNetworkAccessManager   net_;
    const TorSettings*      settings_;

    QString     ip_;
    QString     country_;
    QString     geo_;
    QPixmap     map_;


public:
    TorLocationFinderImpl(const TorSettings* settings):
        settings_(settings)
    {
        connect(&net_, SIGNAL(finished(QNetworkReply*)), SLOT(onReply(QNetworkReply*)));
    }

    void checkLocation()
    {
        auto socks = settings_->attr("SocksPort", "9050");
        QString socksAddr = "localhost";
        QString socksPort;

        if (socks.contains(':'))
        {
            auto tmp = socks.split(':');
            socksAddr = tmp[0];
            socksPort = tmp[1];
        }
        else
        {
            auto host = settings_->attr("SocksListenAddress", "localhost");
            if (host.contains(':'))
                socksAddr = host.split(':')[0];
            else
                socksAddr = host;

            socksPort = socks;
        }

        QNetworkProxy proxy;
        proxy.setType(QNetworkProxy::Socks5Proxy);
        proxy.setHostName(socksAddr);
        proxy.setPort(socksPort.toInt());

        net_.setProxy(proxy);
        //net_.get(QNetworkRequest(QUrl("http://freegeoip.net/xml/")));
        net_.get(QNetworkRequest(QUrl("http://ua.smart-ip.net/geoip-xml")));
    }

protected slots:
    void onReply(QNetworkReply*);

signals:
    void locationFound(const QString&, const QString&, const QString&, const QPixmap&);
};

void TorLocationFinderImpl::onReply(QNetworkReply* reply)
{
    auto error = reply->error();

    if (error == QNetworkReply::NoError)
    {
        if (reply->url().host() == QString("maps.googleapis.com"))
        {
            const QByteArray data(reply->readAll());
            map_.loadFromData(data);
            emit locationFound(ip_, country_, geo_, map_);
        }
        else
        {
            QDomDocument doc("mydocument");
            QString s(reply->readAll());
            doc.setContent(s);
            auto ip = doc.elementsByTagName("host").at(0).toElement().text();
            if (ip.compare(ip_) != 0)
            {
                ip_ = ip;
                country_ = doc.elementsByTagName("countryCode").at(0).toElement().text();
                geo_ = doc.elementsByTagName("latitude").at(0).toElement().text() +  "," +
                               doc.elementsByTagName("longitude").at(0).toElement().text();

                net_.get(QNetworkRequest(QUrl("http://maps.googleapis.com/maps/api/staticmap?center=" + geo_ + "&zoom=3&size=480x480&sensor=false&markers=color:blue|label:E|" + geo_)));
            }
        }
    }
}

#include "torlocationfinder.moc"

/////////////////

TorLocationFinder::TorLocationFinder(const TorSettings* settings):
    impl_(new TorLocationFinderImpl(settings))
{
    connect(impl_, SIGNAL(locationFound(QString,QString,QString,QPixmap)), SLOT(onLocationFound(QString,QString,QString,QPixmap)));
}

TorLocationFinder::~TorLocationFinder() { delete impl_; }
void TorLocationFinder::checkLocation() { impl_->checkLocation(); }

void TorLocationFinder::onLocationFound(const QString& ip, const QString& country, const QString& location, const QPixmap& map)
    { emit locationFound(ip, country, location, map); }
