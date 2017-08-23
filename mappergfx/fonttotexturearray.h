#pragma once
#include <QString>
#include <QImage>

class QOpenGLTexture;
namespace mappergfx
{
    class FontToTextureArray
    {
        public:
            FontToTextureArray(QString path);
            ~FontToTextureArray();
            const QImage* getImage(char letter) const;
            const QImage* getImage(int target) const;
            QOpenGLTexture* getTexture(int val) const;

        private:
            void createDistanceField(QImage* image, int borderSize);
            int getDistanceFromColor(int x, int y, std::vector<QRgb*>& modified, QImage* image, int rVal, int borderSize);
            QImage* images[26];
            QImage* imagesHalfSize[26];
            void makeImages(QString path, int size);
    };
}
