#ifndef TORRIFYWINDOW_H
#define TORRIFYWINDOW_H

#include <QMainWindow>

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
    void changeSelectedTor(const QModelIndex & index);
    void torrcLocationChanged(const QString& path);

private:
    Ui::TorrifyWindow *ui;
};

#endif // TORRIFYWINDOW_H
