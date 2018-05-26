#pragma once
#include <Windows.h>
#include <memory>
#include <dinput.h>
#include <Xinput.h>
#include <string>
#define KEYPRESSED(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000 ? 1 : 0))
#define KEYRELEASED(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000 ? 0 : 1))

// The Input handler class will be used for abstracting out Inputs
enum {
	KEY_A = 0x41,
	KEY_B = 0x42,
	KEY_C = 0x43,
	KEY_D = 0x44,
	KEY_E = 0x45,
	KEY_F = 0x46,
	KEY_G = 0x47,
	KEY_H = 0x48,
	KEY_I = 0x49,
	KEY_J = 0x4A,
	KEY_K = 0x4B,
	KEY_L = 0x4C,
	KEY_M = 0x4D,
	KEY_N = 0x4E,
	KEY_O = 0x4F,
	KEY_P = 0x50,
	KEY_Q = 0x51,
	KEY_R = 0x52,
	KEY_S = 0x53,
	KEY_T = 0x54,
	KEY_U = 0x55,
	KEY_V = 0x56,
	KEY_W = 0x57,
	KEY_X = 0x58,
	KEY_Y = 0x59,
	KEY_Z = 0x5A,
	KEY_0 = 0x30,
	KEY_1 = 0x31,
	KEY_2 = 0x32,
	KEY_3 = 0x33,
	KEY_4 = 0x34,
	KEY_5 = 0x35,
	KEY_6 = 0x36,
	KEY_7 = 0x37,
	KEY_8 = 0x38,
	KEY_9 = 0x39,
	KEY_SLASH = 0xBF,
	KEY_GRAVE = 0xC0 //`
	#pragma region incomplete
	// Values below here are wrong
	/*KEY_Enter = 40,
	KEY_Escape = 41,
	KEY_Delete = 42,
	KEY_Tab = 43,
	KEY_Space = 44,
	KEY_Minus = 45,
	KEY_Equals = 46,
	KEY_LeftBracket = 47,
	KEY_RightBracket = 48,
	KEY_Backslash = 49,
	KEY_Semicolon = 51,
	KEY_Quote = 52,
	KEY_Grave = 53,
	KEY_Comma = 54,
	KEY_Period = 55,
	KEY_Slash = 56,
	KEY_CapsLock = 57,
	KEY_F1 = 58,
	KEY_F2 = 59,
	KEY_F3 = 60,
	KEY_F4 = 61,
	KEY_F5 = 62,
	KEY_F6 = 63,
	KEY_F7 = 64,
	KEY_F8 = 65,
	KEY_F9 = 66,
	KEY_F10 = 67,
	KEY_F11 = 68,
	KEY_F12 = 69,
	KEY_PrintScreen = 70,
	KEY_ScrollLock = 71,
	KEY_Pause = 72,
	KEY_Insert = 73,
	KEY_Home = 74,
	KEY_PageUp = 75,
	KEY_DeleteForward = 76,
	KEY_End = 77,
	KEY_PageDown = 78,
	KEY_Right = 79,
	KEY_Left = 80,
	KEY_Down = 81,
	KEY_Up = 82,
	KP_NumLock = 83,
	KP_Divide = 84,
	KP_Multiply = 85,
	KP_Subtract = 86,
	KP_Add = 87,
	KP_Enter = 88,
	KP_1 = 89,
	KP_2 = 90,
	KP_3 = 91,
	KP_4 = 92,
	KP_5 = 93,
	KP_6 = 94,
	KP_7 = 95,
	KP_8 = 96,
	KP_9 = 97,
	KP_0 = 98,
	KP_Point = 99,
	KEY_NonUSBackslash = 100,
	KP_Equals = 103,
	KEY_F13 = 104,
	KEY_F14 = 105,
	KEY_F15 = 106,
	KEY_F16 = 107,
	KEY_F17 = 108,
	KEY_F18 = 109,
	KEY_F19 = 110,
	KEY_F20 = 111,
	KEY_F21 = 112,
	KEY_F22 = 113,
	KEY_F23 = 114,
	KEY_F24 = 115,
	KEY_Help = 117,
	KEY_Menu = 118,
	KEY_LeftControl = 224,
	KEY_LeftShift = 225,
	KEY_LeftAlt = 226,
	KEY_LeftGUI = 227,
	KEY_RightControl = 228,
	KEY_RightShift = 229,
	KEY_RightAlt = 230,
	KEY_RightGUI = 231*/
	#pragma endregion 
};
// Helper class for toggling buttons
class KeyToggle {
public:
	KeyToggle(int key) :mKey{ key }, mActive{ false } {}
	operator bool()
	{
		if (GetAsyncKeyState(mKey))
		{
			if (!mActive)
			{
				mActive = true;
				return true;
			}
		}
		else
			mActive = false;
		return false;
	}
	int mKey;
	bool mActive;
};

// Input handler class
struct Key
{
	bool down;
	bool released;
	bool pressed;
};

class KeyInput 
{
private:
	static Key s_keys[256];
public:
	KeyInput();
	~KeyInput();
	static bool IsKeyDown(uint32_t keyCode);

	static bool WasKeyPressed(uint32_t keyCode);

	static bool WasKeyReleased(uint32_t keyCode);

	static void SetKey(uint32_t keyCode, bool isDown, bool isReleased, bool isPressed);

	static void ResetKeys();

	static std::wstring GetKeyPressedWString();
	
};