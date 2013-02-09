#ifndef TORCONTROL_H
#define TORCONTROL_H

#include <QObject>

class TorControlImpl;

class TorControl: public QObject
{
    Q_OBJECT

private:
    TorControlImpl* impl_;

public:
    TorControl(const QString &address);

    void connectToTor();
    bool isConnected();
    void updateIdentity();
    void shutdown();

private slots:
    void onConnected();
    void onNewIdentity();

signals:
    void connected();
    void newIdentity();
};

#endif // TORCONTROL_H
