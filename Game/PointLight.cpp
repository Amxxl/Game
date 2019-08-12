#include "pch.h"
#include "PointLight.h"
#include "imgui.h"

PointLight::PointLight(DX::DeviceResources* deviceResources, float radius)
    : cbuf(deviceResources)
{
    Reset();
}

void PointLight::SpawnControlWindow() noexcept
{
    if (ImGui::Begin("Light"))
    {
        ImGui::Text("Position");
        ImGui::SliderFloat("X", &cbData.position.x, -60.0f, 60.0f, "%.1f");
        ImGui::SliderFloat("Y", &cbData.position.y, -60.0f, 60.0f, "%.1f");
        ImGui::SliderFloat("Z", &cbData.position.z, -60.0f, 60.0f, "%.1f");

        ImGui::Text("Intensity/Color");
        ImGui::SliderFloat("Intensity", &cbData.diffuseIntensity, 0.01f, 2.0f, "%.2f", 2);
        ImGui::ColorEdit3("Diffuse Color", &cbData.diffuseColor.x);
        ImGui::ColorEdit3("Ambient", &cbData.ambient.x);

        ImGui::Text("Falloff");
        ImGui::SliderFloat("Constant", &cbData.attConst, 0.05f, 10.0f, "%.2f", 4);
        ImGui::SliderFloat("Linear", &cbData.attLin, 0.0001f, 4.0f, "%.4f", 8);
        ImGui::SliderFloat("Quadratic", &cbData.attQuad, 0.0000001f, 10.0f, "%.7f", 10);

        if (ImGui::Button("Reset"))
        {
            Reset();
        }
    }
    ImGui::End();
}

void PointLight::Reset() noexcept
{
    cbData = {
        { 1.5f,14.0f,-4.5f },
        { 0.05f,0.05f,0.05f },
        { 1.0f,1.0f,1.0f },
        1.0f,
        1.0f,
        0.045f,
        0.0075f,
    };
}

void PointLight::Draw(DX::DeviceResources* deviceResources)
{
}

void PointLight::Bind(DX::DeviceResources* deviceResources, DirectX::FXMMATRIX view) const noexcept
{
    auto dataCopy = cbData;
    auto const position = DirectX::XMLoadFloat3(&cbData.position);
    DirectX::XMStoreFloat3(&dataCopy.position, DirectX::XMVector3Transform(position, view));
    cbuf.Update(deviceResources, dataCopy);
    cbuf.Bind(deviceResources);
}
