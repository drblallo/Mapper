#pragma once
#include <QImage>

namespace mapreader
{
    class WhiteMapToProvinceMap
    {
        public:
            static void createRegionMapFromWhiteMap(QImage& out, int provinceCount);

        private:
            WhiteMapToProvinceMap();
            static void markAllConnectedNeightbours(int x, int y, std::vector<QRgb *> &sourceLines, QImage& source, QRgb sourceColor, QRgb targetColor);
            static std::pair<int, int> tryPlaceProvince(int index, std::vector<QRgb*>& outLines, QImage& source, std::default_random_engine &generator);
            static bool mustBeAdded(int x, int y, QImage& source, std::vector<QRgb *> &sourceLines, QRgb sourceColor);
            static bool isInBound(int x, int y, QImage& source);
            static void enlarge(int* modifiedProvinces, int x, int y, std::vector<QRgb*>& outLines, QImage& source, int size, QRgb target);
            static bool isBlack(QRgb& rgb);
            static bool isWhite(QRgb& rgb);
            static bool parseFullySourdondedArea(int x, int y, std::vector<QRgb*>& outLines, QImage& source, QRgb targetColor, QRgb sourceColor);
            static inline void putpixel(int x, int y, QImage& source, std::vector<QRgb*>& outLines, int* modifiedProvinces, QRgb& target)
            {
                if (isInBound(x, y, source) && isWhite(outLines[y][x]))
                {
                    outLines[y][x] = target;
                    *modifiedProvinces = *modifiedProvinces - 1;
                }
            }
    };
    class MarkingData
    {
        public:
            MarkingData(int xval, int yval) : x(xval), y(yval){}
            MarkingData(const MarkingData& other) : x(other.x), y(other.y){}
            MarkingData& operator=(const MarkingData& other) {x = other.x; y = other.y; return *this;}
            int x, y;
    };

}
