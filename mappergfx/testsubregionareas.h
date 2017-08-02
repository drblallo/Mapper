#pragma once
#include "render/linesobject.h"

namespace mapreader
{
	class Map;
}
namespace mappergfx
{
	class NamePlacer;
	class TestSubRegionAreas  
	{
		public:
			TestSubRegionAreas(mapreader::Map* map, QVector3D scale);
			TestSubRegionAreas(NamePlacer* plc, mapreader::Map* map,  QVector3D scale);

			~TestSubRegionAreas();
		private:
			renderer::LinesObject* ogg;
	};
}
