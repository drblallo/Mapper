#include "mapgfx.h"
#include "background.h"
#include "provincesmask.h"
#include "borders.h"
#include "mapreader/map.h"
#include "nameplacer.h"
#include "testsubregionareas.h"

using namespace mappergfx;
using namespace mapreader;

MapGFX::MapGFX(Map& m, float scale) : map(m), areas(NULL)
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
  //  borders->hide(true);
}

MapGFX::~MapGFX()
{
	delete background;
	delete borders;
	if (areas)
		delete areas;
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
	NamePlacer plc(mask);
	/*ifor (int a = 0; a < plc.getRegionCount(); a++)
	{
		std::cout << "-" << plc.getRegion(a)->regionIndexes.size() << std::endl;	
		for (int b = 0; b < plc.getRegion(a)->regionIndexes.size(); b++)
		{
			std::cout << plc.getRegion(a)->regionIndexes[b] << std::endl;
		}
	}*/

	if (areas)
		delete areas;
    areas = new TestSubRegionAreas(&plc, &map ,getScale());
}

void MapGFX::setCurrentSelected(int index)
{
    background->setSelected(index);
}

