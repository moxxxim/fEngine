#pragma once

#include <cstdint>

namespace Feng
{
    struct InputKey final
    {
        enum eKey : uint32_t
        {
            Unknown = 0,

            // Printable keys.
            Kb_Space,
            Kb_Apostrophe, /* ' */
            Kb_Coma, /* , */
            Kb_Minus, /* - */
            Kb_Period, /* . */
            Kb_Slash, /* / */

            Kb_0,
            Kb_1,
            Kb_2,
            Kb_3,
            Kb_4,
            Kb_5,
            Kb_6,
            Kb_7,
            Kb_8,
            Kb_9,

            Kb_Semicolon, /* ; */
            Kb_Equal, /* = */

            Kb_A,
            Kb_B,
            Kb_C,
            Kb_D,
            Kb_E,
            Kb_F,
            Kb_G,
            Kb_H,
            Kb_I,
            Kb_J,
            Kb_K,
            Kb_L,
            Kb_M,
            Kb_N,
            Kb_O,
            Kb_P,
            Kb_Q,
            Kb_R,
            Kb_S,
            Kb_T,
            Kb_U,
            Kb_V,
            Kb_W,
            Kb_X,
            Kb_Y,
            Kb_Z,

            Kb_Left_Bracket, /* [ */
            Kb_BackSlash, /* \ */
            Kb_Right_Bracket, /* ] */
            Kb_Grave_Accent, /* ` */
            Kb_World_1, /* non-US #1 */
            Kb_World_2, /* non-US #2 */

            // Function keys.
            Kb_Escape,
            Kb_Enter,
            Kb_Tab,
            Kb_Backspace,
            Kb_Insert,
            Kb_Delete,
            Kb_Right,
            Kb_Left,
            Kb_Down,
            Kb_Up,
            Kb_PageUp,
            Kb_PageDown,
            Kb_Home,
            Kb_End,
            Kb_CapsLock,
            Kb_ScrollLock,
            Kb_NumLock,
            Kb_PrintScreen,
            Kb_Pause,
            Kb_F1,
            Kb_F2,
            Kb_F3,
            Kb_F4,
            Kb_F5,
            Kb_F6,
            Kb_F7,
            Kb_F8,
            Kb_F9,
            Kb_F10,
            Kb_F11,
            Kb_F12,
            Kb_F13,
            Kb_F14,
            Kb_F15,
            Kb_F16,
            Kb_F17,
            Kb_F18,
            Kb_F19,
            Kb_F20,
            Kb_F21,
            Kb_F22,
            Kb_F23,
            Kb_F24,
            Kb_F25,
            Kb_Kp_0,
            Kb_Kp_1,
            Kb_Kp_2,
            Kb_Kp_3,
            Kb_Kp_4,
            Kb_Kp_5,
            Kb_Kp_6,
            Kb_Kp_7,
            Kb_Kp_8,
            Kb_Kp_9,
            Kb_Kp_Decimal,
            Kb_Kp_Divide,
            Kb_Kp_Multiply,
            Kb_Kp_Subtract,
            Kb_Kp_Add,
            Kb_Kp_Enter,
            Kb_Kp_Equal,
            Kb_Left_Shift,
            Kb_Left_Control,
            Kb_Left_Alt,
            Kb_Left_Super,
            Kb_Right_Shift,
            Kb_Right_Control,
            Kb_RightAlt,
            Kb_RightSuper,
            Kb_Menu,
            
            Mouse_Btn_1,
            Mouse_Btn_2,
            Mouse_Btn_3,
            Mouse_Btn_4,
            Mouse_Btn_5,
            Mouse_Btn_6,
            Mouse_Btn_7,
            Mouse_Btn_8,
            Mouse_Btn_Last = Mouse_Btn_8,
            Mouse_Btn_Left = Mouse_Btn_1,
            Mouse_Btn_Right = Mouse_Btn_2,
            Mouse_Btn_Middle = Mouse_Btn_3,

            Count,
        };
    };
    
    enum class eKeyStatus : uint8_t
    {
        Released,
        Pressed,
    };
}
