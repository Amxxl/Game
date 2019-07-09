#pragma once

#include "Event.h"
#include "Vector2.h"
#include "Input.h"

struct EventMouseMoved
{
    Vector2i GetDeltaPosition() const { return position - old_position; }

    Vector2i position;
    Vector2i old_position;
};

struct EventMouseMovedRaw
{
    Vector2i position;
};

struct EventMouseWheelScrolled
{
    Vector2i position;
    float delta;
};

struct EventMouseButtonPressed
{
    Vector2i position;
    Input::MouseButton button;
};

struct EventMouseButtonReleased
{
    Vector2i position;
    Input::MouseButton button;
};

struct EventMouseButtonDoubleClicked
{
    Vector2i position;
    Input::MouseButton button;
};

struct EventMouseEnter {};
struct EventMouseLeave {};