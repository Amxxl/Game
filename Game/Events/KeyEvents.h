#pragma once

struct EventKeyChar
{
    size_t key;
    bool repeated;
};

struct EventKeyPressed
{
    size_t key;
    bool repeated;
};

struct EventKeyReleased
{
    size_t key;
};