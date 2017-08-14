#include "mainwindow.h"
#include <QApplication>
#include <QSurfaceFormat>
#include <QDesktopWidget>
#include "mechanics/mechanicsengine.h"
#include <iostream>
#include "render/defaultscene.h"
#include "ui_mainwindow.h"
#include <QFontDatabase>
#include <QVector4D>
#include <QMessageBox>

#include "mappergfx/testsubregionareas.h"
#include "mappergfx/fonttotexturearray.h"

int main(int argc, char *argv[])
{
    mechanics::MechanicsEngine::StartEngine();
    QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);

    QApplication a(argc, argv);
    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(3,3);
    format.setDepthBufferSize(a.desktop()->depth());
    format.setSamples(2);
    format.setOption(QSurfaceFormat::DebugContext);
    format.setSwapInterval(-1);
    QSurfaceFormat::setDefaultFormat(format);

    int id = QFontDatabase::addApplicationFont("arial.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont font(family, 10);
    a.setFont(font);



    MainWindow w;
    w.resize(a.desktop()->size().width(), a.desktop()->size().height());
    w.show();
    QVector4D v(0, 0, 0, 0);
	w.getUI()->openGLWidget->setClearColor(v);

	renderer::DefaultScene scene;

    return a.exec();
}
