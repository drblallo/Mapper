#include "whitemaptoprovincemap.h"
#include <stack>
#include <vector>
#include <random>

using namespace mapreader;

bool WhiteMapToProvinceMap::isBlack(QRgb& rgb)
{
    if (qRed(rgb) == 0 && qBlue(rgb) == 0 && qGreen(rgb) == 0)
        return true;
    return false;
}

bool WhiteMapToProvinceMap::isWhite(QRgb& rgb)
{
    if (qRed(rgb) == 255 && qBlue(rgb) == 255 && qGreen(rgb) == 255)
        return true;
    return false;
}

void WhiteMapToProvinceMap::createRegionMapFromWhiteMap(QImage& out, int provinceCount)
{

    std::default_random_engine generator;
    std::vector<QRgb*> outLines(out.height());
    QColor v(QColor(255, 255, 255));
    QRgb white(v.rgb());
    QColor b(QColor(0, 0, 0));
    QRgb black(b.rgb());
    QColor t(QColor(0, 0, 255));
    QRgb blue(t.rgb());

    for (int a = 0; a < out.height(); a++)
    {
        outLines[a] = (QRgb*)out.scanLine(a);
    }

    int whiteCount(0);

    for (int a = 0; a < out.width(); a++)
        for (int b = 0; b < out.height(); b++)
        {
            if (!isBlack(outLines[b][a]))
            {
                outLines[b][a] = white;
                whiteCount++;
            }
        }

    for (int a = 0; a < out.height(); a++)
    {
        outLines[a][0] = black;
        outLines[a][out.width()-1] = black;
        markAllConnectedNeightbours(0, a, outLines, out, black, blue);
        markAllConnectedNeightbours(out.width() - 1, a, outLines, out, black, blue);
    }
    for (int a = 0; a < out.width(); a++)
    {
        outLines[0][a] = black;
        outLines[out.height()-1][a] = black;
        markAllConnectedNeightbours(a, 0, outLines, out, black, blue);
        markAllConnectedNeightbours(a, out.height() - 1, outLines, out, black, blue);
    }

    std::vector<std::pair<int, int> > provinceCenters;
    for (int a = 0; a < provinceCount; a++)
    {
        std::pair<int, int> pos = tryPlaceProvince(provinceCenters.size() + 1, outLines, out, generator);
        if (pos.first != -1)
        {
            provinceCenters.push_back(pos);
            whiteCount--;
        }
    }
    if (provinceCenters.size() == 0)
        return;

    int size(1);
    while (whiteCount > 0 && size < out.width())
    {
        for (unsigned a = 0; a < provinceCenters.size(); a++)
        {
            std::pair<int, int> p = provinceCenters[a];
            enlarge(&whiteCount, p.first, p.second, outLines, out,size, outLines[p.second][p.first]);
        }
            size++;
    }
    for (int a = 0; a < out.width(); a++)
    {
        for (int b = 0; b < out.height(); b++)
        {
            if (outLines[b][a] == black)
                 parseFullySourdondedArea(a, b, outLines, out, black, blue);
        }
    }
    for (int a = 0; a < out.width(); a++)
        for (int b = 0; b < out.height(); b++)
        {
            if (outLines[b][a] == blue)
                outLines[b][a] = black;
        }


}

void WhiteMapToProvinceMap::enlarge(int* modifiedProvinces, int x0, int y0, std::vector<QRgb*>& outLines, QImage& source, int radius, QRgb target)
{
    int x = radius - 1;
    int y = 0;
    int dx = 1;
    int dy = 1;
    int err = dx - (radius << 1);

    while (x >= y)
    {
        putpixel(x0 + x, y0 + y, source, outLines, modifiedProvinces, target);
        putpixel(x0 + y, y0 + x, source, outLines, modifiedProvinces, target);
        putpixel(x0 - y, y0 + x, source, outLines, modifiedProvinces, target);
        putpixel(x0 - x, y0 + y, source, outLines, modifiedProvinces, target);
        putpixel(x0 - x, y0 - y, source, outLines, modifiedProvinces, target);
        putpixel(x0 - y, y0 - x, source, outLines, modifiedProvinces, target);
        putpixel(x0 + y, y0 - x, source, outLines, modifiedProvinces, target);
        putpixel(x0 + x, y0 - y, source, outLines, modifiedProvinces, target);

        if (err <= 0)
        {
            y++;
            err += dy;
            dy += 2;
        }
        else //(err > 0)
        {
            x--;
            dx += 2;
            err += (-radius << 1) + dx;
        }
    }
}

