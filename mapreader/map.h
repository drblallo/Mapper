#pragma once
#include <QString>
#include <map>
#include <QImage>

namespace mapreader
{
	class Province;
	class Map
	{
		public:
			Map(QString source);
			~Map();
			Map(const Map&) = delete;
			Map& operator=(const Map&) = delete;

			const Province* getProvince(QRgb color) const ;
			const Province* getProvince(long id) const ;
            //const std::map<long, Province*>* getProvincesMap() const {return &provinces;}
			void printMapStat() const;
			const QImage* getTexture() const {return &source;}
            int getIndexOfPixel(int x, int y) const {return redTexture[y][x];}
            //const std::vector<std::vector<int> >* getRedTexture() const {return &redTexture;}
			const std::vector<Province*>* getProvincesList() const {return &provincesList;}

		private:
            std::map<long, Province*> provinces;
			std::vector<Province*> provincesList;
			QImage source;
			std::vector<std::vector<int> > redTexture;

            void createProvinces();
            void setUpRedTexture();
            void finalizeInit();

	};
}
