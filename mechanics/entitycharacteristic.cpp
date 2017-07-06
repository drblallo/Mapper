#include "entitycharacteristic.h"

using namespace mechanics;
EntityCharacteristic::EntityCharacteristic(Entity* en, float basevalue, QString name) :
    stat(basevalue, name),
    entity(en)
{
    if (!en)
        throw new exception();
    entity->addCharacteristic(this);
}

EntityCharacteristic::~EntityCharacteristic()
{
    if (entity)
        entity->removeCharacteristic(this);
}
