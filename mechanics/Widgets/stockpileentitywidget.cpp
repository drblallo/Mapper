#include "stockpileentitywidget.h"
#include "ui_stockpileentitywidget.h"
#include "mechanics/entitystockpile.h"

StockpileEntityWidget::StockpileEntityWidget(QWidget *parent, EntityStockpile *stc) :
    QWidget(parent),
    ui(new Ui::StockpileEntityWidget),
    stockpile(stc)
{
    ui->setupUi(this);
    if (!stc)
        return;
    setToolTip(stc->getStockpile()->getToolTip());
    ui->nameLabel->setText(stockpile->getStockpile()->getName());
    ui->label->setText(QString::number(stockpile->getStockpile()->getValue()));

}

StockpileEntityWidget::~StockpileEntityWidget()
{
    delete ui;
}
