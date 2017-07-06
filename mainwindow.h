#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow();
    virtual ~MainWindow();
    bool eventFilter(QObject *watched, QEvent *event);
    static inline MainWindow* getMainWindow(){return mainWindow;}
    inline Ui::MainWindow* getUI(){return ui;}
    void startUI();


private:
    Ui::MainWindow *ui;
    static MainWindow* mainWindow;

protected:
    virtual void update();
};

#endif // MAINWINDOW_H
