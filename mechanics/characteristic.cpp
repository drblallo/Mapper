#include "characteristic.h"
#include "rawbonus.h"
#include "algorithm"

using namespace mechanics;

//########################################
//##### Constructors and Conversions #####
//########################################
Characteristic::Characteristic(float base, QString nm) : InformativeObject(), chached(base), baseValue(base), name(nm) {}

Characteristic::operator float() const
{
    return chached;
}

Characteristic::~Characteristic()
{
    for (int a = rawExtraBonus.size() - 1; a >= 0; a--)
        removeRawBonuss(rawExtraBonus[a]);

    for (int a = multiplierExtraBonus.size() - 1; a >= 0; a--)
        removeRawBonuss(multiplierExtraBonus[a]);
}

Characteristic::Characteristic(const Characteristic& toCopy) : InformativeObject(), baseValue(toCopy.baseValue), name(toCopy.name)
{

    for (unsigned int a = 0; a < toCopy.rawExtraBonus.size(); a++)
    {
        addRawBonus(toCopy.rawExtraBonus[a]);
    }
    for (unsigned int b = 0; b < toCopy.multiplierExtraBonus.size(); b++)
    {
        addMultiplierBonus(toCopy.multiplierExtraBonus[b]);
    }
}

Characteristic::Characteristic(const Characteristic* toCopy) : InformativeObject(), baseValue(toCopy->baseValue), name(toCopy->name)
{
    for (unsigned int a = 0; a < toCopy->rawExtraBonus.size(); a++)
    {
        addRawBonus(toCopy->rawExtraBonus[a]);
    }
    for (unsigned int b = 0; b < toCopy->multiplierExtraBonus.size(); b++)
    {
        addMultiplierBonus(toCopy->multiplierExtraBonus[b]);
    }
}

Characteristic& Characteristic::operator= (const Characteristic& toCopy)
{
    for (int a = rawExtraBonus.size() - 1; a >= 0; a--)
        removeRawBonuss(rawExtraBonus[a]);

    for (int a = multiplierExtraBonus.size() - 1; a >= 0; a--)
        removeRawBonuss(multiplierExtraBonus[a]);

    for (unsigned int a = 0; a < toCopy.rawExtraBonus.size(); a++)
    {
        addRawBonus(toCopy.rawExtraBonus[a]);
    }
    for (unsigned int b = 0; b < toCopy.multiplierExtraBonus.size(); b++)
    {
        addMultiplierBonus(toCopy.multiplierExtraBonus[b]);
    }
    return *this;
}

//########################################
//#####           Methods            #####
//########################################
float Characteristic::calculate()
{
    chached = getTotalRaw() * getTotalMultiplier();
    return chached;
}

void Characteristic::addRawBonus(RawBonus *bonus)
{
    if (!bonus || rawExtraBonus.end() != find(rawExtraBonus.begin(), rawExtraBonus.end(), bonus))
        return;

    rawExtraBonus.push_back(bonus);
    bonus->chars.push_back(this);
    calculate();
}

void Characteristic::addMultiplierBonus(RawBonus *bonus)
{
    if (!bonus || multiplierExtraBonus.end() != find(multiplierExtraBonus.begin(), multiplierExtraBonus.end(), bonus))
        return;

    multiplierExtraBonus.push_back(bonus);
    bonus->chars.push_back(this);
    calculate();
}

void Characteristic::removeRawBonuss(RawBonus *bonus)
{
    if (!bonus)
        return;

    rawExtraBonus.erase(remove(rawExtraBonus.begin(), rawExtraBonus.end(), bonus), rawExtraBonus.end());
    bonus->chars.erase(remove(bonus->chars.begin(), bonus->chars.end(), this), bonus->chars.end());
    calculate();
}

void Characteristic::removeMultiplierBonus(RawBonus* bonus)
{
    if (!bonus)
        return;

    multiplierExtraBonus.erase(remove(multiplierExtraBonus.begin(), multiplierExtraBonus.end(), bonus), multiplierExtraBonus.end());
    bonus->chars.erase(remove(bonus->chars.begin(), bonus->chars.end(), this), bonus->chars.end());
    calculate();
}

void Characteristic::removeBonus(RawBonus* bonus)
{
    if (!bonus)
        return;

    rawExtraBonus.erase(remove(rawExtraBonus.begin(), rawExtraBonus.end(), bonus ), rawExtraBonus.end());
    multiplierExtraBonus.erase(remove(multiplierExtraBonus.begin(), multiplierExtraBonus.end(), bonus), multiplierExtraBonus.end());
    bonus->chars.erase(remove(bonus->chars.begin(), bonus->chars.end(), this), bonus->chars.end());
    calculate();
}

//########################################
//#####         Accessors            #####
//########################################
float Characteristic::getTotalRaw() const
{
    float base(baseValue);
    for (unsigned int a = 0; a < rawExtraBonus.size(); a++)
        if (rawExtraBonus[a]->isEnabled())
            base += rawExtraBonus[a]->getValue();

    return base;
}

float Characteristic::getTotalMultiplier() const
{
    float multiplier(1);
    for (unsigned int a = 0; a < multiplierExtraBonus.size(); a++)
        if (multiplierExtraBonus[a]->isEnabled())
            multiplier += multiplierExtraBonus[a]->getValue();

    return multiplier;
}

float Characteristic::getBaseValue() const
{
    return baseValue;
}

const vector<RawBonus* >* Characteristic::getRawBonuses() const
{
    return &rawExtraBonus;
}

const vector<RawBonus*>* Characteristic::getMultiplierBonuses() const
{
    return &multiplierExtraBonus;
}

QString Characteristic::getName() const
{
    return name;
}

QString Characteristic::getToolTip() const
{
    QString str(getName() + ": " + QString::number(getValue()));
    str.append('\n');
    str.append('\n');
    RawBonus* b(NULL);
    for (unsigned int a = 0; a < rawExtraBonus.size(); a++)
    {
        b = rawExtraBonus[a];
        str += b->getToolTip();
        str.append("\n");
    }

    for (unsigned int a = 0; a < multiplierExtraBonus.size(); a++)
    {
        b = multiplierExtraBonus[a];
        str += b->getToolTip();
        str.append("%\n");
    }
    return str;
}
