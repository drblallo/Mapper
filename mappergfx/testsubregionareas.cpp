#include "testsubregionareas.h"
#include "mapreader/map.h"
#include "mapreader/subprovince.h"
#include "mapreader/province.h"

using namespace renderer;
using namespace mappergfx;
using namespace mapreader;

TestSubRegionAreas::TestSubRegionAreas(Map* map, QVector3D scale)
{

	QImage i("white.png");
	//QOpenGLTexture* texture(renderer::Device::createTexture(&i));
	std::vector<renderer::Dot> quads;

	
	QVector3D offset(map->getTexture()->width()/-2, map->getTexture()->height()/2, 0);
	for (unsigned a = 1; a < map->getProvincesList()->size(); a++)
	{
		mapreader::Province* p(map->getProvinceFromIndex(a));
		for (int b = 0; b < p->getSubProvinceCount(); b++)
		{
			const SubProvince* sub(p->getSubProvince(b));
			const std::pair<float, float>* box(sub->getCornerBox());

			QVector3D start(box[0].first, -1*box[0].second, 0);
			start += offset;
			quads.push_back(renderer::Dot(start));
			start = QVector3D(box[1].first, -1*box[1].second, 0);
			start += offset;
			quads.push_back(renderer::Dot(start));
			quads.push_back(renderer::Dot(start));
			start = QVector3D(box[2].first, -1*box[2].second, 0);
			start += offset;
			quads.push_back(renderer::Dot(start));
			quads.push_back(renderer::Dot(start));
			start = QVector3D(box[3].first, -1*box[3].second, 0);
			start += offset;
			quads.push_back(renderer::Dot(start));
			quads.push_back(renderer::Dot(start));
			start = QVector3D(box[0].first, -1*box[0].second, 0);
			start += offset;
			quads.push_back(renderer::Dot(start));
		}	
	}


	ogg = new LinesObject(&quads);
	ogg->getTransform()->setTranslation(0, 0, -10);
	ogg->getTransform()->setScale(scale);
}

TestSubRegionAreas::~TestSubRegionAreas()
{
	delete ogg;
}
