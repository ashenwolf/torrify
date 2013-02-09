#include "torcontrol.h"

#include <QTcpSocket>
#include <QStringList>

class TorControlImpl: public QObject
{
    Q_OBJECT

private:
    QStringList host_;
    QTcpSocket  sock_;
    quint16 blockSize;

private:
    void sendCommand(const QString& command)
    {
        sock_.write(command.toLatin1(), command.size());
    }

signals:
    void connected();
    void newIdentity();

public slots:
    void authenticate();
    void readData();
    void updateIdentity();
    void shutdown();

public:
    TorControlImpl(const QString& address)
    {
        host_ = address.split(":");

        connect(&sock_, SIGNAL(connected()), SLOT(authenticate()));
        //connect(&sock_, SIGNAL(disconnected()), SLOT(authenticate()));
        connect(&sock_, SIGNAL(readyRead()), SLOT(readData()));
    }

    void connectToTor()
    {
        if (isConnected())
            sock_.disconnectFromHost();
        sock_.connectToHost(host_[0], host_[1].toInt());
    }

    bool isConnected() { return sock_.state() == QAbstractSocket::ConnectedState; }
};

#include "torcontrol.moc"

void TorControlImpl::authenticate()
{
    sendCommand("authenticate \"\"\n");
    emit connected();
}

void TorControlImpl::updateIdentity()
{
    sendCommand("signal newnym\n");
    emit newIdentity();
}

void TorControlImpl::shutdown()
{
    sendCommand("signal shutdown\n");
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

/////////

TorControl::TorControl(const QString& address):
    impl_(new TorControlImpl(address))
{
    connect(impl_, SIGNAL(connected()), SLOT(onConnected()));
    connect(impl_, SIGNAL(newIdentity()), SLOT(onNewIdentity()));
}

void TorControl::connectToTor()   { return impl_->connectToTor(); }
bool TorControl::isConnected()    { return impl_->isConnected(); }
void TorControl::updateIdentity() { impl_->updateIdentity(); }
void TorControl::shutdown()       { impl_->shutdown(); }

void TorControl::onNewIdentity()  { emit newIdentity(); }
void TorControl::onConnected()    { emit connected(); }
