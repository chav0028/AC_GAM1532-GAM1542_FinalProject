#ifndef __InputEvent_H__
#define __InputEvent_H__

enum InputEventTypes
{
    InputEventType_Key,
    InputEventType_Mouse,
};

enum InputEventState
{
    InputEventState_Down,
    InputEventState_Held,
    InputEventState_Up,
    InputEventState_Max,
};

struct InputEvent
{
    InputEventTypes type;
    InputEventState state;
    union
    {
        int keycode;
        int mousebutton;
    };
    int x;
    int y;

    InputEvent(InputEventTypes atype, InputEventState astate, int akeycode, int ax, int ay)
    {
        type = atype;
        state = astate;
        keycode = akeycode;
        x = ax;
        y = ay;
    }
};

#endif //__InputEvent_H__