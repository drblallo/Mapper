#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <functional>
#include "mappergfx/provincesmask.h"
#include <QColor>
#include <QFileDialog>
#include "mappergfx/fonttotexturearray.h"
#include "mappergfx/namedisplay.h"
#include "blackmaploaderdialog.h"
#include "mapreader/whitemaptoprovincemap.h"
#include <QProgressDialog>
#include <thread>

using namespace mappergfx;
using namespace mapreader;
MainWindow* MainWindow::mainWindow(NULL);


MainWindow::MainWindow() :
    QMainWindow(NULL),
    ui(new Ui::MainWindow),
    map(NULL),
    graphic(NULL),
    fontName(""),
	updateBlocker(0)

{
    ui->setupUi(this);
    qApp->installEventFilter(this);
    mainWindow = this;
    startUI();
}

void MainWindow::startUI()
{

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

    connect(ui->actionLoad_Map, &QAction::triggered, this, &MainWindow::lockUpdate);
    connect(ui->actionLoad_Map, &QAction::triggered, this, &MainWindow::loadMap);
    connect(ui->actionLoad_Map, &QAction::triggered, this, &MainWindow::unlockUpdate);


    connect(ui->actionLoad_Font, &QAction::triggered, this, &MainWindow::lockUpdate);
    connect(ui->actionLoad_Font, &QAction::triggered, this, &MainWindow::loadFont);
    connect(ui->actionLoad_Font, &QAction::triggered, this, &MainWindow::unlockUpdate);
    connect(ui->actionLoad_Font, &QAction::triggered, this, &MainWindow::updateMap);

    connect(ui->actionplace_camera_to_origin, &QAction::triggered, this, &MainWindow::resetCameraPosition);

    connect(ui->actionScreen_large_as_image, &QAction::toggled, this, &MainWindow::toggleWidgetCamera);

    connect(ui->actionLoad_Black_Map, &QAction::triggered, this, &MainWindow::lockUpdate);
    connect(ui->actionLoad_Black_Map, &QAction::triggered, this, &MainWindow::loadBlackImage);
    connect(ui->actionLoad_Black_Map, &QAction::triggered, this, &MainWindow::unlockUpdate);


    connect(ui->borderSkip, static_cast<void (QSpinBox::*) (int)> (&QSpinBox::valueChanged), this, &MainWindow::reloadBorders);
    connect(ui->borderWidth, static_cast<void (QDoubleSpinBox::*) (double)> (&QDoubleSpinBox::valueChanged), this, &MainWindow::reloadBorders);

    connect(ui->backgroundRed, static_cast<void (QSpinBox::*) (int)> (&QSpinBox::valueChanged), this, &MainWindow::updateBackgroundColor);
    connect(ui->backgroundGreen, static_cast<void (QSpinBox::*) (int)> (&QSpinBox::valueChanged), this, &MainWindow::updateBackgroundColor);
    connect(ui->backgroundBlue, static_cast<void (QSpinBox::*) (int)> (&QSpinBox::valueChanged), this, &MainWindow::updateBackgroundColor);

    connect(ui->textureBackgroundAlpa, static_cast<void (QDoubleSpinBox::*) (double)> (&QDoubleSpinBox::valueChanged), this, &MainWindow::changeTextureBackgroudAlpha);
    connect(ui->textureInterpolation, static_cast<void (QDoubleSpinBox::*) (double)> (&QDoubleSpinBox::valueChanged), this, &MainWindow::changeTextureInterpolation);

    connect(ui->actionExport, &QAction::triggered, this, &MainWindow::exportCurrentView);
}

void MainWindow::loadBlackImageInAnotherThread(QString path, QString output, int provinceCount, bool* ended)
{

    QImage image(path);
    mapreader::WhiteMapToProvinceMap::createRegionMapFromWhiteMap(image, provinceCount);
    image.save(output);
    *ended = true;
}

void MainWindow::loadBlackImage()
{
    BlackMapLoaderDialog dialog;

    if (!dialog.exec() || dialog.getOutput().length() == 0)
        return;

    QFile path(dialog.getInput());
    if (!path.exists())
        return;


    bool ended = false;
    std::thread th(std::bind(&MainWindow::loadBlackImageInAnotherThread, this, dialog.getInput(),dialog.getOutput(), dialog.getProvinceCount(), &ended));
    th.detach();
    QProgressDialog progress("Splitting Map into provinces, could take a while", "cancel", 0, 0, this);
   // progress.setCancelButton(0);
    progress.setWindowModality(Qt::WindowModal);
    progress.show();
    //progress.setValue(0);

    QEventLoop lp;
    while (!ended)
    {
        lp.processEvents();
        if (progress.wasCanceled())
            return;
    }
    progress.hide();

    createMap(dialog.getOutput());
}

void MainWindow::exportCurrentView()
{

    QFileDialog f(this, tr("Export View"));

    f.setFileMode(QFileDialog::AnyFile);
    f.setNameFilter(tr("Image file (*.png)"));
    f.setDirectory(QDir::home().absolutePath());
    f.setOption(QFileDialog::DontUseNativeDialog, true);

    if (!f.exec())
        return;

    QString s(f.selectedFiles()[0]);
    if (!s.endsWith(".png"))
        s.append(".png");

    ui->openGLWidget->grabFramebuffer().save(s);
}

void MainWindow::changeTextureBackgroudAlpha()
{
    if (graphic)
        graphic->setBackgroundAlpa(ui->textureBackgroundAlpa->value());
}

