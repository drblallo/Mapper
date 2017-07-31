#include "mainwindow.h"
#include <QApplication>
#include <QSurfaceFormat>
#include <QDesktopWidget>
#include "mechanics/mechanicsengine.h"
#include <iostream>
#include "render/defaultscene.h"
#include "ui_mainwindow.h"
#include <QVector4D>

#include "mapreader/map.h"
#include "mappergfx/mapgfx.h"
#include "render/test/testQuad.h"
#include "mappergfx/provincesmask.h"


int main(int argc, char *argv[])
{
	
    mechanics::MechanicsEngine::StartEngine();
	

    QApplication a(argc, argv);
    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(4,1);
    format.setDepthBufferSize(a.desktop()->depth());
    format.setSamples(2);
    format.setOption(QSurfaceFormat::DebugContext);
    format.setSwapInterval(-1);
	
	
    QSurfaceFormat::setDefaultFormat(format);


    MainWindow w;
    w.resize(a.desktop()->size().width(), a.desktop()->size().height());
    //w.resize(2000, 2000);
    w.show();
	QVector4D v(0, 0, 200, 0);
	w.getUI()->openGLWidget->setClearColor(v);
	renderer::DefaultScene scene;
	w.createMap();




    return a.exec();
}
