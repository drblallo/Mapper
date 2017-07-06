#pragma once
#include <QColor>

namespace renderer
{
    class Region
    {
        public:
            Region(
                    unsigned int id,
                    QColor first,
                    QColor second = QColor::fromRgba64(0, 0, 0, 0),
                    QColor third = QColor::fromRgba64(0, 0, 0, 0)
                    );

            inline unsigned int getID() const {return regionID;}
            inline QColor getFirstColor() const {return firstColor;}
            inline QColor getSecondColor() const {return secondColor;}
            inline QColor getThirdColor() const {return thirdColor;}

            inline void setFirstColor(QColor color) {firstColor = color;}
            inline void setThirdColor(QColor color) {thirdColor = color;}
            inline void setSecondColor(QColor color) {secondColor = color;}

        private:
            unsigned int regionID;
            QColor firstColor;
            QColor secondColor;
            QColor thirdColor;
            QColor originalColor;
    };
}