void MainWindow::changeTextureInterpolation()
{
    if (graphic)
        graphic->setBackgroundInterpolationValue(ui->textureInterpolation->value());
}

void MainWindow::resetCameraPosition()
{
    ui->openGLWidget->getCamera()->setRotation(QQuaternion::fromEulerAngles(0, 0, 0));

    ui->openGLWidget->getCamera()->setTranslation(0, 0, 50);
}

void MainWindow::reloadBorders()
{
    if (graphic)
    {
        graphic->reloadBorders(ui->borderWidth->value(), ui->borderSkip->value());
        updateMap();
    }
}

void MainWindow::updateBackgroundColor()
{
    QVector4D v(ui->backgroundRed->value(), ui->backgroundGreen->value(), ui->backgroundBlue->value(), 1);
    ui->openGLWidget->setClearColor(v/255.0f);
}

void MainWindow::loadMap()
{
    QFileDialog f(this, tr("Load Map"));

    f.setFileMode(QFileDialog::ExistingFile);
    f.setNameFilter(tr("Image file (*.png)"));
    f.setDirectory(QDir::home().absolutePath());
    f.setOption(QFileDialog::DontUseNativeDialog, true);

    if (!f.exec())
        return;

    createMap(f.selectedFiles()[0]);

}

void MainWindow::toggleWidgetCamera()
{
    if (!map)
        return;
    if (ui->actionScreen_large_as_image->isChecked())
    {
        currentSize = ui->openGLWidget->minimumSize();
        ui->openGLWidget->setMinimumSize(map->getTexture()->width()/2,map->getTexture()->height()/2);
    }
    else
    {
        ui->openGLWidget->setMinimumSize(currentSize);
    }
}

void MainWindow::loadFont()
{
    QFileDialog f(this, tr("Load Font"));

    f.setFileMode(QFileDialog::ExistingFile);
    f.setNameFilter(tr("Font file (*.ttf)"));
    f.setDirectory(QDir::home().absolutePath());
    f.setOption(QFileDialog::DontUseNativeDialog, true);

    if (!f.exec())
        return;

    fontName = f.selectedFiles()[0];
    for (int a = 0; a < textTexture.size(); a++)
    {
        textTexture[a]->destroy();
        delete textTexture[a];
    }
    textTexture.clear();
    mappergfx::FontToTextureArray text(fontName);
    for (int a = 0; a < 26; a++)
        textTexture.push_back(text.getTexture(a));
    graphic->setLetters(textTexture);
}

void MainWindow::provinceSelectedChanged(int cRow, int, int, int)
{
    graphic->setCurrentSelected(cRow);
}

void MainWindow::createMapInOtherThread(QString path)
{
    map = new Map(path);
}

void MainWindow::createMap(QString path)
{
    if (graphic)
        delete graphic;
    if (map)
        delete map;
    map = NULL;
    std::thread th(std::bind(&MainWindow::createMapInOtherThread, this, path));
    th.detach();
    QProgressDialog progress("parsing splitted map, could take a while", "cancel", 0, 0, this);
    //progress.setCancelButton(0);
    progress.setWindowModality(Qt::WindowModal);
    progress.show();
    //progress.setValue(0);

    QEventLoop lp;
    while (!map)
    {
        lp.processEvents();
        if (progress.wasCanceled())
            return;
    }



    graphic = new MapGFX(*map, 5, 9);
    float scale(100.0f/map->getTexture()->width());

    graphic->scale(scale, scale, 10);
    getUI()->provinceTable->populate(map);
    getUI()->groupTable->populate(map);
    //getUI()->openGLWidget->setMinimumSize(map.getTexture()->width(), map.getTexture()->height());
    resetCameraPosition();

    ui->actionSaveColors->setEnabled(true);
    ui->actionLoadColors->setEnabled(true);
    ui->actionLoad_Background->setEnabled(true);
    ui->actionScreen_large_as_image->setEnabled(true);
    ui->actionLoad_Font->setEnabled(true);
}

void MainWindow::loadBackground()
{

    if (!graphic)
        return;
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
    if (!graphic)
        return;
	QFileDialog f(this, tr("Save File"));

	f.setFileMode(QFileDialog::AnyFile);
	f.setNameFilter(tr("Text files (*.txt)"));
	f.setDirectory(QDir::home().absolutePath());
	f.setOption(QFileDialog::DontUseNativeDialog, true);

	if (!f.exec())
		return;

	QString s(f.selectedFiles()[0]);

    if (!s.endsWith(".txt"))
        s.append(".txt");
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
    if (!graphic)
        return;
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
    if (updateBlocker != 0 || !graphic)
		return;

    updateBlocker++;
    ProvincesMask mask(map);
    for (unsigned a = 0; a < map->getProvincesList()->size(); a++)
	{
		int targetGroup(getUI()->provinceTable->getGroupOfProvince(a));
        QColor col(getUI()->groupTable->getColorOfGroup(targetGroup));
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
    if  (graphic)
        delete graphic;
    if (map)
        delete map;
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
    if (!graphic)
        return;
    if (index == 0)
        return;
	int group(getUI()->groupTable->currentRow());

	if (group >= 0)
	{
		if (getUI()->provinceTable->item(index, 0)->text().toInt() != group)
			getUI()->provinceTable->item(index, 0)->setText(QString::number(group));
	}
}
