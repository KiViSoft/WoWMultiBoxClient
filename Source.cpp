#define WIN32_LEAN_AND_MEAN
#define UNICODE 

#include "WindowManager.h" // windows header etc.

DWORD wStyle = WS_BORDER | WS_MAXIMIZE | WS_VISIBLE;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	WindowHelper::ClientManager MultiBox(L"World of Warcraft", L"GxWindowClass");
	MultiBox.initializeClients();
	MultiBox.getClient1().setKeyBindings(KEY_W, KEY_S, KEY_A, KEY_D);
	MultiBox.getClient2().setKeyBindings(KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT);

	const wchar_t CLASS_NAME[] = L"MultiBox";
	WNDCLASS wc = { };

	wc.lpfnWndProc = WindowHelper::ClientManager::WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

	// Get Screen size

	// Create the window.
	HWND hwnd = CreateWindowEx(
		0,                              // Optional window styles.
		CLASS_NAME,                     // Window class
		L"MultiBox overlay",		    // Window text
		wStyle,						     // Window style

		// Size and position
		0, 0, 300, 300,

		NULL,       // Parent window    //HWND_MESSAGE
		NULL,       // Menu
		NULL,  // Instance handle
		NULL        // Additional application data
	);

	if (hwnd == NULL)
	{
		return 0;
	}

	SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG>(&MultiBox));

	SetWindowLong(hwnd, GWL_EXSTYLE,
	GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(hwnd, 0, (255 * 5) / 100, LWA_ALPHA);

	// bring to top
	SetForegroundWindow(hwnd);

	// Run the message loop.
	MSG msg = {};
	while( GetMessage(&msg, NULL, 0,0) )
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}