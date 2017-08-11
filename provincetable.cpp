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
    setItem(0, 0, new QTableWidgetItem("0"));
    for (unsigned a = 1; a < map->getProvincesList()->size(); a++)
        setItem(a, 0, new QTableWidgetItem("1"));
}

void ProvinceTable::modified(QTableWidgetItem* item)
{
    if (item->row() == 0)
        item->setText("0");
	bool ok(true);
	int t(0);
	t = item->text().toInt(&ok);

    if (t < 0)
       ok =  false;


	if (!ok)
		item->setText("0");
}

int ProvinceTable::getGroupOfProvince(int index)
{
    if (item(index, 0) != NULL)
    {
        int t = item(index, 0)->text().toInt();
        if (t == 0 && index != 0)
            return 1;
        return t;
    }
    else
    {
        return 1;
    }
}
