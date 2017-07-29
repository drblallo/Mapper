#include "background.h"
#include "mapreader/map.h"
#include "render/device.h"
#include <QOpenGLTexture>
#include <vector>
#include <QOpenGLShader>
#include "provincesmask.h"

using namespace mapreader;
using namespace mappergfx;
using namespace renderer;
Background::Background(Map* m, ProvincesMask* mask) : TexturedObject(":/shaders/background.vert", ":/shaders/background.frag", Device::createTexture(m->getIndexTexture())), map(m)
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
	canBeDrawn = true;
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
	regionColorBuffer.destroy();
}

void Background::Prerender()
{
	TexturedObject::Prerender();	
}
