#include "pch.h"
#include "NodeDisplayFPS.h"

NodeDisplayFPS::NodeDisplayFPS()
{
    m_fps = 0;
    display = true;
}


NodeDisplayFPS::~NodeDisplayFPS()
{
}

void NodeDisplayFPS::Update(DX::StepTimer const& timer)
{
    m_fps = timer.GetFramesPerSecond();
    SceneNode::Update(timer);
}

void NodeDisplayFPS::Render()
{
    if (!display)
        return;

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("NodeDisplayFPS");

    std::ostringstream ss;
    ss << "Frames per second: " << m_fps;
    ImGui::Text(ss.str().c_str());
    if (ImGui::Button("Hide", ImVec2(60.0f, 20.0f)))
    {
        display = false;
    }

    ImGui::End();
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    SceneNode::Render();
}
