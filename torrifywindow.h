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

private:
    Ui::TorrifyWindow *ui;
};

#endif // TORRIFYWINDOW_H
