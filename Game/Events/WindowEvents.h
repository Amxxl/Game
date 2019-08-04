#pragma once

#include "Event.h"

struct EventWindowResized
{
    int width;
    int height;
};

struct EventWindowMoved
{
    int x, y;
};

struct EventWindowClosed
{
};