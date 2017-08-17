#pragma once
#include <QImage>
#include <stack>
#include <random>
#include "interfaces/stringnotifiable.h"

namespace mapreader
{
    //Utility class used to keep track of which element are visited in the map
    class MarkingData
    {
        public:
            MarkingData(int xval, int yval) : x(xval), y(yval){}
            MarkingData(const MarkingData& other) : x(other.x), y(other.y){}
            MarkingData& operator=(const MarkingData& other) {x = other.x; y = other.y; return *this;}
            int x, y;
    };

    //Utility class used to not call image.width() every second.
    class MapInfo
    {
        public:
            MapInfo(QImage& in) : sourceLines(in.height()), width(in.width()), height(in.height()), image(in)
            {
                for (int a = 0; a < image.height(); a++)
                    sourceLines[a] = (QRgb*)image.scanLine(a);
            }

            std::vector<QRgb*> sourceLines;
            int width;
            int height;
            QImage& image;
    };

    //utility class used to transfer data across functions.
    class InscribedRemovalData
    {
    public:
        QRgb oneNeighbourColor;
        bool fondZeroNeighbour;
        QRgb originalAreaColor;
        bool foundAdjactedPixelWithSameColor;
        bool areaIsTouchingMultipleColors;
    };

    //since splitting the map requires keeping track of a lot of factors, it's best to keep
    //track of the by setting them as member fields of a class.
    class WhiteMapToProvinceMap
    {
        public:
            static void splitMap(QImage& out, int provinceCount, StringNotifiable *noto)
            {
                WhiteMapToProvinceMap m(out, provinceCount, noto );
            }

        private:
            WhiteMapToProvinceMap(QImage& out, int provinceCount, StringNotifiable *noto);
            std::default_random_engine generator;
            QRgb black;
            QRgb white;
            QRgb blue;
            QString status;
            MapInfo info;
            std::vector<std::pair<int, int> > provinceCenters;
            int provinceCount;
            StringNotifiable *notif;
            int makeBlackAndWhite();
            int makeBlackEdges();
            void createRegionMapFromWhiteMap();
            void changeColorOfAreasTouchingBorders();
            void changeColorOfArea(int x, int y, QRgb sourceColor, QRgb targetColor);
            int makeProvinceCenters();
            std::pair<int, int> tryPlaceProvince(int index);
            int enlarge(int x, int y, int size, QRgb target);
            inline bool isInBound(int x, int y)
            {
                if (x >= 0 && x < info.width && y >= 0 && y < info.height)
                    return true;
                return false;
            }
            void removeFullyInscribedBlackAreas();
            void removeFullyInscribedBlackArea(int x, int y, QRgb targetColor);
            void finalFix();
            void removeFullyIscribedArea(int x, int y, QRgb targetColor, std::vector<QRgb*>& suppLines);
            inline QRgb getColor(int x, int y) {return info.sourceLines[y][x];}
            inline void setColor(int x, int y, QRgb color) {info.sourceLines[y][x] = color;}
            void parseBlackAreaPixel(int x, int y, std::stack<MarkingData> &stack, InscribedRemovalData &data);
            inline void putpixel(int x, int y, int* modifiedProvinces, QRgb& target)
            {
                if (isInBound(x, y) && white == info.sourceLines[y][x])
                {
                    info.sourceLines[y][x] = target;
                    *modifiedProvinces = *modifiedProvinces + 1;
                }
            }

            void parsePixel(int x, int y, std::stack<MarkingData>& stack, QRgb black, InscribedRemovalData &data);
    };

}
