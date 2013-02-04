#include "torcontrol.h"

#include <QTcpSocket>
#include <QStringList>

class TorControlImpl: public QObject
{
    Q_OBJECT

private:
    QTcpSocket sock_;
    quint16 blockSize;

private:
    void sendCommand(const QString& command)
    {
        sock_.write(command.toLatin1(), command.size());
    }

signals:
    void newIdentity();

public slots:
    void authenticate();
    void readData();
    void updateIdentity();

public:
    TorControlImpl()
    {
        connect(&sock_, SIGNAL(connected()), SLOT(authenticate()));
        connect(&sock_, SIGNAL(readyRead()), SLOT(readData()));
    }

    void connectToTor(const QString& tor)
    {
        auto host = tor.split(":");
        sock_.connectToHost(host[0], host[1].toInt());
    }

    bool isConnected() { return sock_.state() == QAbstractSocket::ConnectedState; }
};

#include "torcontrol.moc"

void TorControlImpl::authenticate()
{
    sendCommand("authenticate \"\"\n");
}

void TorControlImpl::updateIdentity()
{
    sendCommand("signal newnym\n");
    emit newIdentity();
}

void TorControlImpl::readData()
{
    QDataStream in(&sock_);
    in.setVersion(QDataStream::Qt_4_0);

    if (blockSize == 0) {
        if (sock_.bytesAvailable() < (int)sizeof(quint16))
            return;

        in >> blockSize;
    }

    if (sock_.bytesAvailable() < blockSize)
        return;
}

////

TorControl::TorControl() {
    impl_ = new TorControlImpl();
    connect(impl_, SIGNAL(newIdentity()), SLOT(onNewIdentity()));
}

void TorControl::connectToTor(const QString& tor) { return impl_->connectToTor(tor); }
bool TorControl::isConnected() { return impl_->isConnected(); }
void TorControl::updateIdentity() { impl_->updateIdentity(); }
void TorControl::onNewIdentity() { emit newIdentity(); }
