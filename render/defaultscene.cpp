#include "defaultscene.h"
#include "render/test/testQuad.h"
#include "render/defaultinputmanager.h"

using namespace renderer;

//########################################
//##### Constructors and Conversions #####
//########################################
DefaultScene::DefaultScene() : Scene()
{
}

DefaultScene::~DefaultScene()
{
    if (isCurrentScene())
        tearDown();
}

//########################################
//#####           Methods            #####
//########################################
void DefaultScene::setUp()
{
    TestQuad* test =  new TestQuad();
    //DefaultInputManager* manager = new DefaultInputManager();

    spawned.push_back(test);
    //spawned.push_back(manager);
}


void DefaultScene::tearDown()
{
    while (spawned.size() != 0)
    {
        delete spawned[spawned.size()-1];
        spawned.pop_back();
    }
}
