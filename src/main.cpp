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