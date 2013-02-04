#ifndef TORRIFYWINDOW_H
#define TORRIFYWINDOW_H

#include <QMainWindow>
#include <QItemSelection>
#include <QtNetwork>

namespace Ui {
class TorrifyWindow;
}

class TorrifyWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit TorrifyWindow(QWidget *parent = 0);
    ~TorrifyWindow();
    
protected slots:
    void addTorInstance();
    void deleteTorInstance();
    void runTorInstance();
    void stopTorInstance();

    void browseForTorrc();
    void changeSelectedTor(const QItemSelection &, const QItemSelection &);
    void torrcLocationChanged(const QString& path);
    void torrcPortChanged(const QString &port);
    void refreshLocationInfo();
    void changeTorIdentity();

    void updateUI();

private:
    Ui::TorrifyWindow *ui;
};

#endif // TORRIFYWINDOW_H
