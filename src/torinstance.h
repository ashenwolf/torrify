#ifndef TORINSTANCE_H
#define TORINSTANCE_H

#include <QObject>

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

    // tor identity control
    void updateIdentity();
    void checkIdentity();

    // identity info
    QString ip();
    QString country();
    QString location();
    QPixmap map();
};

#endif // TORINSTANCE_H
