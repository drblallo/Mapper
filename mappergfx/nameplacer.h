#pragma once
#include <vector>
#include <QVector2D>
#include <QString>

namespace mappergfx
{
    class ConnectedRegions
	{
        public:
            ConnectedRegions() : name("NO NAME"){}
            std::vector<int> regionIndexes;
            QVector2D corners[4];
            std::vector<QVector2D> edges;
            std::vector<QVector2D> centers;
            QString name;
    };

	class ProvincesMask;

	class NamePlacer
	{
		public:
			NamePlacer(const ProvincesMask* mask);
            int getRegionCount() const {return division.size();}
			ConnectedRegions* getRegion(int region) {return &division[region];}
            const ConnectedRegions* getRegion(int region) const {return &division[region];}

		private:
			std::vector<ConnectedRegions> division;
			void generateBoxes(const ProvincesMask* mask);

	};
}
