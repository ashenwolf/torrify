#ifndef TORINSTANCE_H
#define TORINSTANCE_H

#include "torsettings.h"
#include <QObject>
#include <QPixmap>

class TorInstanceImpl;

class TorInstance: public QObject
{
    Q_OBJECT

private:
    TorInstanceImpl* impl_;

public:
    TorInstance(const TorSettings* settings);

public:
    // tor instance
    void start();
    void stop();
    bool isRunning();

    // settings
    const TorSettings* settings();

    // tor identity control
    void updateIdentity();
    void checkIdentity();

    // identity info
    QString ip();
    QString country();
    QString location();
    QPixmap map();
    QString port();
};

#endif // TORINSTANCE_H