QRgb indexToCol(int index)
{
    int red(index/100);
    red = red * 25;
    index = index % 100;
    int green(index/10);
    green = green * 25;
    index = index %10;
    return QColor(red, green, index).rgb();
}

std::pair<int, int> WhiteMapToProvinceMap::tryPlaceProvince(int index, std::vector<QRgb*>& outLines, QImage& source,std::default_random_engine& generator)
{
    std::uniform_int_distribution<int> distribution(0,source.width()-1);
    std::uniform_int_distribution<int> distributionY(0,source.height()-1);

    int selectedLine(distribution(generator));
    int selectedY(distributionY(generator));

    std::pair<int, int> out;
    int count = 0;
    while (!isWhite(outLines[selectedY][selectedLine]) && count < 50)
    {
        selectedLine = (distribution(generator));
        selectedY = (distributionY(generator));
        count++;
    }
    out.second = selectedY;
    if (count < 50)
    {
        out.first = selectedLine;
        outLines[out.second][out.first] = indexToCol(index);
    }
    else
        out.first = -1;
    return out;
}

bool WhiteMapToProvinceMap::isInBound(int x, int y, QImage& source)
{
    if (x >= 0 && x < source.width() && y >= 0 && y < source.height())
        return true;
    return false;
}

bool WhiteMapToProvinceMap::mustBeAdded(int x, int y, QImage& source, std::vector<QRgb*>& sourceLines, QRgb sourceColor)
{
    if (isInBound(x, y, source) && sourceColor == sourceLines[y][x])
            return true;
    return false;
}

void WhiteMapToProvinceMap::markAllConnectedNeightbours(
        int x,
        int y,
        std::vector<QRgb*>& sourceLines,
        QImage& source,
        QRgb sourceColor,
        QRgb targetColor
        )
{
    if (sourceColor != sourceLines[y][x])
        return;

    std::stack<MarkingData> stack;
    sourceLines[y][x] = targetColor;
    stack.push(MarkingData(x, y));

    while (!stack.empty())
    {
        MarkingData data(stack.top());
        sourceLines[data.y][data.x] = targetColor;
        bool foundOne(false);
        for (int a = -1; a <= 1 && !foundOne; a++)
            for (int b = -1; b <= 1 && !foundOne; b++)
                if (mustBeAdded(data.x + a, data.y + b, source, sourceLines, sourceColor))
                {
                    stack.push(MarkingData(data.x + a, data.y + b));
                    foundOne = true;
                }

        if (!foundOne)
            stack.pop();
    }
}

bool WhiteMapToProvinceMap::parseFullySourdondedArea(int x,int y,std::vector<QRgb*>& sourceLines,QImage& source, QRgb targetColor, QRgb sourceColor)
{
    if (targetColor != sourceLines[y][x])
        return false;
    QRgb oneNeighbourColor(targetColor);


    std::stack<MarkingData> stack;
    sourceLines[y][x] = sourceColor;
    stack.push(MarkingData(x, y));

    bool areaIsTouchingMultipleColors(false);

    while (!stack.empty())
    {
        MarkingData data(stack.top());
        sourceLines[data.y][data.x] = sourceColor;
        bool foundOne(false);
        for (int a = -1; a <= 1 && !foundOne; a++)
            for (int b = -1; b <= 1 && !foundOne; b++)
            {
                if (mustBeAdded(data.x + a, data.y + b, source, sourceLines, targetColor))
                {
                    stack.push(MarkingData(data.x + a, data.y + b));
                    foundOne = true;
                }
                else
                {
                    if (isInBound(data.x+a, data.y+b, source) && sourceLines[data.y+b][data.x+a] != sourceColor)
                    {
                        if (oneNeighbourColor != targetColor)
                        {
                            if (oneNeighbourColor != sourceLines[data.y+b][data.x+a])
                                areaIsTouchingMultipleColors = true;
                        }
                        else
                            oneNeighbourColor = sourceLines[data.y+b][data.x+a];
                    }
                }
            }
        if (!foundOne)
            stack.pop();
    }

    if (!areaIsTouchingMultipleColors)
        markAllConnectedNeightbours(x, y, sourceLines, source, sourceColor, oneNeighbourColor);

}
