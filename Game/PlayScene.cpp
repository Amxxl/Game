//
// PlayScene.cpp
//

#include "pch.h"
#include "PlayScene.h"

PlayScene PlayScene::s_instance;

PlayScene::PlayScene()
{
}

PlayScene::~PlayScene()
{
}

bool PlayScene::Load()
{
    // @todo: Load resources here.
    return true;
}

void PlayScene::Unload()
{
    // @todo: Unload resources here.
}

void PlayScene::Update(DX::StepTimer const& timer)
{
    if (paused)
        return;

    float deltaTime = static_cast<float>(timer.GetElapsedSeconds());

    // @todo: Add your scene logic here.
    deltaTime;
}

void PlayScene::Render()
{
    // @todo: Add your rendering code here.
}
