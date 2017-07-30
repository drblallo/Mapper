#include "grouptable.h"
#include "mappergfx/provincesmask.h"
#include "mapreader/map.h"
#include <QHeaderView>

using namespace mapreader;
using namespace mappergfx;

GroupTable::GroupTable(QWidget* w) : QTableWidget(w)
{
	connect(this, &QTableWidget::itemChanged, this, &GroupTable::modified);
}

void GroupTable::populate(const Map* map)
{
	setSortingEnabled(false);
	setColumnCount(3);	
	setRowCount(1);	
	//horizontalHeader()->setVisible(false);
	//for (unsigned a = 0; a < map->getProvincesList()->size(); a++)
	setItem(0, 0, new QTableWidgetItem("255"));
	setItem(0, 1, new QTableWidgetItem("255"));
	setItem(0, 2, new QTableWidgetItem("255"));
}

void GroupTable::modified(QTableWidgetItem* item)
{
	bool ok(true);
	int t(0);
	t = item->text().toInt(&ok);

	if (!ok || t < 0 || t > 255)
		item->setText("0");
	
	
}

QColor GroupTable::getColorOfGroup(int index)
{
	if (rowCount() <= index || index < 0)
		return QColor(100, 100, 100);

	float r(item(index, 0)->text().toInt());
	float g(item(index, 1)->text().toInt());
	float b(item(index, 2)->text().toInt());

	return QColor(r, g, b);
}

void GroupTable::deleteLast()
{
	if (rowCount() > 0)
		removeRow(rowCount() - 1);
}

void GroupTable::createRow()
{

	setRowCount(rowCount() + 1);	
	setItem(rowCount() - 1, 0, new QTableWidgetItem("255"));
	setItem(rowCount() - 1, 1, new QTableWidgetItem("255"));
	setItem(rowCount() - 1, 2, new QTableWidgetItem("255"));
}
