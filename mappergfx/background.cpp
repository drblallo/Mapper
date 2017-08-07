#include "background.h"
#include "mapreader/map.h"
#include "render/device.h"
#include <QOpenGLTexture>
#include <vector>
#include <QOpenGLShader>
#include "provincesmask.h"
#include "render/transform3d.h"
#include "mainwindow.h"
#include "provincetable.h"
#include <QMainWindow>

using namespace mapreader;
using namespace mappergfx;
using namespace renderer;
Background::Background(Map* m, ProvincesMask* mask) :
    TexturedObject(":/shaders/background.vert", ":/shaders/background.frag", Device::createTexture(m->getIndexTexture())),
    map(m),
    currSelected(0)
{
	std::vector<float> list;

    list.push_back(-0.5f);
    list.push_back(0.5f);
	list.push_back(0);
	
	list.push_back(1);
	list.push_back(1);
	list.push_back(1);
	list.push_back(1);

	list.push_back(0);
	list.push_back(0);


	
    list.push_back(-0.5f);
    list.push_back(-0.5f);
	list.push_back(0);
	
	list.push_back(1);
	list.push_back(1);
	list.push_back(1);
	list.push_back(1);

	list.push_back(0);
	list.push_back(1);


    list.push_back(0.5f);
    list.push_back(0.5f);
	list.push_back(0);
	
	list.push_back(1);
	list.push_back(1);
	list.push_back(1);
	list.push_back(1);

	list.push_back(1);
	list.push_back(0);


    list.push_back(0.5f);
    list.push_back(0.5f);
	list.push_back(0);
	
	list.push_back(1);
	list.push_back(1);
	list.push_back(1);
	list.push_back(1);

	list.push_back(1);
	list.push_back(0);



    list.push_back(-0.5f);
    list.push_back(-0.5f);
	list.push_back(0);
	
	list.push_back(1);
	list.push_back(1);
	list.push_back(1);
	list.push_back(1);

	list.push_back(0);
	list.push_back(1);


    list.push_back(0.5f);
    list.push_back(-0.5f);
	list.push_back(0);
	
	list.push_back(1);
	list.push_back(1);
	list.push_back(1);
	list.push_back(1);

	list.push_back(1);
	list.push_back(1);

	setBuffer(&list[0], sizeof(float)*list.size());

	renderState.blending.setEnabled(true);
	renderState.blending.setDestinationAlphaFactor(DestinationBlendingOneMinusAlpha);
	renderState.blending.setSourceAlphaFactor(SourceBlendingSourceAlpha);
	renderState.depthMask = false;

    setProvinceMask(*mask);

	texture->setMinificationFilter(QOpenGLTexture::Nearest);
	texture->setMagnificationFilter(QOpenGLTexture::Nearest);

	QImage s("white.png");
	backgroundTexture = Device::createTexture(&s);

	shader->bind();

	int val = shader->uniformLocation("backgroundTexture");
	shader->setUniformValue(val, 1);
    shader->setUniformValue("blackSpaceAlpha", 1.0f);
    shader->setUniformValue("textureColorImportance", 0.0f);
    shader->release();

	canBeDrawn = true;
}

void Background::setBlackSpaceAlpha(float value)
{
    shader->bind();
    shader->setUniformValue("blackSpaceAlpha", value);
    shader->release();
}

void Background::setTextureImportace(float value)
{
    shader->bind();
    shader->setUniformValue("textureColorImportance", value);
    shader->release();

}

void Background::Update()
{
}

void Background::setProvinceMask(ProvincesMask& mask)
{
/*	delete texture;
	texture = Device::createTexture(mask.getImage());
	shader->bind();
	texture->bind();
	AO.bind();
	AO.release();
	texture->release();
	shader->release();
*/
   shader->bind();
   int location(shader->uniformLocation("provinceColors"));
   shader->setUniformValueArray(location, &(mask.getColors()->at(0)), mask.getColors()->size());
   shader->release();

}


Background::~Background()
{
	delete texture;
	delete backgroundTexture;
	regionColorBuffer.destroy();
}

void Background::Prerender()
{
	TexturedObject::Prerender();	
	backgroundTexture->bind(1);

    shader->setUniformValue("selected", currSelected);
}

void Background::PostRender()
{
	backgroundTexture->release();
	TexturedObject::PostRender();

}

//#include <iostream>
//#include "render/linesobject.h"
bool Background::hitted(const QVector3D * source, const QVector3D * dir, QVector3D* out) const
{


	Transform3D t(transform);
	QMatrix4x4 m(t.toMatrix());
	//std::cout << source->x() << ", " << source->y() << ", " << source->z() << std::endl;
	//std::cout << dir->x() << ", " << dir->y() << ", " << dir->z() << std::endl;
	m = m.inverted();
	QVector3D localSource(m * *source);
	QVector3D localDir(m * (*source + *dir));
	localDir = localDir - localSource;


	//std::cout << localSource.x() << ", " << localSource.y() << ", " << localSource.z() << std::endl;
	//std::cout << localDir.x() << ", " << localDir.y() << ", " << localDir.z() << std::endl;
	float val(-1.0f*localSource.z()/localDir.z());
	QVector3D intersectionPoint(localSource + (val*localDir));

	//std::cout << val << std::endl;
	//std::cout << intersectionPoint.x() << ", " << intersectionPoint.y() << ", " << intersectionPoint.z() << std::endl;

	//std::vector<Dot> lines;
	//lines.push_back(Dot(*source));
	//lines.push_back(Dot(*source + (100* *dir)));
	//lines.push_back(Dot(localSource));
	//lines.push_back(Dot(localSource+ (val* localDir)));
	//LinesObject *ogg = new LinesObject(&lines);
	//ogg->getTransform()->setTranslation(0, 0, 0);
	//ogg->getTransform()->setScale(1, 1, 1);

	if (intersectionPoint.x() < 0.5f && intersectionPoint.x() > -0.5f && intersectionPoint.y() > -0.5f && intersectionPoint.y() < 0.5f){
//		std::cout << "this " << std::endl;	
		*out = intersectionPoint;
		return true;
	}
	return false;
//	std::cout << "other" << std::endl;
}

//#include <iostream>
void Background::OnMouseDown(QVector3D intersectionPoint)
{
	int x((intersectionPoint.x()+0.5f)* map->getTexture()->width());
	int y((intersectionPoint.y()+0.5f) * map->getTexture()->height());
	y = map->getTexture()->height() - y;
	QRgb color = map->getIndexOfPixel(x, y);
	int index = qRed(color)*255 + qGreen(color);

//	std::cout << index << std::endl;
//
	//int t = map->getProvinc
	MainWindow::getMainWindow()->changeProvinceGroupToCurrent(index);
	
}

void Background::setBackgroundTexture(QString path)
{
	QImage m(path);	
	delete backgroundTexture;
	backgroundTexture = Device::createTexture(&m);
}

void Background::setSelected(int index)
{
    currSelected = index;
}

