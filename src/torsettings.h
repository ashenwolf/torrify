#ifndef TORSETTINGS_H
#define TORSETTINGS_H

#include <QString>

class TorSettingsImpl;

class TorSettings
{
private:
    TorSettingsImpl* impl_;

public:
    TorSettings(const QString& name, const QString& torrc);

    QString name() const;
    QString torrc() const;

    void load();
//    void save();

    QString attr(const QString& name) const;
//    void setAttr(const QString& name, const QString& value);
};

#endif // TORSETTINGS_H
