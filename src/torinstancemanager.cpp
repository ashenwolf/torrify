#include "torinstancemanager.h"
#include "torinstance.h"

#include <QStringList>
#include <QSettings>

class TorInstanceManagerImpl: public QObject
{
    Q_OBJECT

private:
    QSettings               settings_;
    QList<TorInstance*>     instances_;
    TorInstanceListModel*   model_;

public:
    TorInstanceManagerImpl(ITorInstanceManager* parent):
        settings_(QSettings::IniFormat, QSettings::UserScope, "Skoll", "Torrify"),
        model_(new TorInstanceListModel(parent))
    {}

    ~TorInstanceManagerImpl()
    {
        delete model_;
    }

    void load()
    {
        foreach (QString name, settings_.childKeys())
        {
            add(name, settings_.value(name).toString(), false);
        }
    }

    void add(const QString& name, const QString& path, bool isUserAdd = true)
    {
        auto torSettings = new TorSettings(name, path);
        auto torInstance = new TorInstance(torSettings);
        instances_.push_back(torInstance);
        if (isUserAdd)
            settings_.setValue(name, path);
        model_->add();
    }

    void remove(const QString& name)
    {
        for (auto tor = instances_.begin(); tor != instances_.end(); tor++)
        {
            if ((*tor)->settings()->name().compare(name) == 0)
            {
                int i = tor - instances_.begin();
                instances_.erase(tor);
                settings_.remove(name);
                model_->remove(i);
                return;
            }
        }
    }
    int count() { return instances_.size(); }

    TorInstance* at(int i)
    {
        if (i >= 0 && i < count())
            return *(instances_.begin() + i);
        return NULL;
    }

    TorInstanceListModel* getModel() { return model_; }
};

#include "torinstancemanager.moc"

///////////

TorInstanceManager::TorInstanceManager():
    impl_(new TorInstanceManagerImpl(this)) {}
TorInstanceManager::~TorInstanceManager()   { delete impl_; }

void TorInstanceManager::load()                                         { impl_->load(); }

void TorInstanceManager::add(const QString& name, const QString& path)  { impl_->add(name, path); }
void TorInstanceManager::remove(const QString& name)                    { impl_->remove(name); }
int TorInstanceManager::count()                                         { return impl_->count(); }
TorInstance *TorInstanceManager::at(int i)                              { return impl_->at(i); }

TorInstanceListModel* TorInstanceManager::getModel()                    { return impl_->getModel(); }
