#pragma once
#include <QTableWidget>

namespace mappergfx
{
	class ProvincesMask;
}

namespace mapreader
{
	class Map;
}

class ProvinceTable : public QTableWidget
{
	public:
		ProvinceTable(QWidget* wid);
		void populateMask(mappergfx::ProvincesMask* mask) const;
		void populate(const mapreader::Map* map);
		int getGroupOfProvince(int index);

	public slots:
		void modified(QTableWidgetItem* item);

};
