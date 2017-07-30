#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <functional>
#include "mappergfx/provincesmask.h"
#include <QColor>


using namespace mappergfx;
using namespace mapreader;
MainWindow* MainWindow::mainWindow(NULL);


MainWindow::MainWindow() :
    QMainWindow(NULL),
    ui(new Ui::MainWindow),
	map("input.png"),
	graphic(NULL)

{
    ui->setupUi(this);
    qApp->installEventFilter(this);
    mainWindow = this;
    startUI();
}

void MainWindow::startUI()
{
	getUI()->provinceTable->populate(&map);
	getUI()->groupTable->populate(&map);
	connect(getUI()->provinceTable, &QTableWidget::itemChanged, this, &MainWindow::updateMap);
	connect(getUI()->groupTable, &QTableWidget::itemChanged, this, &MainWindow::updateMap);

	connect(getUI()->DeleteGroupButton, &QPushButton::clicked, getUI()->groupTable, &GroupTable::deleteLast);
	connect(getUI()->DeleteGroupButton, &QPushButton::clicked, this, &MainWindow::updateMap);

	connect(getUI()->CreateGroupButton, &QPushButton::clicked, getUI()->groupTable, &GroupTable::createRow);
	connect(getUI()->CreateGroupButton, &QPushButton::clicked, this, &MainWindow::updateMap);
}

void MainWindow::createMap()
{
	graphic = new MapGFX(map, 0.4f);
	graphic->scale(0.01, 0.01, 10);
}

void MainWindow::updateMap()
{
	ProvincesMask mask(&map);	
	for (unsigned a = 0; a < map.getProvincesList()->size(); a++)
	{
		int targetGroup(getUI()->provinceTable->getGroupOfProvince(a));
		QColor col(getUI()->groupTable->getColorOfGroup(targetGroup ));	
		mask.setColor(col, int(a));
	}
	graphic->applyMask(&mask);

}

MainWindow::~MainWindow()
{
    delete ui;
	delete graphic;
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

void MainWindow::changeProvinceGroupToCurrent(int index)
{
	int group(getUI()->groupTable->currentRow());

	if (group >= 0)
	{
		if (getUI()->provinceTable->item(index, 0)->text().toInt() != group)
			getUI()->provinceTable->item(index, 0)->setText(QString::number(group));
	}
}
