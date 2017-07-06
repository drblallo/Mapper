#include "stockpile.h"
#include <string>

using namespace mechanics;

StockPile::StockPile(int baseValue, QString n) :
    count(baseValue),
    addModifiers(1, QString("Increase Modifier")),
    removeModifiers(1, QString("Decrease Modifier")),
    name(n)
{

}

void StockPile::add(float t)
{
    if (t > 0)
        count += t*addModifiers;
    else
        count += t*removeModifiers;
}

QString StockPile::getToolTip() const
{
    QString s(getName());

    s += ": " + QString::number(getValue()) + "\n\n";
    s += addModifiers.getToolTip();
    s.append("\n\n");
    s += removeModifiers.getToolTip();
    s.append('\n');

    return s;
}
