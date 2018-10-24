//
// NodeDisplayFPS.h
//

#pragma once

#include <sstream>

#include "SceneNode.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

class NodeDisplayFPS : public SceneNode
{
    public:
        NodeDisplayFPS();
        ~NodeDisplayFPS();

        void Update(DX::StepTimer const& timer) override;
        void Render() override;

    private:
        unsigned int m_fps;
};

