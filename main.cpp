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
	
    mapreader::Map map("input.png");
    map.printMapStat();

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
    w.resize(2000, 2000);
    w.show();
	QVector4D v(0, 0, 0, 0);
	w.getUI()->openGLWidget->setClearColor(v);

	renderer::DefaultScene scene;

    //renderer::TestQuad quad;
    //quad.getTransform()->setTranslation(QVector3D(0, 0, -100));

	mappergfx::ProvincesMask mask(&map);

	for (int a =40; a < 56; a++)
		mask.setColor(QColor(255, 0, 0) ,a);


	mappergfx::MapGFX gfx(map, 0.8f);
	gfx.scale(0.01, 0.01, 10);

	gfx.applyMask(&mask);

    return a.exec();
}
