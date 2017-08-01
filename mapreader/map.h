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
			Province* getProvinceFromIndex(int index){return provincesList[index];}
            //const std::map<long, Province*>* getProvincesMap() const {return &provinces;}
			void printMapStat() const;
			const QImage* getTexture() const {return &source;}
            const QImage* getIndexTexture() const {return &indexTexture;}
            QRgb getIndexOfPixel(int x, int y) const {return indexTexture.pixel(x, y);}
            //const std::vector<std::vector<int> >* getRedTexture() const {return &redTexture;}
			const std::vector<Province*>* getProvincesList() const {return &provincesList;}

		private:
            std::map<long, Province*> provinces;
			std::vector<Province*> provincesList;
			QImage source;
            QImage indexTexture;

            void createProvinces();
            void setUpRedTexture();
            void finalizeInit();

	};
}
