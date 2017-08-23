#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mappergfx/mapgfx.h"
#include "mapreader/map.h"
#include <QOpenGLTexture>
#include <thread>
#include <mutex>
#include "interfaces/stringnotifiable.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public StringNotifiable
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
    void createMap(QString path);
	void changeProvinceGroupToCurrent(int index);
	void saveColors();
	void loadColor();
    void lockUpdate(){updateBlocker++;}
    void unlockUpdate(){updateBlocker--;}
    void loadBackground();
    void loadBlackImage();
    void loadMap();
    void loadFont();
    void resetCameraPosition();
    void provinceSelectedChanged(int cRow, int cCol, int pRow, int pCol);
    void toggleWidgetCamera();
    void reloadBorders();
    void updateBackgroundColor();
    void changeTextureBackgroudAlpha();
    void changeTextureInterpolation();
    void exportCurrentView();
    void createMapInOtherThread(QString path);
    void loadBlackImageInAnotherThread(QString path, QString output, int provinceCount, bool* ended);
    virtual void setNotifyText(QString info);
    virtual QString getNotifyText();
    mapreader::Map* getMap() {return map;}
	mappergfx::MapGFX* getGraphic(){return graphic;}


private:
    Ui::MainWindow *ui;
    static MainWindow* mainWindow;
    mapreader::Map* map;
	mappergfx::MapGFX* graphic;
    QString fontName;
    std::vector<QOpenGLTexture*> textTexture;
    int updateBlocker;
    QSize currentSize;
    QString operationInfo;
    std::mutex mutex;
    QString currentDir;

protected:
    virtual void update();
};

#endif // MAINWINDOW_H
