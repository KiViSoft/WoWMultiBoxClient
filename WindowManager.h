#pragma once
#define UNICODE
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <string>
#include <vector>

enum KeyBind
{
	KEY_LEFT = 0x25,
	KEY_UP = 0x26,
	KEY_DOWN = 0x27,
	KEY_RIGHT = 0x28,
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
};

enum State
{
	PLAYING = 0,
	PAUSED = 1
};

enum Protocol
{
	POST = 0,
	SEND = 1
};

class Window
{
public:
	explicit Window(HWND win);
	~Window() = default;
	HWND getWindowHandle() const;
private:
	HWND mWindow;
		
};

class Client
{
public:
	Client(HWND win);
	~Client() = default;

	void setWindow(HWND win);
	void setKeyBindings(KeyBind up, KeyBind down, KeyBind left, KeyBind right);
	Window getWindow() const;
private:
	Window mClientWindow;
	KeyBind mUp;
	KeyBind mDown;
	KeyBind mLeft;
	KeyBind mRight;
};

namespace WindowHelper
{
	class ClientManager
	{
	public:
		ClientManager(std::wstring client_name, std::wstring client_class);
		~ClientManager() = default;
		void initializeClients();
		Client& getClient1();
		Client& getClient2();
		static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		std::wstring mClientName;
		std::wstring mClientClass;
		std::vector<Client> mClients;
		void BroadcastKeyPress(UINT uMsg, WPARAM wParam, LPARAM lParam, Protocol proto);
		LRESULT CALLBACK WindowMessageHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		
		static BOOL CALLBACK FindClientWindowHandles(HWND hwnd, LPARAM lparam);
	};
	
	

}

