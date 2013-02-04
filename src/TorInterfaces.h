#ifndef TORINTERFACES_H
#define TORINTERFACES_H

#include <QString>
#include <QPixmap>

struct ITorController
{
    void startTor();
    void stopTor();

    void updateIdentity();
    bool isTorRunning();
};

struct ITorInfo
{
    QString torrcPath();
    void setTorrcPath(const QString& torrc);

    QString torAddress();
    QString torPort();
    QString torControlPort();

    QString torExitIP();
    QString torExitCountry();
    QString torExitLocation();
    QPixmap torExitMap();
};

#endif // TORINTERFACES_H
