#include <FEngine/App/Input.h>

feng::Input * feng::Input::GetInstance()
{
	static Input inputManager;

	return  &inputManager;
}

void feng::Input::PressKey(unsigned char key, bool isKeyPressed)
{
	m_pressedKey = key;
	m_isKeyPressed = isKeyPressed;
}
