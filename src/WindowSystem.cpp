#include <krimo/WindowSystem.hpp>
#include <krimo/krimo.hpp>
#include <iostream>


#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace krimo {
namespace impl {

class WindowClassInternals {
public:
	HWND hwnd;
};

}
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if(uMsg == WM_CREATE) {
		const CREATESTRUCTW* createStruct = reinterpret_cast<CREATESTRUCTW*>(lParam);
		SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(createStruct->lpCreateParams));
	}
	krimo::WindowSystem* ws = reinterpret_cast<krimo::WindowSystem*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	if(uMsg == WM_CLOSE){
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

void krimo::WindowSystem::OnAttach(){
	internals = new impl::WindowClassInternals;

	const wchar_t kClassName[] = L"Krimo Class";
	const wchar_t kWindowName[] = L"Krimo Application";

	const HINSTANCE hInstance = GetModuleHandleW(NULL);

	WNDCLASSW windowClass {
		.lpfnWndProc = WindowProc,
		.hInstance = hInstance,
		.lpszClassName = kClassName
	};

	RegisterClassW(&windowClass);

	internals->hwnd = CreateWindowExW(
		0, 								// exStyle
		kClassName,						// class name
		kWindowName,					// window name
		WS_OVERLAPPEDWINDOW,			// style
		CW_USEDEFAULT, CW_USEDEFAULT, 	// x, y
		CW_USEDEFAULT, CW_USEDEFAULT, 	// w, h
		NULL, 							// parent,
		NULL,							// menu
		hInstance,						// hInstance
		this
	);

	ShowWindow(internals->hwnd, SW_SHOW);
}

void krimo::WindowSystem::OnUpdate(){
	MSG msg;
	while(PeekMessageW(&msg, NULL, 0, 0, TRUE)) {
		if(msg.message == WM_QUIT){
			krimo->Quit();
			return;
		}
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
}

void krimo::WindowSystem::OnDetach(){
	delete internals;
}