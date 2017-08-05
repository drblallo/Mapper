#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <functional>
#include "mappergfx/provincesmask.h"
#include <QColor>
#include <QFileDialog>


using namespace mappergfx;
using namespace mapreader;
MainWindow* MainWindow::mainWindow(NULL);


MainWindow::MainWindow() :
    QMainWindow(NULL),
    ui(new Ui::MainWindow),
	map("input.png"),
	graphic(NULL),
	updateBlocker(0)

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

    connect(getUI()->DeleteGroupButton, &QPushButton::clicked, this, &MainWindow::lockUpdate);
    connect(getUI()->DeleteGroupButton, &QPushButton::clicked, getUI()->groupTable, &GroupTable::deleteLast);
    connect(getUI()->DeleteGroupButton, &QPushButton::clicked, this, &MainWindow::unlockUpdate);
    connect(getUI()->DeleteGroupButton, &QPushButton::clicked, this, &MainWindow::updateMap);

    connect(getUI()->CreateGroupButton, &QPushButton::clicked, this, &MainWindow::lockUpdate);
    connect(getUI()->CreateGroupButton, &QPushButton::clicked, getUI()->groupTable, &GroupTable::createRow);
    connect(getUI()->CreateGroupButton, &QPushButton::clicked, this, &MainWindow::unlockUpdate);
    connect(getUI()->CreateGroupButton, &QPushButton::clicked, this, &MainWindow::updateMap);

	connect(getUI()->actionSaveColors, &QAction::triggered, this, &MainWindow::saveColors);
	connect(getUI()->actionLoadColors, &QAction::triggered, this, &MainWindow::loadColor);
	connect(getUI()->actionLoad_Background, &QAction::triggered, this, &MainWindow::loadBackground);

    connect(ui->provinceTable, &QTableWidget::currentCellChanged, this, &MainWindow::provinceSelectedChanged);
}

void MainWindow::provinceSelectedChanged(int cRow, int cCol, int pRow, int pCol)
{
    graphic->setCurrentSelected(cRow);
}

void MainWindow::createMap()
{
	graphic = new MapGFX(map, 0.2f);
	graphic->scale(0.01, 0.01, 10);
    //getUI()->openGLWidget->setMinimumSize(map.getTexture()->width(), map.getTexture()->height());
}

void MainWindow::loadBackground()
{

	QFileDialog f(this, tr("Load File"));

	f.setFileMode(QFileDialog::ExistingFile);
	f.setNameFilter(tr("Image file (*.png *.jpg)"));
	f.setDirectory(QDir::home().absolutePath());
	f.setOption(QFileDialog::DontUseNativeDialog, true);

	if (!f.exec())
		return;

	graphic->setBackground(f.selectedFiles()[0]);
}

void MainWindow::saveColors()
{
	QFileDialog f(this, tr("Save File"));

	f.setFileMode(QFileDialog::AnyFile);
	f.setNameFilter(tr("Text files (*.txt)"));
	f.setDirectory(QDir::home().absolutePath());
	f.setOption(QFileDialog::DontUseNativeDialog, true);

	if (!f.exec())
		return;

	QString s(f.selectedFiles()[0]);

	QFile path(s);
	if (path.exists())
		path.remove();


	path.open(QIODevice::Text | QIODevice::ReadWrite);
	QTextStream stream(&path);
	for (int a = 0; a < getUI()->provinceTable->rowCount(); a++)
	{
		QString string(getUI()->provinceTable->item(a, 0)->text());
		//path.write(string.toStdString().c_str(), string.length());
		stream << string;
		stream << "\n";
	}
	stream << "group\n";
	
	for (int a = 0; a < getUI()->groupTable->rowCount(); a++)
	{
		QString string(getUI()->groupTable->item(a, 0)->text());
		//path.write(string.toStdString().c_str(), string.length());
		stream << string << "-";
		string = (getUI()->groupTable->item(a, 1)->text());
		stream << string << "-";
		string = (getUI()->groupTable->item(a, 2)->text());
        stream << string << "-";
        string = (getUI()->groupTable->item(a, 4)->text());
        stream << string;
        stream << "\n";
	}
	stream << "end\n";
	path.close();
}

void MainWindow::loadColor()
{
	
	QFileDialog f(this, tr("Load File"));

	f.setFileMode(QFileDialog::ExistingFile);
	f.setNameFilter(tr("Text files (*.txt)"));
	f.setDirectory(QDir::home().absolutePath());
	f.setOption(QFileDialog::DontUseNativeDialog, true);

	if (!f.exec())
		return;

	QString s(f.selectedFiles()[0]);
	QFile path(s);

	path.open(QIODevice::Text | QIODevice::ReadOnly);	
	updateBlocker++;

	QString line(path.readLine());
	bool doneRight;
	line.toInt(&doneRight);
	int count(0);
	while (doneRight && count < getUI()->provinceTable->rowCount())
	{
		line = line.remove('\n');
		getUI()->provinceTable->item(count, 0)->setText(line);
		count++;
		line = path.readLine();
		line.toInt(&doneRight);
	}

	while(line != "group\n")
		line = path.readLine();

	line = path.readLine();

	count = 0;
	while (line != "end\n")
	{
		line = line.remove('\n');
		if (getUI()->groupTable->rowCount() <= count)
			getUI()->groupTable->createRow();
		QStringList ls(line.split('-'));	
        if (ls.size() == 4)
		{
			for (int a = 0; a < 3; a++)
				getUI()->groupTable->item(count, a)->setText(ls[a]);
            getUI()->groupTable->item(count, 4)->setText(ls[3]);
		}
		line = path.readLine();
		count++;
	}

	path.close();
	updateBlocker--;
	updateMap();
}

void MainWindow::updateMap()
{
	if (updateBlocker != 0)
		return;

    updateBlocker++;
	ProvincesMask mask(&map);	
	for (unsigned a = 0; a < map.getProvincesList()->size(); a++)
	{
		int targetGroup(getUI()->provinceTable->getGroupOfProvince(a));
		QColor col(getUI()->groupTable->getColorOfGroup(targetGroup ));	
		mask.setColor(col, int(a));
	}

    for (unsigned a = 0; a < ui->groupTable->rowCount(); a++)
    {
        mask.setName(ui->groupTable->getColorOfGroup(a), ui->groupTable->getNameOfGroup(a));
    }
	graphic->applyMask(&mask);

    updateBlocker--;

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
    if (index == 0)
        return;
	int group(getUI()->groupTable->currentRow());

	if (group >= 0)
	{
		if (getUI()->provinceTable->item(index, 0)->text().toInt() != group)
			getUI()->provinceTable->item(index, 0)->setText(QString::number(group));
	}
}
