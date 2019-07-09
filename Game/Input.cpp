#include "pch.h"
#include "Input.h"
#include "MouseEvents.h"
#include "KeyEvents.h"

constexpr static unsigned short HID_USAGE_PAGE_GENERIC = static_cast<unsigned short>(0x01);
constexpr static unsigned short HID_USAGE_GENERIC_MOUSE = static_cast<unsigned short>(0x02);

bool Input::m_bMouseButtonStates[static_cast<int>(MouseButton::TOTAL_MOUSE_BUTTONS)];
bool Input::m_bKeyStates[MaxKeys];

Input::Input()
{
    RAWINPUTDEVICE rid;
    rid.usUsagePage = HID_USAGE_PAGE_GENERIC;
    rid.usUsage = HID_USAGE_GENERIC_MOUSE;
    rid.dwFlags = 0;
    rid.hwndTarget = nullptr;

    if (RegisterRawInputDevices(&rid, 1, sizeof(RAWINPUTDEVICE)) == FALSE)
    {
        Logger::Get()->error("Failed to initialize raw input device!");
        PostQuitMessage(1);
    }

    Reset();
}


Input::~Input()
{
}

bool const Input::IsMouseButtonDown(MouseButton const button)
{
    return m_bMouseButtonStates[static_cast<int>(button)];
}

bool const Input::IsKeyDown(size_t const key)
{
    return m_bKeyStates[key];
}

bool Input::IsMouseConnected()
{
    return GetSystemMetrics(SM_MOUSEPRESENT) != 0;
}

bool Input::IsKeyboardConnected()
{
    return true;
}

void Input::OnMouseMoved(Vector2i const& position)
{
    DispatchEvent<EventMouseMoved>(position, m_vecMousePosition);
    m_vecMousePosition = position;
}

void Input::OnMouseMovedRaw(Vector2i const& position)
{
    DispatchEvent<EventMouseMovedRaw>(position);
}

void Input::OnMouseWheelScrolled(Vector2i const& position, float const delta)
{
    m_vecMousePosition = position;
    DispatchEvent<EventMouseWheelScrolled>(position, delta);
}

void Input::OnMouseButtonPressed(Vector2i const& position, MouseButton const button)
{
    m_vecMousePosition = position;
    m_bMouseButtonStates[static_cast<int>(button)] = true;
    DispatchEvent<EventMouseButtonPressed>(position, button);
}

void Input::OnMouseButtonReleased(Vector2i const& position, MouseButton const button)
{
    m_vecMousePosition = position;
    m_bMouseButtonStates[static_cast<int>(button)] = false;
    DispatchEvent<EventMouseButtonReleased>(position, button);
}

void Input::OnMouseButtonDoubleClicked(Vector2i const& position, MouseButton const button)
{
    m_vecMousePosition = position;
    m_bMouseButtonStates[static_cast<int>(button)] = true;
    DispatchEvent<EventMouseButtonDoubleClicked>(position, button);
}

void Input::OnMouseEnter()
{
    Logger::Get()->info("Mouse enter");
    m_bMouseInWindow = true;
    DispatchEvent<EventMouseEnter>();
}

void Input::OnMouseLeave()
{
    Logger::Get()->info("Mouse leave");
    m_bMouseInWindow = false;
    DispatchEvent<EventMouseLeave>();
}

void Input::OnKeyPressed(size_t const key, bool repeated)
{
    if (key >= Key::None && key <= Key::OemClear)
    {
        // Don't do autorepeat if it's disabled.
        if (repeated && !IsAutorepeatEnabled())
            return;

        m_bKeyStates[key] = true;
        DispatchEvent<EventKeyPressed>(key, repeated);
    }
}

void Input::OnKeyReleased(size_t const key)
{
    if (key >= Key::None && key <= Key::OemClear)
    {
        m_bKeyStates[key] = false;
        DispatchEvent<EventKeyReleased>(key);
    }
}

void Input::Reset()
{
    memset(&m_bMouseButtonStates, 0, static_cast<int>(MouseButton::TOTAL_MOUSE_BUTTONS));
    memset(&m_bKeyStates, 0, MaxKeys);
}