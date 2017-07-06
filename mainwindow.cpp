#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <functional>


MainWindow* MainWindow::mainWindow(NULL);


MainWindow::MainWindow() :
    QMainWindow(NULL),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qApp->installEventFilter(this);
    mainWindow = this;
    startUI();
}

void MainWindow::startUI()
{

}

MainWindow::~MainWindow()
{
    delete ui;
}
bool MainWindow::eventFilter(QObject *, QEvent *e)
{
    if(e->type() == QEvent::KeyPress)
    {
   //     qWarning() << "The bad guy which steals the keyevent is" << o;
    }
    return false;
}

void MainWindow::update()
{

}


