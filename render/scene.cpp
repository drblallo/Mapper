#include "scene.h"
#include "device.h"
#include "glwidget.h"
#include <iostream>

using namespace renderer;

//########################################
//##### Constructors and Conversions #####
//########################################
Scene::Scene()
{

}

Scene::~Scene()
{
    if (isCurrentScene())
    {
        Device::getGraphicWindow()->scene = NULL;
    }
}

bool Scene::canBeReplacedBy(const Scene *newScene) const
{
    if (!newScene || this == newScene)
        return false;
    return true;
}

//########################################
//#####           Methods            #####
//########################################
void Scene::setAsCurrent()
{
    std::cout << "tried to replace scene, window " << Device::getGraphicWindow() << "\n";
    if (!Device::getGraphicWindow()->scene)
    {
        std::cout << "scene creation called \n";
        setUp();
        Device::getGraphicWindow()->scene = this;
        return;
    }
    if (Device::getGraphicWindow()->scene->canBeReplacedBy(this))
    {
        std::cout << "scene replacement called \n";
        Device::getGraphicWindow()->scene->tearDown();
        Device::getGraphicWindow()->scene = this;
        setUp();
    }
}

//########################################
//#####         Accessors            #####
//########################################
bool Scene::isCurrentScene() const
{
    return (Device::getGraphicWindow()->scene == this);
}
