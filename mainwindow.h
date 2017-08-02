#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mappergfx/mapgfx.h"
#include "mapreader/map.h"


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
	void updateMap();
	void createMap();
	void changeProvinceGroupToCurrent(int index);
	void saveColors();
	void loadColor();
	void loadBackground();
    void provinceSelectedChanged(int cRow, int cCol, int pRow, int pCol);
    mapreader::Map* getMap() {return &map;}
	mappergfx::MapGFX* getGraphic(){return graphic;}


private:
    Ui::MainWindow *ui;
    static MainWindow* mainWindow;
	mapreader::Map map;
	mappergfx::MapGFX* graphic;
	int updateBlocker;

protected:
    virtual void update();
};

#endif // MAINWINDOW_H
