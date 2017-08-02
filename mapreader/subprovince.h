#pragma once
#include <vector>
#include <map>
#include <QImage>

namespace mapreader
{
	typedef std::pair<int, int>  position;
	struct NeighbourData
	{
		int borderStart;
		int borderEnd;
		long neighbourID;
		unsigned neighbourIndex;
	};

	class Province;
	class SubProvince
	{
		friend class Province;
		public:
			bool isNeinghbourOf(Province& p) const;	
			const Province* getProvince() const {return parent;}
			const std::vector<position>* getBorders() const {return &border;}	
            const std::vector<NeighbourData>* getBordersData() const {return &borderNeighbours;}
			const std::pair<float, float>* getCornerBox() const {return boxCorners;}
			std::vector<NeighbourData> getBorderWith(long id) const;
			SubProvince(const SubProvince&) = delete;
			SubProvince& operator=(const SubProvince&) = delete;
			float getBoxSize() const;
			
		private:
			SubProvince(Province* parent,const std::vector<position>& border, QRgb** , QImage&);
			std::vector<position> border;
			Province* parent;
			std::map<long,const Province*> neighbours;
			std::vector<NeighbourData> borderNeighbours;
			void updateNeighbourData();
			void generateLargestInscribedBox();
			void generateBoxWithOrientation(std::pair<float, float>* outBox, float angle);
			std::pair<float, float> boxCorners[4];

			void addNeighbours(int x, int y, QRgb** source, QImage&);
			bool touchesNeighbour(int x, int y, QRgb col, QRgb** source, QImage&);
	};
}
