#pragma once
#include <QTableWidget>
#include <QColor>
#include <random>

namespace mappergfx
{
	class ProvincesMask;
}

namespace mapreader
{
	class Map;
}

class GroupTable : public QTableWidget
{
	public:
		GroupTable(QWidget* wid);
		void populateMask(mappergfx::ProvincesMask* mask) const;
		void populate(const mapreader::Map* map);
		QColor getColorOfGroup(int index);
        QString getNameOfGroup(int index);
        void deleteLast();
		void createRow();

    private:
        std::default_random_engine generator;

	public slots:
		void modified(QTableWidgetItem* item);

};
