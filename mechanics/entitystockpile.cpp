#include "entitystockpile.h"

using namespace mechanics;
EntityStockpile::EntityStockpile(Entity* en, float basevalue, QString name) :
    stat(basevalue, name),
    entity(en)
{
    if (!en)
        throw new exception();
    entity->addStockpile(this);
}

EntityStockpile::~EntityStockpile()
{
    if (entity)
        entity->removeStockpile(this);
}
