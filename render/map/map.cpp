#include "map.h"
#include <map>

using namespace renderer;
Map::Map(QImage *image) : generatedImage(image->width(), image->height(), QImage::Format_ARGB32)
{
    if (image->format() != QImage::Format_ARGB32)
        throw new exception();
    loadFromImage(image);
}

unsigned int Map::createRegion(QColor color)
{
    lock_guard<mutex> l(m);
    regions.push_back(Region(regions.size(), color));
    connections.push_back(vector<unsigned int>());
    return regions.size() - 1;
}

bool colCompare (QColor first, QColor second)
{
    if (first.red() < second.red())
        return true;
    if (first.red() > second.red())
        return false;
    if (first.green() < second.green())
        return true;
    if (first.green() > second.green())
        return false;
    if (first.blue() < second.blue())
        return true;
    return false;
}

void Map::loadFromImage(QImage *image)
{

    bool(*fn_pt)(QColor, QColor) = colCompare;
    map<QColor, unsigned int, bool(*)(QColor, QColor)> m(fn_pt);
    unsigned int val;
    for (int a = 0; a < image->height(); a++)
    {
        QRgb *rowData = (QRgb*)image->scanLine(a);
        for (int b = 0; b < image->width(); b++)
        {
            QRgb rgb(rowData[b]);
            QColor col(qRed(rgb), qGreen(rgb), qBlue(rgb));
            auto search(m.begin());
            search = m.find(col);
            if (search == m.end())
            {
                val = createRegion(col);
                m.insert(pair<QColor, unsigned int>(col, val));
                QColor c(0, val/256, val%256);
                generatedImage.setPixelColor(b, a, c);
            }
            else
            {
                val = search->second;
                QColor c(0, val/256, val%256);
                generatedImage.setPixelColor(b, a, c);
            }
        }
    }

    for (int a = 0; a < generatedImage.height() - 1; a++)
    {
        QRgb *rowData = (QRgb*)image->scanLine(a);
        QRgb *rowDataNextLine = (QRgb*)image->scanLine(a+1);
        for (int b = 0; b < generatedImage.width() - 1; b++)
        {
            QRgb rgb(rowData[b]);
            QColor col(qRed(rgb), qGreen(rgb), qBlue(rgb));

            unsigned int target((col.green()*256) + col.blue());


            QRgb rgbAfter(rowData[b+1]);
            QColor colAfter(qRed(rgbAfter), qGreen(rgbAfter), qBlue(rgbAfter));

            unsigned int targetAfter((colAfter.green()*256) + colAfter.blue());

            if (target != targetAfter)
                setConnected(target, targetAfter);


            rgbAfter = rowDataNextLine[b];
            colAfter = QColor(qRed(rgbAfter), qGreen(rgbAfter), qBlue(rgbAfter));

            targetAfter = (colAfter.green()*256) + colAfter.blue();

            if (target != targetAfter)
                setConnected(target, targetAfter);

        }

    }
}
