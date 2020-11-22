#include <FEngine/App/Input.h>

fengine::Input * fengine::Input::GetInstance()
{
	static Input inputManager;

	return  &inputManager;
}

void fengine::Input::PressKey(unsigned char key, bool isKeyPressed)
{
	m_pressedKey = key;
	m_isKeyPressed = isKeyPressed;
}
