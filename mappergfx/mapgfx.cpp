#include "mapgfx.h"
#include "background.h"
#include "provincesmask.h"
#include "borders.h"
#include "mapreader/map.h"
#include "nameplacer.h"
#include "testsubregionareas.h"
#include "namedisplay.h"
#include "render/device.h"
#include "fonttotexturearray.h"

using namespace mappergfx;
using namespace mapreader;

MapGFX::MapGFX(Map& m, float scale, int borderSkip) : map(m)
{
	ProvincesMask mask(&m);
	background = new Background(&m, &mask);
    borders = new Borders(&m, scale, &mask, borderSkip);
    /*QImage img("inputa.png");
    textTexture = renderer::Device::createTexture(&img);
    textTexture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    textTexture->setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);*/
    background->getTransform()->setTranslation(0, 0, 0);
   // background->getTransform()->setTranslation(0, 0, 0);
    borders->getTransform()->setTranslation(0, 0, 0.01);
    background->getTransform()->setScale(map.getTexture()->width(), map.getTexture()->height(),  1);
   // background->getTransform()->setScale(1, 1, 1);
    borders->getTransform()->setScale(1.0f, 1.0f, 1);
   // borders->hide(true);
   // background->hide(true);
}

MapGFX::~MapGFX()
{
	delete background;
	delete borders;
    //if (areas)
    //	delete areas;
    while (areas.size() != 0)
    {
        delete areas.back();
        areas.pop_back();
    }
}

void MapGFX::scale(const QVector3D& vec)
{
	background->getTransform()->scale(vec);
	borders->getTransform()->scale(vec);
}

void MapGFX::scale(float x, float y, float z)
{
	background->getTransform()->scale(x, y, z);
	borders->getTransform()->scale(x, y, z);
}

void MapGFX::applyMask(ProvincesMask* mask)
{
	background->setProvinceMask(*mask);
	borders->setProvinceMask(mask);
	createTexts(mask);
}

void MapGFX::setBackground(QString path)
{
	background->setBackgroundTexture(path);
}

QVector3D MapGFX::getScale() const
{
	return borders->getTransform()->scale();
}

void MapGFX::createTexts(const ProvincesMask* mask)
{
    QVector2D offset(map.getTexture()->width()/-2, map.getTexture()->height()/2);
    NamePlacer plc(mask);
	/*ifor (int a = 0; a < plc.getRegionCount(); a++)
	{
		std::cout << "-" << plc.getRegion(a)->regionIndexes.size() << std::endl;	
		for (int b = 0; b < plc.getRegion(a)->regionIndexes.size(); b++)
		{
			std::cout << plc.getRegion(a)->regionIndexes[b] << std::endl;
		}
	}*/

    while (areas.size() != 0)
    {
        delete areas.back();
        areas.pop_back();
    }

   // for (int a = 0; a < plc.getRegionCount(); a++)
    //{
        areas.push_back(new NameDisplay(&plc, offset));
        NameDisplay* d(areas.back());
        d->getTransform()->setTranslation(0, 0, 0.02f);

        d->getTransform()->setScale(getScale());
    //}
}

void MapGFX::setBackgroundInterpolationValue(float value)
{
    background->setTextureImportace(value);
}

void MapGFX::setBackgroundAlpa(float value)
{
    background->setBlackSpaceAlpha(value);
}

void MapGFX::reloadBorders(float borderWidth, int borderSkip)
{
    QVector3D scale = getScale();
    ProvincesMask mask(&map);
    delete borders;
    borders = new Borders(&map, borderWidth, &mask, borderSkip);
    borders->getTransform()->setTranslation(0, 0, 0.01);
    borders->getTransform()->setScale(1.0f, 1.0f, 1);
    borders->getTransform()->scale(scale);
}

void MapGFX::setCurrentSelected(int index)
{
    background->setSelected(index);
}

