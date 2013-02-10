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

    QMap<QString, QString>    rc_;

public:
    TorSettingsImpl(const QString& name, const QString& torrc):
        name_(name),
        torrc_(torrc)
    {
        load();
    }

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
        QRegExp rx("^\\s*(?!#)(\\w+)\\s+([^#]+)#?.*");

        while(!in.atEnd())
        {
            QString line = in.readLine();
            if (rx.indexIn(line) != -1)
            {
                auto str = rx.cap(2).simplified();
                rc_[rx.cap(1)] = str;
            }
        }

        file.close();
    }


    QString attr(const QString& name, const QString& def)
    {
        if (rc_.find(name) != rc_.end())
            return rc_[name];
        return def;
    }

    //void save();
    //void setAttr(const QString& name, const QString& value) {  }
};

///////////////

TorSettings::TorSettings(const QString& name, const QString& torrc):
    impl_(new TorSettingsImpl(name, torrc)) {}

QString TorSettings::name() const     { return impl_->name(); }
QString TorSettings::torrc() const    { return impl_->torrc(); }

void TorSettings::load()        { impl_->load(); }
//void TorSettings::save()        { impl_->save(); }

QString TorSettings::attr(const QString& name, const QString& def) const    { return impl_->attr(name, def); }
//void TorSettings::setAttr(const QString& name, const QString& value)      { impl_->setAttr(); }
