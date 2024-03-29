#pragma once
#include <QImage>
#include <vector>

namespace mapreader
{
	class Map;
	class SubProvince;
	typedef std::pair<int, int>  position;
	class Province
	{
		friend class Map;
		public:
			const Map* getMap() const;				
			long getId() const;		
			QRgb getColor() const;
			unsigned getIndex() const;
            int getSubProvinceCount() const {return subProvinces.size();}
            const SubProvince* getSubProvince(int target) const {return subProvinces[target];}
			const std::pair<float, float>* getCornerBox() const;
            const std::pair<float, float>* getBoudingBox() const;

			Province(const Province&) = delete;
			Province& operator=(const Province&) = delete;
			void getNeighbours(std::vector<const Province*>* out) const;
			static long getIdFromColor(QRgb color);

		private:	
			Province(QRgb& col, Map* map);
			~Province();
			void updateNeighbourData();

			void addSubProvince(const std::vector<position>& border, QImage& source, QRgb** map);

			Map* map;
			QRgb color;
			long id;
			int index;
			std::vector<SubProvince*> subProvinces;
			int largestBox;
	};
}
