#include "defaultinputmanager.h"
#include "input.h"
#include "device.h"
#include "camera3d.h"
#include "glwidget.h"
#include <iostream>

using namespace renderer;

//########################################
//##### Constructors and Conversions #####
//########################################
DefaultInputManager::DefaultInputManager() : EngineObject()
{

}

DefaultInputManager::~DefaultInputManager()
{

}

//########################################
//#####           Methods            #####
//########################################
void DefaultInputManager::Update()
{

    Camera3D *m_camera(Device::getGraphicWindow()->getCamera());
      static const float transSpeed = 0.5f;
      static const float rotSpeed   = 0.5f;

    // Camera Transformation
    if (Input::buttonPressed(Qt::RightButton))
    {
      // Handle rotations
      m_camera->rotate(-rotSpeed * Input::mouseDelta().x(), Camera3D::LocalUp);
      m_camera->rotate(-rotSpeed * Input::mouseDelta().y(), m_camera->right());

    }

      // Handle translations
    QVector3D translation;
    if (Input::keyPressed(Qt::Key_W))
    {
        translation += QVector3D(0, 1, 0);
    }
    if (Input::keyPressed(Qt::Key_S))
    {
        translation += QVector3D(0, -1, 0);
    }
    if (Input::keyPressed(Qt::Key_A))
    {
        translation += QVector3D(-1, 0, 0);
    }
    if (Input::keyPressed(Qt::Key_D))
    {
        translation += QVector3D(1, 0, 0);
    }
    if (Input::keyPressed(Qt::Key_Q) || Input::wheelScrool() <= -1)
    {
        translation += QVector3D(0, 0, 1);
        translation += QVector3D(0, 0, -0.01) * Input::wheelScrool();
    }
    if (Input::keyPressed(Qt::Key_E) || Input::wheelScrool() >= 1)
    {
        translation += QVector3D(0, 0, -1);
        translation += QVector3D(0, 0, -0.01) * Input::wheelScrool();
    }
    m_camera->translate(transSpeed * translation);

	if (Input::keyReleased(Qt::Key_N))
	{
		QVector3D v(Device::getGraphicWindow()->getCamera()->translation());
		std::cout << v.x() << ", " << v.y() << ", " << v.z() << std::endl;
	}

	if (Input::buttonPressed(Qt::LeftButton))
	{
		Device::getGraphicWindow()->leftMouseDown();
	}
}
