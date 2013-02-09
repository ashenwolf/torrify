#ifndef TORLOCATIONFINDER_H
#define TORLOCATIONFINDER_H

#include "torsettings.h"

#include <QObject>
#include <QString>
#include <QPixmap>

class TorLocationFinderImpl;

class TorLocationFinder : public QObject
{
    Q_OBJECT

    TorLocationFinderImpl* impl_;
public:
    TorLocationFinder(const TorSettings*);
    ~TorLocationFinder();

public slots:
    void checkLocation();
    void onLocationFound(const QString&, const QString&, const QString&, const QPixmap&);
    
signals:
    void locationFound(const QString&, const QString&, const QString&, const QPixmap&);
    
};

#endif // TORLOCATIONFINDER_H
