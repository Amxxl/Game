#pragma once

class MouseData
{
    public:
        static int GetRelativeX() { return x_relative; }
        static int GetRelativeY() { return y_relative; }

        static void SetRelativePos(int x, int y) {
            x_relative = x;
            y_relative = y;
        }

    private:
        static int x_relative;
        static int y_relative;
};

