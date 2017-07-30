#include "borders.h"
#include "mapreader/map.h"
#include "render/device.h"
#include <QOpenGLTexture>
#include "mapreader/province.h"
#include "mapreader/subprovince.h"
#include <QVector2D>
#include <QOpenGLShaderProgram>
#include "provincesmask.h"

using namespace mappergfx;
using namespace renderer;
using namespace mapreader;

QOpenGLTexture* texture(NULL);

QOpenGLTexture* getTexture()
{
    if (!texture)
    {
        QImage img("borderTexture.png");
        texture = Device::createTexture(&img);
    }
    return texture;

}

Borders::Borders(Map* m, float borderScale, ProvincesMask* mask) :
    TexturedObject
    (
        ":/shaders/borders.vert",
        ":/shaders/borders.frag",
        getTexture()
    ), map(m), vertexCount(3), scale(borderScale)
{
    setUpData();
    renderState.blending.setEnabled(true);
    renderState.blending.setDestinationAlphaFactor(DestinationBlendingOneMinusAlpha);
    renderState.blending.setSourceAlphaFactor(SourceBlendingSourceAlpha);
    renderState.depthMask = false;
	setProvinceMask(mask);

    canBeDrawn = true;
}

Borders::~Borders()
{

}

void Borders::setProvinceMask(ProvincesMask* mask)
{

   shader->bind();
   int location(shader->uniformLocation("provincesColor"));
   shader->setUniformValueArray(location, &(mask->getColors()->at(0)), mask->getColors()->size());
   shader->release();
}

void addVertex(
		std::vector<float>& list,
		const QRgb& color,
		const QVector2D& pos, 
		float u, 
		float v,
		float provinceIndex,
		float otherProvinceIndex)

{
    list.push_back(pos.x());
    list.push_back(pos.y()*-1);
    list.push_back(0);

    list.push_back(0);
    //list.push_back(float(qRed(color)/255.0f));
    list.push_back(0);
    list.push_back(0);
    list.push_back(1);

    list.push_back(u);
    list.push_back(v);
	list.push_back(provinceIndex);
	list.push_back(otherProvinceIndex);
}

void addTris(
        std::vector<float>& list,
        const QRgb& color,
        const std::pair<int, int>& curr,
        const std::pair<int, int>& next,
        const std::pair<int, int>& next2,
        const std::pair<int, int>& prev,
        int mapWidth,
        int mapHeight,
		float scale,
		float provinceIndex,
		float otherProvinceIndex
        )
{
	
	float displacementX(mapWidth/2 - 0.5f);
	float displacementY(mapHeight/2 - 0.5f);

    QVector2D first(curr.first - displacementX, curr.second - displacementY);
    QVector2D second(next.first - displacementX, next.second- displacementY);
    QVector2D third(next2.first - displacementX, next2.second- displacementY);
    QVector2D quart(prev.first - displacementX, prev.second- displacementY);

    QVector2D direction(second - first);
    QVector2D direction2(third - second);
    QVector2D direction3(first - quart);
    direction = QVector2D(direction.y(), direction.x()*-1) / scale;
    direction2 = QVector2D(direction2.y(), direction2.x()*-1) / scale;
    direction3 = QVector2D(direction3.y(), direction3.x()*-1) / scale;

    QVector2D firsMoved(first + ((direction+direction3)/2));
    QVector2D secondMoved(second + ((direction2+direction)/2));
	first = first - ((direction+direction3)/2);
	second = second - ((direction2+direction)/2);


    addVertex(list, color, first, 0, 0, provinceIndex, otherProvinceIndex);
    addVertex(list, color, second, 1, 0, provinceIndex, otherProvinceIndex);
    addVertex(list, color, firsMoved, 0, 1, provinceIndex, otherProvinceIndex);
    addVertex(list, color, secondMoved, 1, 1, provinceIndex, otherProvinceIndex);
    addVertex(list, color, firsMoved, 0, 1, provinceIndex, otherProvinceIndex);
    addVertex(list, color, second, 1, 0, provinceIndex, otherProvinceIndex);
}

void addGenerateSubProvinceBorders(std::vector<float>& list, const SubProvince& province, int mapWidth, int mapHeight, float scale)
{
    auto neighData(province.getBordersData());
    auto borders(province.getBorders());
    for (unsigned a = 0; a < neighData->size(); a++)
    {
        NeighbourData data(neighData->at(a));
        if (data.borderEnd - data.borderStart < 4)
            continue;

        int c(2);
        for (int x = data.borderStart; x <= data.borderEnd; x = x + c)
        {
            std::pair<int, int> curr(borders->at(x));
            std::pair<int, int> next;
            std::pair<int, int> next2;
            std::pair<int, int> prev;

            if (x+c >= borders->size())
            {
                next = borders->at(0);
                next2 = borders->at(c);
            }
            else
            {
                next = borders->at(x+c);
                if (x + (2*c) >= borders->size())
                    next2 = borders->at(0);
                else
                    next2 = borders->at(x + (2*c));
            }

            if (x - c < 0)
                prev = borders->front();
            else
                prev = borders->at(x - c);

            addTris(
					list,
				   	province.getProvince()->getColor(),
				   	curr,
				   	next,
				   	next2,
				   	prev,
				   	mapWidth,
				   	mapHeight,
				   	scale,
				   	province.getProvince()->getIndex(),
					data.neighbourIndex
					);
        }
    }
}

void addGenerateProvinceBorders(std::vector<float>& list,Province& province, int mapWidth, int mapHeight, float scale)
{
    for (int a = 0; a < province.getSubProvinceCount(); a++)
        addGenerateSubProvinceBorders(list, *province.getSubProvince(a), mapWidth, mapHeight, scale);
}
void Borders::setUpData()
{
    std::vector<float> list;

    for (unsigned a = 1; a < map->getProvincesList()->size(); a++)
        addGenerateProvinceBorders(list, *map->getProvincesList()->at(a), map->getTexture()->width(), map->getTexture()->height(), scale);


    setBuffer(&(list[0]), list.size() * sizeof(float));
    vertexCount = list.size() / 11;

}

void Borders::setBuffer(const void * pos, int size)
{
    shader->bind();
    texture->bind();
    buffer.bind();
    AO.bind();

    buffer.allocate(pos, size);
    shader->enableAttributeArray(0);
    shader->enableAttributeArray(1);
    shader->enableAttributeArray(2);
    shader->enableAttributeArray(3);
    shader->enableAttributeArray(4);
    shader->setAttributeBuffer(0, GL_FLOAT, 0, 3, 11*sizeof(float));
    shader->setAttributeBuffer(1, GL_FLOAT, 3*sizeof(float), 4, 11*sizeof(float));
    shader->setAttributeBuffer(2, GL_FLOAT, 7*sizeof(float), 2, 11*sizeof(float));
    shader->setAttributeBuffer(3, GL_FLOAT, 9*sizeof(float), 1, 11*sizeof(float));
    shader->setAttributeBuffer(4, GL_FLOAT, 10*sizeof(float), 1, 11*sizeof(float));

    AO.release();
    buffer.release();
    texture->release();
    shader->release();
}
