#include "entity.h"
#include <algorithm>
#include "entitycharacteristic.h"
#include "entitystockpile.h"
#include "entityaction.h"

using namespace mechanics;

//########################################
//##### Constructors and Conversions #####
//########################################
Entity::Entity(QString nm) : name(nm){}

Entity::~Entity()
{

}

void Entity::removeCharacteristic(EntityCharacteristic* chara)
{
    stats.erase(remove(stats.begin(), stats.end(), chara), stats.end());
}

void Entity::addCharacteristic(EntityCharacteristic* chara)
{
    stats.push_back(chara);
}

void Entity::removeStockpile(EntityStockpile* chara)
{
    stockpiles.erase(remove(stockpiles.begin(), stockpiles.end(), chara), stockpiles.end());
}

void Entity::addStockpile(EntityStockpile* chara)
{
    stockpiles.push_back(chara);
}

void Entity::addAction(EntityAction* chara)
{
    actions.push_back(chara);
}

void Entity::removeAction(EntityAction* chara)
{
    actions.erase(remove(actions.begin(), actions.end(), chara), actions.end());
}

QString Entity::getToolTip() const
{
    QString toReturn(name);
    toReturn += "\n\n";
    for (unsigned int a = 0; a < stats.size(); a++)
    {
        EntityCharacteristic* c(stats[a]);
        if (!c->getCharacteristic()->canBeShown())
            continue;
        toReturn += c->getCharacteristic()->getName();
        toReturn += QString::number(c->getCharacteristic()->getValue());
        toReturn += "\n";
    }

    for (unsigned int a = 0; a < stockpiles.size(); a++)
    {
        EntityStockpile* c(stockpiles[a]);
        if (!c->getStockpile()->canBeShown())
            continue;

        toReturn += c->getStockpile()->getToolTip();
        toReturn += "\n";
    }

    for (unsigned int a = 0; a < actions.size(); a++)
    {
        EntityAction* c(actions[a]);
        if (!c->getAction()->canBeShown())
            continue;

        toReturn += c->getAction()->getToolTip();
        toReturn += "\n";
    }

    toReturn += "\nTags:\n";
    for (unsigned int a = 0; a < tags.size(); a++)
    {
        toReturn += tags[a] + "\n";
    }

    return toReturn;
}

