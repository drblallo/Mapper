#include "grouptable.h"
#include "mappergfx/provincesmask.h"
#include "mapreader/map.h"
#include <QHeaderView>
#include <chrono>

using namespace mapreader;
using namespace mappergfx;

GroupTable::GroupTable(QWidget* w) : QTableWidget(w), generator(std::chrono::system_clock::now().time_since_epoch().count())
{
	connect(this, &QTableWidget::itemChanged, this, &GroupTable::modified);
}

void GroupTable::populate(const Map* map)
{
	setSortingEnabled(false);
    setColumnCount(5);
    setRowCount(2);
	//horizontalHeader()->setVisible(false);
	//for (unsigned a = 0; a < map->getProvincesList()->size(); a++)
    setItem(0, 0, new QTableWidgetItem("0"));
    setItem(0, 1, new QTableWidgetItem("0"));
    setItem(0, 2, new QTableWidgetItem("0"));
	setItem(0, 3, new QTableWidgetItem(""));
    setItem(0, 4, new QTableWidgetItem("NEW TEXT"));

    setItem(1, 0, new QTableWidgetItem("255"));
    setItem(1, 1, new QTableWidgetItem("255"));
    setItem(1, 2, new QTableWidgetItem("255"));
    setItem(1, 3, new QTableWidgetItem(""));
    setItem(1, 4, new QTableWidgetItem("NEW TEXT"));
}

void GroupTable::modified(QTableWidgetItem* item)
{
	if (item->column() == 3)
		return;

    if (item->column() == 4)
    {
        QString text = item->text();
        text = text.toUpper();
        for (int a = 0; a < text.length(); a++)
        {
            if (!text[a].isLetter() && !text[a].isSpace())
            {
                text = "NEW NAME";
                break;
            }
        }
        item->setText(text);
        return;
    }

	bool ok(true);
	int t(0);
	t = item->text().toInt(&ok);

	if (!ok || t < 0 || t > 255)
		item->setText("0");
	

	for (int a = 0; a < 4; a++)
		if (!item->tableWidget()->item(item->row(), a))
			return;

	float r(item->tableWidget()->item(item->row(), 0)->text().toInt());
	float g(item->tableWidget()->item(item->row(), 1)->text().toInt());
	float b(item->tableWidget()->item(item->row(), 2)->text().toInt());

	QColor c(r, g, b);
	
	item->tableWidget()->item(item->row(), 3)->setBackgroundColor(c);
	
}

QColor GroupTable::getColorOfGroup(int index)
{
	if (rowCount() <= index || index < 0)
        return getColorOfGroup(1);

	float r(item(index, 0)->text().toInt());
	float g(item(index, 1)->text().toInt());
	float b(item(index, 2)->text().toInt());

	return QColor(r, g, b);
}

QString GroupTable::getNameOfGroup(int index)
{
    if (rowCount() <= index || index < 0)
        return QString("NO NAME");

    QString r(item(index, 4)->text());

    return r;
}

void GroupTable::deleteLast()
{
	if (rowCount() > 0)
		removeRow(rowCount() - 1);
}

void GroupTable::createRow()
{
    if (rowCount() == 0)
        return;
    std::uniform_int_distribution<int> dist(1, 255);
	setRowCount(rowCount() + 1);	
    QColor col(dist(generator), dist(generator), dist(generator));
    setItem(rowCount() - 1, 0, new QTableWidgetItem(QString::number(col.red())));
    setItem(rowCount() - 1, 1, new QTableWidgetItem(QString::number(col.green())));
    setItem(rowCount() - 1, 2, new QTableWidgetItem(QString::number(col.blue())));
	setItem(rowCount() - 1, 3, new QTableWidgetItem(""));
    item(rowCount() - 1, 3)->setBackgroundColor(col);
    setItem(rowCount() - 1, 4, new QTableWidgetItem("NEW TEXT"));
    selectRow(rowCount() - 1);
}
