#include "rawbonus.h"
#include "characteristic.h"

using namespace mechanics;

//########################################
//##### Constructors and Conversions #####
//########################################
RawBonus::RawBonus(float val, const QString* des) : InformativeObject(), value(val), description(*des), enabled(true) {}
RawBonus::RawBonus(float val, const QString des) : InformativeObject(), value(val), description(des), enabled(true) {}


RawBonus::operator float() const
{
    return value;
}

RawBonus::~RawBonus()
{
    for (int a = chars.size() - 1; a >= 0; a++)
        chars[a]->removeBonus(this);
}

//########################################
//#####           Methods            #####
//########################################
void RawBonus::setValue(float newValue)
{
    value = newValue;
}

void RawBonus::setDescription(const QString* newDes)
{
    description = *newDes;
}

void RawBonus::setEnabled(bool ena)
{
    enabled = ena;
}

//########################################
//#####          Accessors           #####
//########################################
QString RawBonus::getToolTip() const
{
    return getName() + ": " + QString::number(getValue());
}

QString RawBonus::getName() const
{
    return description;
}

float RawBonus::getValue() const
{
    return value;
}

bool RawBonus::isEnabled() const
{
    return enabled;
}

const vector<Characteristic*>* RawBonus::getCarateristics() const
{
    return &chars;
}
