#include "fonttotexturearray.h"
#include <QPainter>
#include <QFontDatabase>
#include <QFont>
#include <iostream>
#include <QFile>
#include <QFileInfo>
#include <QFontMetrics>
#include <QOpenGLTexture>
#include "render/device.h"
#include <vector>


using namespace mappergfx;


QString getFamily(QString path)
{

    QFile f (path);
    if (!f.exists())
        std::cout << "file does not exists" << std::endl;
    if (f.open(QIODevice::ReadOnly) == false)
    {
        QFont f;
        return f.defaultFamily();
    }
    int id = QFontDatabase::addApplicationFontFromData(f.readAll());
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    return family;
}

FontToTextureArray::FontToTextureArray(QString path)
{
    makeImages(path, 64);
}

void FontToTextureArray::makeImages(QString path, int size)
{
    QString family(getFamily(path));
    for (int a = 0; a < 26; a++)
    {
        images[a] = new QImage(size, size, QImage::Format_ARGB32);
        images[a]->fill(Qt::transparent);
    }

    QFont font(family, size);
    QFontMetrics metrics(font);
    int dim = size-1;
    font.setBold(true);
    while (metrics.height() > size-6)
    {
        font = QFont(family, dim);
        metrics = QFontMetrics(font);
        font.setBold(true);
        dim--;
    }

    for (int a = 0; a < 26; a++)
    {
        QPainter painter(images[a]);
        painter.setFont(font);
        painter.setPen(Qt::red);
        char c = 'A'+a;
        painter.drawText(0, 0, size, size, Qt::AlignCenter | Qt::AlignTop,QString(c));
    }

 //  for (int a = 0; a < 26; a++)
   //     createDistanceField(images[a], size);
}

bool pixelIsRight(int x, int y, int rval, int width, int height, std::vector<QRgb*>& modified)
{
    if (x >= 0 && x < width && y >= 0 && y < height)
        if (qRed(modified[y][x]) == rval)
            return true;
    return false;
}

int FontToTextureArray::getDistanceFromColor(int x0, int y0, std::vector<QRgb*>& modified, QImage* image, int rVal, int borderSize)
{
    int radius(0);
    bool foundOne(false);
    int width(image->width());
    int height(image->height());
    while (!foundOne && radius< borderSize)
    {
        radius++;
        int x = radius - 1;
        int y = 0;
        int dx = 1;
        int dy = 1;
        int err = dx - (radius << 1);
        while (x >= y)
        {
            if (pixelIsRight(x0 + x, y0 + y, rVal, width, height, modified))
                return radius;
            if (pixelIsRight(x0 + y, y0 + x, rVal, width, height, modified))
                return radius;
            if (pixelIsRight(x0 - y, y0 + x, rVal, width, height, modified))
                return radius;
            if (pixelIsRight(x0 - x, y0 + y, rVal, width, height, modified))
                return radius;
            if (pixelIsRight(x0 - x, y0 - y, rVal, width, height, modified))
                return radius;
            if (pixelIsRight(x0 - y, y0 - x, rVal, width, height, modified))
                return radius;
            if (pixelIsRight(x0 + y, y0 - x, rVal, width, height, modified))
                return radius;
            if (pixelIsRight(x0 + x, y0 - y, rVal, width, height, modified))
                return radius;

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
    return borderSize;
}

void FontToTextureArray::createDistanceField(QImage* image, int borderSize)
{
//    QRgb col(QColor(255, 255, 255, 100).rgba());
    std::vector<QRgb*> modified(image->height());
    for (int a = 0; a < image->height(); a++)
        modified[a] = (QRgb*)image->scanLine(a);

    for (int a = 0; a < image->width(); a++)
    {
        for (int b = 0; b < image->height(); b++)
        {
            if (qAlpha(modified[b][a]) == 255)
                modified[b][a] = QColor(0, 0, 0, 128+(getDistanceFromColor(a, b, modified, image,0, borderSize)*(128/borderSize))).rgba();
            else
                modified[b][a] = QColor(0, 0, 0, 128-(getDistanceFromColor(a, b, modified, image, 255, borderSize)*(128/borderSize))).rgba();
        }
    }

}

FontToTextureArray::~FontToTextureArray()
{
    for (int a = 0; a < 26; a++)
        delete images[a];
}

const QImage* FontToTextureArray::getImage(char letter) const
{
    int index(letter - 'A');
    if (index < 0 || index >= 26)
        return NULL;
    return (images[index]);
}

const QImage* FontToTextureArray::getImage(int target) const
{
    if (target < 0 || target >= 26)
        return NULL;
    return (images[target]);
}

QOpenGLTexture* FontToTextureArray::getTexture(int val) const
{
    if (val < 0 || val >= 26)
        return NULL;
    QOpenGLTexture* t = renderer::Device::createTexture(images[val], QOpenGLTexture::DontGenerateMipMaps);
    t->setMinificationFilter(QOpenGLTexture::Nearest);
    t->setWrapMode(QOpenGLTexture::ClampToEdge);
    return t;
}
