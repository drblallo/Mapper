#pragma once
#include <QString>
#include <QRgb>
#include <vector>
#include <map>
#include <QVector2D>

class QImage;
namespace mapreader
{
	typedef std::vector<std::pair<int, int> > Region;
	typedef std::pair<QRgb, Region> RegionPair;

	struct VectorNext
	{
		public:
			VectorNext() : c(2) {}

			short getX();
			short getY();
			void invert();
			void rotate();

		private:
			short c;
	};

	struct ParsingData
	{
		int x, y;
		VectorNext next;
		ParsingData() : x(0), y(0), next(){}
	};

	class MapReader
	{
		public:
			MapReader(QImage& source);
			const Region* getVetrices(QRgb& target) const;
			const std::vector<RegionPair>* getRegions() const {return &allRegions;}

		private:
			std::vector<RegionPair> allRegions;
			void cleanSource(const QImage& image, QImage& manipulated) const;
			void parseSource(QImage& image);
			void parseZone(QImage& image, QRgb** map, int x, int y);
			bool parseGetNext(QImage&, QRgb**, ParsingData&, QRgb) const;
	};
}
