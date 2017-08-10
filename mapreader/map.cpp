#include "map.h"
#include "province.h"
#include "mapreader.h"
#define SPEAK
#include "mapreaderdefines.h"
#include <iostream>

using namespace mapreader;

Map::Map(QString s) :
    provincesList(),
    source(s),
    indexTexture(source.width()*2, source.height()*2, source.format()),
    indexOpenglTexture(QOpenGLTexture::Target2D)

{
    source = source.scaled(source.width()*2, source.height()*2, Qt::IgnoreAspectRatio, Qt::FastTransformation);
    createProvinces();
    finalizeInit();
}

void Map::createProvinces()
{
    MapReader reader(source);
    const std::vector<RegionPair>* ls(reader.getRegions());
    WRITE("Created map reader");

    QRgb* modified[source.height()];
    QRgb black(QColor(0, 0, 0).rgb());

    Province *nullProvince (new Province(black, this));
    provinces.insert(std::pair<long, Province*>(0, nullProvince));
    provincesList.push_back(nullProvince);

    for (int a = 0; a < source.height(); a++)
        modified[a] = (QRgb*)source.scanLine(a);

    for (unsigned a = 0; a < ls->size(); a++)
    {
        QRgb col(ls->at(a).first);
        long id(Province::getIdFromColor(col));
        if (provinces.find(id) == provinces.end())
        {
            WRITE("Created a province");
            Province* p (new Province(col, this));
            provinces.insert(std::pair<long, Province*>(id, p));
            provincesList.push_back(p);
        }
    }
    for (unsigned a = 0; a < provincesList.size(); a++)
        provincesList[a]->index = a;

    WRITE("Adding borders");
    for (unsigned a = 0; a < ls->size(); a++)
    {
        QRgb col(ls->at(a).first);
        long id(Province::getIdFromColor(col));

        provinces.find(id)->second->addSubProvince(ls->at(a).second, source, modified);
    }
}

void Map::finalizeInit()
{
	
    for (unsigned a = 0; a < provincesList.size(); a++)
        provincesList[a]->updateNeighbourData();

    setUpRedTexture();
  //  setUpOpenGLRedTexture();
}

void Map::setUpRedTexture()
{
    //redTexture.resize(source.height());
    QRgb* modified[source.height()];
    QRgb* modified2[source.height()];
    for (int a = 0; a < source.height(); a++)
    {
        modified[a] = (QRgb*)source.scanLine(a);
        modified2[a] = (QRgb*)indexTexture.scanLine(a);
    }
    for (int a = 0; a < source.height(); a++)
    {
        for (int b = 0; b < source.width(); b++)
        {
            QRgb col(modified[a][b]);
            long id(Province::getIdFromColor(col));
            auto iter(provinces.find(id));
            if (iter != provinces.end())
            {
                unsigned indx(iter->second->getIndex());
                modified2[a][b] = QColor(std::floor(indx/255.0f), indx%255, 0).rgb();
                //modified2[a][b] = QColor(0, 255, 0).rgb();
            }
            else
            {
                WRITE("a tile has a color of no province");
                modified2[a][b] = QColor(0, 0, 0).rgb();
            }
        }
    }

}

void Map::setUpOpenGLRedTexture()
{
    indexOpenglTexture.setSize(source.width(), source.height());
    indexOpenglTexture.setAutoMipMapGenerationEnabled(false);
    indexOpenglTexture.setMagnificationFilter(QOpenGLTexture::Linear);
    indexOpenglTexture.setMinificationFilter(QOpenGLTexture::Linear);
   // indexOpenglTexture.setMinimumLevelOfDetail(0);
   // indexOpenglTexture.setMaximumLevelOfDetail(0);
    indexOpenglTexture.setFormat(QOpenGLTexture::R32I);
    //indexOpenglTexture.setLayers(1);
  //  indexOpenglTexture.setMipLevels(1);
    indexOpenglTexture.allocateStorage(QOpenGLTexture::Red_Integer, QOpenGLTexture::Int32);

    std::vector<int> val(source.width() * source.height());

    std::vector<QRgb*> modified(source.height());
    for (int a = 0; a < source.height(); a++)
        modified[a] = (QRgb*)source.scanLine(a);

    for (int a = 0; a < source.height(); a++)
    {
        for (int b = 0; b < source.width(); b++)
        {
            QRgb col(modified[a][b]);
            long id(Province::getIdFromColor(col));
            auto iter(provinces.find(id));
            if (iter != provinces.end())
            {
                int indx(iter->second->getIndex());
                val[b + (a*source.width())] = indx;
                //modified2[a][b] = QColor(0, 255, 0).rgb();
            }
            else
            {
                WRITE("a tile has a color of no province");
                val[b + (a*source.width())] = 0;
            }
        }
    }
    indexOpenglTexture.setData(QOpenGLTexture::Red_Integer, QOpenGLTexture::Int32, &(modified.at(0)));
}


Map::~Map()
{
	for (auto t(provinces.begin()); t != provinces.end(); t++)
		delete t->second;	
}

void Map::printMapStat() const
{
	for (auto t(provinces.begin()); t != provinces.end(); t++)
	{
		std::cout << "id: " << t->first << std::endl;
		std::cout << "sub regions count: " << t->second->subProvinces.size() << std::endl;
	}
}

const Province* Map::getProvince(QRgb color) const 
{
	long id(Province::getIdFromColor(color));
	return getProvince(id);
}

const Province* Map::getProvince(long id) const
{
	auto iter(provinces.find(id));
	if (iter != provinces.end())
		return iter->second;

	return NULL;
}
