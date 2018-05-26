//------------------------------------
// By Jishnu Girish
//------------------------------------
#include "UserInputHandler.h"

Key KeyInput::s_keys[256] = {};

//-----------------------------------------------
// Initialize the state of all keys
//-----------------------------------------------
KeyInput::KeyInput()
{
	for (int i = 0; i < 256; i++)
	{
		KeyInput::s_keys[i].down = false;
		KeyInput::s_keys[i].released = false;
		KeyInput::s_keys[i].pressed = false;
	}
}

//-----------------------------------------------------------
// Checks is the key state is down and returns true if it is
//-----------------------------------------------------------
bool KeyInput::IsKeyDown(uint32_t keyCode)
{
	return s_keys[keyCode].down;
}

//-----------------------------------------------------------
// Checks is the key state is pressed and returns true if it is
//-----------------------------------------------------------
bool KeyInput::WasKeyPressed(uint32_t keyCode)
{
	return s_keys[keyCode].pressed;
}

//-----------------------------------------------------------
// Checks is the key state is released and returns true if it is
//-----------------------------------------------------------
bool KeyInput::WasKeyReleased(uint32_t keyCode)
{
	return s_keys[keyCode].released;
}


//-----------------------------------------------------------
// Sets the key states to down, pressed or released
//-----------------------------------------------------------
void KeyInput::SetKey(uint32_t keyCode, bool isDown, bool isReleased, bool isPressed)
{
	s_keys[keyCode].down = isDown;
	s_keys[keyCode].released = isReleased;
	s_keys[keyCode].pressed = isPressed;
}


//-----------------------------------------------------------
// Resets the key states
//-----------------------------------------------------------
void KeyInput::ResetKeys()
{
	for (int i = 0; i < 256; i++)
	{
		s_keys[i].released = false;
		s_keys[i].pressed = false;
	}
}

//-----------------------------------------------------------
//Returns the currently pressed key as a wide string (for the debug window)
//-----------------------------------------------------------
std::wstring KeyInput::GetKeyPressedWString() {
	std::wstring key = L"";

#pragma region GetKeys
	if (s_keys[KEY_A].pressed) {
		key += L'a';
	}
	if (s_keys[KEY_B].pressed) {
		key += L'b';
	}
	if (s_keys[KEY_C].pressed) {
		key += L'c';
	}
	if (s_keys[KEY_D].pressed) {
		key += L'd';
	}
	if (s_keys[KEY_E].pressed) {
		key += L'e';
	}
	if (s_keys[KEY_F].pressed) {
		key += L'f';
	}
	if (s_keys[KEY_G].pressed) {
		key += L'g';
	}
	if (s_keys[KEY_H].pressed) {
		key += L'h';
	}
	if (s_keys[KEY_I].pressed) {
		key += L'i';
	}
	if (s_keys[KEY_J].pressed) {
		key += L'j';
	}
	if (s_keys[KEY_K].pressed) {
		key += L'k';
	}
	if (s_keys[KEY_L].pressed) {
		key += L'l';
	}
	if (s_keys[KEY_M].pressed) {
		key += L'm';
	}
	if (s_keys[KEY_N].pressed) {
		key += L'n';
	}
	if (s_keys[KEY_O].pressed) {
		key += L'o';
	}
	if (s_keys[KEY_P].pressed) {
		key += L'p';
	}
	if (s_keys[KEY_Q].pressed) {
		key += L'q';
	}
	if (s_keys[KEY_R].pressed) {
		key += L'r';
	}
	if (s_keys[KEY_S].pressed) {
		key += L's';
	}
	if (s_keys[KEY_T].pressed) {
		key += L't';
	}
	if (s_keys[KEY_U].pressed) {
		key += L'u';
	}
	if (s_keys[KEY_V].pressed) {
		key += L'v';
	}
	if (s_keys[KEY_W].pressed) {
		key += L'w';
	}
	if (s_keys[KEY_X].pressed) {
		key += L'x';
	}
	if (s_keys[KEY_Y].pressed) {
		key += L'y';
	}
	if (s_keys[KEY_Z].pressed) {
		key += L'z';
	}
	if (s_keys[KEY_0].pressed) {
		key += L'0';
	}
	if (s_keys[KEY_1].pressed) {
		key += L'1';
	}
	if (s_keys[KEY_2].pressed) {
		key += L'2';
	}
	if (s_keys[KEY_3].pressed) {
		key += L'3';
	}
	if (s_keys[KEY_4].pressed) {
		key += L'4';
	}
	if (s_keys[KEY_5].pressed) {
		key += L'5';
	}
	if (s_keys[KEY_6].pressed) {
		key += L'6';
	}
	if (s_keys[KEY_7].pressed) {
		key += L'7';
	}
	if (s_keys[KEY_8].pressed) {
		key += L'8';
	}
	if (s_keys[KEY_9].pressed) {
		key += L'9';
	}
	if (s_keys[VK_OEM_PERIOD].pressed) {
		key += L'.';
	}
	if (s_keys[KEY_SLASH].pressed) {
		key += L'/';
	}
	if (s_keys[VK_SPACE].pressed) {
		key += L' ';
	}
#pragma endregion

	if (s_keys[VK_SHIFT].down && key != L"") {
		wchar_t c = key[0];
		c = toupper(c);
		key = c;
	}

	return key;
}