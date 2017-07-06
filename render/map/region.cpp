#include "region.h"

using namespace renderer;
Region::Region(
                    unsigned int id,
                    QColor first,
                    QColor second,
                    QColor third
        ) :
    regionID(id),
    firstColor(first),
    secondColor(second),
    thirdColor(third),
    originalColor(first)
{

}
