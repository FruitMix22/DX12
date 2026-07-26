// Includes al  Win32 API stuff
#include <Windows.h>

// Main window handle; tthis is used to identify
// a created window (window ID)
HWND ghMainWnd = 0;

// Window initialisation
/// <returns>True if the window was created successfully, otherwise false.</returns>
bool InitWindowsApp(HINSTANCE instanceHandle, int show);

// Wraps message loop code
int Run();

// Handles events the window receives 
LRESULT CALLBACK
WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Fancy windows version of main
int WINAPI
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nShowCmd)
{
	// Initialise the main application window
	// If fails, exit out
	if (!InitWindowsApp(hInstance, nShowCmd))
		return 0;

	// Once created and initialised, run the message loop
	// Exits once WM_QUIT is received
	return Run();
}

bool InitWindowsApp(HINSTANCE instanceHandle, int show)
{
	// describe window characteristics
	WNDCLASS wc;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = instanceHandle;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = L"BasicWncClass";

	// Register the WNDCLASS instance with windows, so we can 
	// create a window with it
	if (!RegisterClass(&wc))
	{
		MessageBox(0, L"RegisterClass FAILED", 0, 0);
		return false;
	}

	// Now, create a window with the WNDCLASS that has been registered
	// Will return a HWND (window handle)
	ghMainWnd = CreateWindow(
		L"BasicWndClass",	// registered WNDCLASS instance to use
		L"Win32Basic",	// window title
		WS_OVERLAPPEDWINDOW,	// style flag
		CW_USEDEFAULT,	// x
		CW_USEDEFAULT,	// y
		CW_USEDEFAULT,	// width
		CW_USEDEFAULT,	// height
		0,	// parent window
		0,	// menu handle
		instanceHandle,	// app instance
		0); // extra stuff?

	if (ghMainWnd == 0)
	{
		MessageBox(0, L"CreateWindow FAILED", 0, 0);
		return false;
	}

	// Now we can show the window!!
	ShowWindow(ghMainWnd, show);
	UpdateWindow(ghMainWnd);

	// if all is good, return true!
	return true;
}

int Run()
{
	MSG msg = {0};

	// Loop until we get WM_QUIT
	// Will return -1 if theres an error
	BOOL bRet = 1;
	while (bRet = GetMessage(&msg, 0, 0, 0) != 0)
	{
		if (bRet == -1)
		{
			MessageBox(0, L"GetMessage FAILED", L"Error", MB_OK);
			break;
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK		
WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Handle window messages, return 0 if so
	switch (msg)
	{
		// If left mouse button is pressed, display a message
	case WM_LBUTTONDOWN:
		MessageBox(0, L"If you're seeing this, i didnt blow up", L"Howdy", MB_OK);
		return 0;

		// If esc is pressed, destroy the window
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			DestroyWindow(ghMainWnd);
		return 0;

		// If a destroy message (pressing X or alt+f4) , send a quit message
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}