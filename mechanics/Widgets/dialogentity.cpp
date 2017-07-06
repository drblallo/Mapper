#include "dialogentity.h"
#include "ui_dialogentity.h"
#include "mechanics/entity.h"
#include "mechanics/entitycharacteristic.h"
#include "mechanics/entitystockpile.h"
#include "mechanics/entityaction.h"

DialogEntity::DialogEntity(QWidget *parent, Entity *ent) :
    QDialog(parent),
    ui(new Ui::DialogEntity),
    entity(ent)
{
    ui->setupUi(this);
    if (!ent)
        return;
    setWindowTitle(entity->getName());

    for (unsigned int a = 0; a < ent->getStats()->size(); a++)
    {
        EntityCharacteristic* t (ent->getStats()->at(a));
        ui->StatsSlot->addWidget(t->getWidget(this));
    }
    for (unsigned int a = 0; a < ent->getStockpiles()->size(); a++)
    {
        EntityStockpile* t (ent->getStockpiles()->at(a));
        ui->StockPileSlots->addWidget(t->getWidget(this));
    }
    for (unsigned int a = 0; a < ent->getAction()->size(); a++)
    {
        EntityAction* t (ent->getAction()->at(a));
        ui->ActionsSlots->addWidget(t->getWidget(this));
    }
}

DialogEntity::~DialogEntity()
{
    delete ui;
}
