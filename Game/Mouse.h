#pragma once

#include <queue>

class Mouse
{
    public:
        class Event
        {
            public:
                enum Type
                {
                    LeftButtonPressed,
                    LeftButtonReleased,
                    RightButtonPressed,
                    RightButtonReleased,
                    MiddleButtonPressed,
                    MiddleButtonReleased,
                    XButton1Pressed,
                    XButton1Released,
                    XButton2Pressed,
                    XButton2Released,
                    WheelMoveUp,
                    WheelMoveDown,
                    Move,
                    MoveRaw,
                    Invalid
                };

                Event() : eventType(Type::Invalid), x(0), y(0) {}

                Event(Type type, int const x, int const y)
                    : eventType(type), x(x), y(y)
                {

                }

                bool IsValid() const { return eventType != Type::Invalid; }
                Type GetType() const { return eventType; }

                int const GetPositionX() const { return x; }
                int const GetPositionY() const { return y; }

            private:
                Type eventType;
                int x, y;
        };

        Mouse();
        ~Mouse();

        void OnLeftPressed(int x, int y);
        void OnLeftReleased(int x, int y);
        void OnRightPressed(int x, int y);
        void OnRightReleased(int x, int y);
        void OnMiddlePressed(int x, int y);
        void OnMiddleReleased(int x, int y);
        void OnXButton1Pressed(int x, int y);
        void OnXButton1Released(int x, int y);
        void OnXButton2Pressed(int x, int y);
        void OnXButton2Released(int x, int y);
        void OnWheelUp(int x, int y);
        void OnWheelDown(int x, int y);
        void OnMouseMove(int x, int y);
        void OnMouseMoveRaw(int x, int y);

        // Feature detection.
        bool IsConnected() const;

        bool IsLeftDown() const { return is_left_down; }
        bool IsMiddleDown() const { return is_middle_down; }
        bool IsRightDown() const { return is_right_down; }
        bool IsX1Down() const { return is_xbutton1_down; }
        bool IsX2Down() const { return is_xbutton2_down; }

        int GetPositionX() const { return x; }
        int GetPositionY() const { return y; }

        bool EventBufferIsEmpty();
        Event ReadEvent();

        static Mouse& Get();

    private:
        static Mouse* s_mouse;

        std::queue<Event> eventBuffer;
        bool is_left_down = false;
        bool is_middle_down = false;
        bool is_right_down = false;
        bool is_xbutton1_down = false;
        bool is_xbutton2_down = false;
        int x = 0, y = 0;
};