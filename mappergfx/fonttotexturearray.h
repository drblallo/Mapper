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
            QImage* images[26];
    };
}
