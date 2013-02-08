#include "torsettings.h"

#include <QMap>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

class TorSettingsImpl
{

private:
    QString name_;
    QString torrc_;

    QMap    rc_;

public:
    TorSettingsImpl(const QString& name, const QString& torrc):
        name_(name),
        torrc_(torrc)
    {}

    QString name()  { return name_; }
    QString torrc() { return torrc_; }

    void load()
    {
        QFile file(torrc_);
        if(!file.open(QIODevice::ReadOnly))
        {
            QMessageBox::information(0, "error", file.errorString());
        }

        QTextStream in(&file);
        QRegExp rx("(?!#)(\\w+)\\s+(.+)");

        while(!in.atEnd())
        {
            QString line = in.readLine();
            if (rx.indexIn(line) != -1)
                rc_[rx.cap(1)] = rx.cap(2);
        }

        file.close();
    }


    QString attr(const QString& name)
    {
        if (rc_.find(name))
            return rc[name];
        return QString();
    }

    //void save();
    //void setAttr(const QString& name, const QString& value) {  }
};

///////////////

TorSettings::TorSettings(const QString& name, const QString& torrc):
    impl_(new TorSettingsImpl(name, torrc)) {}

QString TorSettings::name()     { return impl_->name(); }
QString TorSettings::torrc()    { return impl_->torrc(); }

void TorSettings::load()        { impl_->load(); }
//void TorSettings::save()        { impl_->save(); }

QString TorSettings::attr(const QString& name)                          { return impl_->attr(); }
//void TorSettings::setAttr(const QString& name, const QString& value)    { impl_->setAttr(); }
