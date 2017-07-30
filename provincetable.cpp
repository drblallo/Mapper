#include "provincetable.h"
#include "mappergfx/provincesmask.h"
#include "mapreader/map.h"
#include <QHeaderView>

using namespace mapreader;
using namespace mappergfx;

ProvinceTable::ProvinceTable(QWidget* w) : QTableWidget(w)
{
	connect(this, &QTableWidget::itemChanged, this, &ProvinceTable::modified);
}

void ProvinceTable::populate(const Map* map)
{
	setSortingEnabled(false);
	setColumnCount(1);	
	setRowCount(map->getProvincesList()->size());	
	horizontalHeader()->setVisible(false);
	for (unsigned a = 0; a < map->getProvincesList()->size(); a++)
		setItem(a, 0, new QTableWidgetItem("0"));
}

void ProvinceTable::modified(QTableWidgetItem* item)
{
	bool ok(true);
	int t(0);
	t = item->text().toInt(&ok);

	if (!ok)
		item->setText("0");
}

#include <iostream>
int ProvinceTable::getGroupOfProvince(int index)
{
    if (item(index, 0) != NULL )
    {
        int t = item(index, 0)->text().toInt();
        return t;
    }
    else
    {
        return 0;
    }
}
