#pragma once

struct EventKeyPressed
{
    size_t key;
    bool repeated;
};

struct EventKeyReleased
{
    size_t key;
};