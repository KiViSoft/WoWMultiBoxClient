
#include "WindowManager.h"

Window::Window(HWND win) : mWindow(win)
{
}

HWND Window::getWindowHandle() const
{
	return mWindow;
}

void WindowHelper::ClientManager::BroadcastKeyPress(UINT uMsg, WPARAM wParam, LPARAM lParam, Protocol proto)
{
	for (const auto& c : mClients)
	{
		if (proto == Protocol::POST)
		{
			PostMessage(c.getWindow().getWindowHandle(), uMsg, wParam, lParam);
		}
		else if (proto == Protocol::SEND)
		{
			SendMessage(c.getWindow().getWindowHandle(), uMsg, wParam, lParam);
		}
	}
}

LRESULT WindowHelper::ClientManager::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	ClientManager* mngr = reinterpret_cast<ClientManager*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	return mngr->WindowMessageHandler(hwnd, uMsg, wParam, lParam);
}

BOOL CALLBACK WindowHelper::ClientManager::FindClientWindowHandles(HWND hwnd, LPARAM lparam)
{
	ClientManager* mngr = reinterpret_cast<ClientManager*>(lparam);
	TCHAR window_caption[64];

	GetWindowText(hwnd, window_caption, 64);

	if (wcscmp(window_caption, mngr->mClientName.c_str()) == 0)
	{
		TCHAR window_class[64];
		GetClassName(hwnd, window_class, 64);
		if (wcscmp(window_class, mngr->mClientClass.c_str()) == 0)
		{
			mngr->mClients.emplace_back(Client(hwnd));

			// placeholder support for two windows to stop enumeration early
			if (mngr->mClients.size() >= 2)
			{
				return FALSE;
			}
		}
	}
}

LRESULT CALLBACK WindowHelper::ClientManager::WindowMessageHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

		EndPaint(hwnd, &ps);
	}

	case WM_KEYDOWN:
	{
			// press p to quit
			if (wParam == 0x50)
			{
				PostQuitMessage(0);
			}
			else
			{
				BroadcastKeyPress(uMsg, wParam, lParam, Protocol::SEND);
			}

			return 0;
		}

		case WM_KEYUP:
		{
			BroadcastKeyPress(uMsg, wParam, lParam, Protocol::SEND);
			return 0;
		}

		case WM_NCHITTEST:
		{
			BroadcastKeyPress(uMsg, wParam, lParam, Protocol::SEND);
			return HTCLIENT;
		}

		case WM_SETCURSOR:
		{
			BroadcastKeyPress(uMsg, wParam, lParam, Protocol::SEND);
			return TRUE;
		}

		case WM_MOUSEMOVE:
		{
			BroadcastKeyPress(uMsg, wParam, lParam, Protocol::POST);
			return 0;
		}

		case WM_LBUTTONDOWN:
			BroadcastKeyPress(WM_NCHITTEST, 0, lParam, Protocol::SEND);
			BroadcastKeyPress(uMsg, wParam, lParam, Protocol::POST);
			BroadcastKeyPress(WM_LBUTTONUP, wParam, lParam, Protocol::POST);
			return 0;

		case WM_RBUTTONDOWN:
			BroadcastKeyPress(WM_NCHITTEST, 0, lParam, Protocol::SEND);
			BroadcastKeyPress(uMsg, wParam, lParam, Protocol::POST);
			BroadcastKeyPress(WM_RBUTTONUP, wParam, lParam, Protocol::POST);

		case WM_ACTIVATE:
		{
			if (lParam == 0)
			{
				SetForegroundWindow(hwnd);
				SetFocus(hwnd);
			}
		}

		case WM_NCACTIVATE:
			ShowWindow(hwnd, SW_RESTORE);
			SetFocus(hwnd);
			return 0;

		case WM_ACTIVATEAPP:
			SetForegroundWindow(hwnd);
			SetFocus(hwnd);
			return 0;

		case WM_KILLFOCUS:
		{
			SetForegroundWindow(hwnd);
			SetFocus(hwnd);
		}

		case WM_CAPTURECHANGED:
		{
			SetForegroundWindow(hwnd);
			SetFocus(hwnd);
			return 0;
		}

		case WM_WINDOWPOSCHANGING:
		{
			SetForegroundWindow(hwnd);
			SetFocus(hwnd);
			return 0;
		}

	}


	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

WindowHelper::ClientManager::ClientManager(std::wstring client_name, std::wstring client_class) : mClientName(client_name), mClientClass(client_class)
{
}

void WindowHelper::ClientManager::initializeClients()
{
	EnumWindows(FindClientWindowHandles, reinterpret_cast<LPARAM>(this));
}

Client& WindowHelper::ClientManager::getClient1()
{
	if (!mClients.empty())
	{
		return mClients.at(0);
	}
}

 Client& WindowHelper::ClientManager::getClient2()
{
	if (mClients.size() > 1)
	{
		return mClients.at(1);
	}
}

Client::Client(HWND win) : mClientWindow(Window(win)), mUp(KEY_W), mDown(KEY_S), mLeft(KEY_A), mRight(KEY_D)
{
}

void Client::setWindow(HWND win)
{
	mClientWindow = Window(win);
}

void Client::setKeyBindings(KeyBind up, KeyBind down, KeyBind left, KeyBind right)
{
	mUp = up;
	mDown = down;
	mLeft = left;
	mRight = right;
}

Window Client::getWindow() const
{
	return mClientWindow;
}
