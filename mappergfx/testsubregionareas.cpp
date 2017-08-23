#include "testsubregionareas.h"
#include "mapreader/map.h"
#include "mapreader/subprovince.h"
#include "mapreader/province.h"
#include "nameplacer.h"

using namespace renderer;
using namespace mappergfx;
using namespace mapreader;

TestSubRegionAreas::TestSubRegionAreas(Map* map, QVector3D scale)
{

//	QImage i("white.png");
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
    ogg->getTransform()->setTranslation(0, 0, 0.03);
	ogg->getTransform()->setScale(scale);
}

TestSubRegionAreas::TestSubRegionAreas(NamePlacer* plc, Map* map, QVector3D scale)
{
	std::vector<renderer::Dot> quads;
	QVector3D offset(map->getTexture()->width()/-2, map->getTexture()->height()/2, 0);

    for (int b = 0; b < plc->getRegionCount(); b++)
	{
		QVector2D* box(plc->getRegion(b)->corners);

        QVector3D start(box[0].x(), box[0].y(), 0);
        start += offset;
		quads.push_back(renderer::Dot(start));
        start = QVector3D(box[1].x(), box[1].y(), 0);
        start += offset;
		quads.push_back(renderer::Dot(start));
		quads.push_back(renderer::Dot(start));
        start = QVector3D(box[2].x(), box[2].y(), 0);
        start += offset;
		quads.push_back(renderer::Dot(start));
		quads.push_back(renderer::Dot(start));
        start = QVector3D(box[3].x(), box[3].y(), 0);
        start += offset;
		quads.push_back(renderer::Dot(start));
		quads.push_back(renderer::Dot(start));
        start = QVector3D(box[0].x(), box[0].y(), 0);
        start += offset;
		quads.push_back(renderer::Dot(start));
    }

    for (int b = 0; b < plc->getRegionCount(); b++)
    {
        std::vector<QVector2D>* edges(&plc->getRegion(b)->edges);

        for (int a = 0; a < edges->size(); a++)
        {
            QVector3D start(edges->at(a).x(), -1*edges->at(a).y(), 0);
            start += offset;
            quads.push_back(renderer::Dot(start));
        }
    }

	ogg = new LinesObject(&quads);
    ogg->getTransform()->setTranslation(0, 0, 0.03);
	ogg->getTransform()->setScale(scale);
}

TestSubRegionAreas::~TestSubRegionAreas()
{
	delete ogg;
}
