#pragma once
#include <vector>
#include <QVector2D>

namespace mappergfx
{
	struct ConnectedRegions
	{
		std::vector<int> regionIndexes;	
		QVector2D corners[4];
        std::vector<QVector2D> edges;
        std::vector<QVector2D> centers;
    };

	class ProvincesMask;

	class NamePlacer
	{
		public:
			NamePlacer(const ProvincesMask* mask);
			int getRegionCount() {return division.size();}
			ConnectedRegions* getRegion(int region) {return &division[region];}

		private:
			std::vector<ConnectedRegions> division;
			void generateBoxes(const ProvincesMask* mask);

	};
}
