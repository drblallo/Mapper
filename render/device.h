#pragma once
#include <string>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>

class GLWidget;
class QOpenGLShaderProgram;
class QImage;
namespace renderer
{
    class Device
    {
        public:

        //########################################
        //#####           Methods            #####
        //########################################
            inline static GLWidget* getGraphicWindow(){return GraphicWindow;}
            static QOpenGLBuffer createVertexBuffer(QOpenGLBuffer::UsagePattern pattern);
            static QOpenGLShaderProgram* createProgram(const std::string& vertex,const std::string& fragment);
            inline static void setGraphicWindow(GLWidget* wid){GraphicWindow = wid;}
            static QOpenGLTexture* createTexture(const std::string* textureName);
            static QOpenGLTexture* createTexture(const QImage *texture, QOpenGLTexture::MipMapGeneration genMipMaps = QOpenGLTexture::GenerateMipMaps);

        private:
        //########################################
        //#####         Privatizing          #####
        //########################################
            Device();

        //########################################
        //#####           Fields             #####
        //########################################
            static GLWidget* GraphicWindow;

    };
}
