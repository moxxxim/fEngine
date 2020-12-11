#pragma once

namespace feng
{
    class Input final
    {
    public:
        static Input * GetInstance();

        Input(const Input& manager) = delete;
        Input& operator =(const Input& manager) = delete;

        inline bool IsKeyPressed() const;
        inline unsigned char GetPressedKey() const;
        void PressKey(unsigned char key, bool bIsPressed);

    private:
        Input() = default;
        ~Input() = default;

        bool m_isKeyPressed = false;
        unsigned char m_pressedKey;
    };

    inline bool Input::IsKeyPressed() const
    {
        return m_isKeyPressed;
    }

    inline unsigned char Input::GetPressedKey() const
    {
        return m_pressedKey;
    }
}
