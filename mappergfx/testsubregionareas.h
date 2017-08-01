#pragma once
#include "render/linesobject.h"

namespace mapreader
{
	class Map;
}
namespace mappergfx
{
	class TestSubRegionAreas  
	{
		public:
			TestSubRegionAreas(mapreader::Map* map, QVector3D scale);
			~TestSubRegionAreas();
		private:
			renderer::LinesObject* ogg;
	};
}
