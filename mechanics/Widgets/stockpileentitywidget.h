#ifndef STOCKPILEENTITYWIDGET_H
#define STOCKPILEENTITYWIDGET_H

#include <QWidget>
namespace mechanics
{
    class EntityStockpile;
}
using namespace mechanics;
namespace Ui {
class StockpileEntityWidget;
}

class StockpileEntityWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StockpileEntityWidget(QWidget *parent = 0, EntityStockpile* stockpile = NULL);
    ~StockpileEntityWidget();
    inline EntityStockpile* getStockpile() const{return stockpile;}

private:
    Ui::StockpileEntityWidget *ui;
    EntityStockpile* stockpile;
};

#endif // STOCKPILEENTITYWIDGET_H
