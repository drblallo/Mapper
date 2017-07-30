#include "mapgfx.h"
#include "background.h"
#include "provincesmask.h"
#include "borders.h"
#include "mapreader/map.h"

using namespace mappergfx;
using namespace mapreader;

MapGFX::MapGFX(Map& m, float scale) : map(m)
{
	ProvincesMask mask(&m);
	background = new Background(&m, &mask);
	borders = new Borders(&m, scale, &mask);

    background->getTransform()->setTranslation(0, 0, -10);
   // background->getTransform()->setTranslation(0, 0, 0);
    borders->getTransform()->setTranslation(0, 0, -9.99);
    background->getTransform()->setScale(map.getTexture()->width(), map.getTexture()->height(),  10);
   // background->getTransform()->setScale(1, 1, 1);
    borders->getTransform()->setScale(1.0f, 1.0f, 1);
}

MapGFX::~MapGFX()
{
	delete background;
	delete borders;
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
}
			void applyMask(ProvincesMask* mask);
