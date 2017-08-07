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
    QString family(getFamily(path));
    for (int a = 0; a < 26; a++)
    {
        images[a] = new QImage(128, 128, QImage::Format_ARGB32);
        images[a]->fill(Qt::transparent);
    }

    QFont font(family, 128);
    QFontMetrics metrics(font);
    int dim = 127;
    font.setBold(true);
    while (metrics.height() > 128)
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
        painter.setPen(Qt::black);
        char c = 'A'+a;
        painter.drawText(0, 0, 128, 128, Qt::AlignCenter | Qt::AlignTop,QString(c));
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
    QOpenGLTexture* t = renderer::Device::createTexture(images[val]);
    t->setMinificationFilter(QOpenGLTexture::Nearest);
    return t;
}
