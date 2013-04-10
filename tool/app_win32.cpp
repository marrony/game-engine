#include "app.h"

#ifdef WIN32

void Application::initialize_variables() {
	window = 0;
}

void Application::swap_buffers() {
//	SwapBuffers(hdc);
}

void Application::initialize() {
	WNDCLASSEX wcx;
	wcx.cbSize = sizeof(wcx);
	wcx.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcx.lpfnWndProc = DefWindowProc;
	wcx.cbClsExtra = 0;
	wcx.cbWndExtra = sizeof(Application*);
	wcx.hInstance = GetModuleHandle(NULL);
	wcx.hIcon = NULL;
	wcx.hCursor = NULL;
	wcx.hbrBackground = NULL;
	wcx.lpszMenuName = NULL;
	wcx.lpszClassName = "WindowClass";
	wcx.hIconSm = NULL;

	RegisterClassEx(&wcx);

	window = CreateWindowEx(0,
			"WindowClass",
			title,
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			width,
			height,
			HWND_DESKTOP,
			(HMENU) NULL,
			GetModuleHandle(NULL),
			(LPVOID)NULL);

	SetWindowLongPtr(window, 0, (LONG_PTR)this);
	SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)MainWndProc);

	ShowWindow(window, SW_SHOW);
}

void Application::finalize() {
	DestroyWindow(window);
//	wglMakeCurrent(hdc, NULL);
//	wglDeleteContext(hglrc);
//	ReleaseDC(hwnd, hdc);
}

static AppKeyCode get_app_keycode(WPARAM wParam) {
	if(wParam >= 0x30 && wParam <= 0x39)
	return (AppKeyCode)(KEY_0 + (wParam - 0x30));

	if(wParam >= 0x41 && wParam <= 0x5A)
	return (AppKeyCode)(KEY_A + (wParam - 0x41));

	if(wParam >= VK_NUMPAD0 && wParam <= VK_DIVIDE)
	return (AppKeyCode)(KEY_NUMPAD_0 + (wParam - VK_NUMPAD0));

	if(wParam >= VK_F1 && wParam <= VK_F12)
	return (AppKeyCode)(KEY_F1 + (wParam - VK_F1));

	switch(wParam) {
		case VK_LBUTTON: return KEY_LBUTTON;
		case VK_RBUTTON: return KEY_RBUTTON;
		case VK_MBUTTON: return KEY_MBUTTON;
		//case VK_XBUTTON1: return KEY_XBUTTON1;
		//case VK_XBUTTON2: return KEY_XBUTTON2;
		case VK_BACK: return KEY_BACKSPACE;
		case VK_TAB: return KEY_TAB;
		case VK_RETURN: return KEY_ENTER;
		case VK_ESCAPE: return KEY_ESCAPE;
		case VK_SPACE: return KEY_SPACE;
		case VK_PRIOR: return KEY_PAGE_UP;
		case VK_NEXT: return KEY_PAGE_DOWN;
		case VK_END: return KEY_END;
		case VK_HOME: return KEY_HOME;
		case VK_INSERT: return KEY_INSERT;
		case VK_DELETE: return KEY_DELETE;
		case VK_LSHIFT: return KEY_LEFT_SHIFT;
		case VK_LMENU: return KEY_LEFT_ALT;
		case VK_LCONTROL: return KEY_LEFT_CONTROL;
		case VK_LWIN: return KEY_LEFT_WIN;
		case VK_RSHIFT: return KEY_RIGHT_SHIFT;
		case VK_RMENU: return KEY_RIGHT_ALT;
		case VK_RCONTROL: return KEY_RIGHT_CONTROL;
		case VK_RWIN: return KEY_RIGHT_WIN;
		case VK_CAPITAL: return KEY_CAPS_LOCK;
		case VK_LEFT: return KEY_LEFT;
		case VK_UP: return KEY_UP;
		case VK_RIGHT: return KEY_RIGHT;
		case VK_DOWN: return KEY_DOWN;
		case VK_SNAPSHOT: return KEY_PRINT_SCREEN;
		case VK_SCROLL: return KEY_SCROLL_LOCK;
		case VK_PAUSE: return KEY_PAUSE;
	}

	return KEY_NONE;
}

LRESULT CALLBACK Application::MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	Application* app = (Application*) GetWindowLongPtr(hwnd, 0);

	switch(msg) {
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;

	case WM_KEYUP:
		app->on_key_up(get_app_keycode(wParam));
		return 0;

	case WM_KEYDOWN:
		app->on_key_down(get_app_keycode(wParam));
		return 0;

	case WM_SIZE:
		app->on_resize(LOWORD(lParam), HIWORD(lParam));
		return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

void Application::process_events() {
	MSG msg;

	if(PeekMessage(&msg, (HWND)NULL, 0, 0, PM_REMOVE) != 0) {
		if(msg.message == WM_QUIT) {
			running = false;
			return;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

#endif //WIN32
