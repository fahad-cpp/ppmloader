#include "Window.h"
Window::Window(const char* name, unsigned int width, unsigned int height,const std::string& iconfile)
{
	mWindowClass = {};
	mWindowClass.lpfnWndProc = windowProcedure;
	mWindowClass.lpszClassName = "ClassName";
	mWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	mWindowClass.style = CS_HREDRAW | CS_VREDRAW;
	
	if(iconfile != "NULL"){
		HINSTANCE instanceHandle = GetModuleHandle(NULL);
		mWindowClass.hIcon = (HICON)LoadImageA(instanceHandle,iconfile.c_str(),IMAGE_ICON,0,0,LR_LOADFROMFILE | LR_DEFAULTSIZE);
	}

	RegisterClassA(&mWindowClass);
	DWORD dwStyle=(WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
	mWindowHandle = CreateWindowA(mWindowClass.lpszClassName, name, dwStyle, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, NULL, this);
	mDeviceContextHandle = GetDC(mWindowHandle);

	renderState.height = height;
	renderState.width = width;
	
	int bufferSize = height * width * sizeof(unsigned int);

	if (renderState.screenBuffer)VirtualFree(renderState.screenBuffer, 0, MEM_RELEASE);
	renderState.screenBuffer = VirtualAlloc(0, bufferSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

	renderState.bitmapInfo.bmiHeader.biSize = sizeof(renderState.bitmapInfo.bmiHeader);
	renderState.bitmapInfo.bmiHeader.biWidth = renderState.width;
	renderState.bitmapInfo.bmiHeader.biHeight = renderState.height;
	renderState.bitmapInfo.bmiHeader.biBitCount = 32;
	renderState.bitmapInfo.bmiHeader.biPlanes = 1;
	renderState.bitmapInfo.bmiHeader.biCompression = BI_RGB;
}
Window::~Window() {
	ReleaseDC(mWindowHandle,mDeviceContextHandle);
	DestroyWindow(mWindowHandle);
}


LRESULT windowProcedure(HWND window, UINT msg, WPARAM wParam, LPARAM lParam) {
	LRESULT result = 0;
	Window* pWindow = nullptr;
	if ((msg != WM_NCCREATE)) {
		pWindow = reinterpret_cast<Window*>(GetWindowLongPtr(window,GWLP_USERDATA));
	}
	switch (msg) {
	case WM_NCCREATE: {
		CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
		pWindow = reinterpret_cast<Window*>(pCreate->lpCreateParams);
		SetWindowLongPtr(window, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
	}break;
	case WM_CLOSE:
	case WM_DESTROY: {
		RenderState* renderStateU = pWindow->getRenderState();
		if (renderStateU->screenBuffer)VirtualFree(renderStateU->screenBuffer, 0, MEM_RELEASE);
		DestroyWindow(window);
		return result;
	}break;
	case WM_SIZE: {
		RECT rect;
		GetWindowRect(window, &rect);
		RenderState* renderStateU = pWindow->getRenderState();
		renderStateU->width = rect.right - rect.left;
		renderStateU->height = rect.bottom - rect.top;

		int bufferSize = renderStateU->width * renderStateU->height * sizeof(unsigned int);

		if (renderStateU->screenBuffer)VirtualFree(renderStateU->screenBuffer, 0, MEM_RELEASE);
		renderStateU->screenBuffer = VirtualAlloc(0, bufferSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

		int dBufferSize = renderStateU->width * renderStateU->height * sizeof(float);

		renderStateU->bitmapInfo.bmiHeader.biSize = sizeof(renderStateU->bitmapInfo.bmiHeader);
		renderStateU->bitmapInfo.bmiHeader.biWidth = renderStateU->width;
		renderStateU->bitmapInfo.bmiHeader.biHeight = renderStateU->height;
		renderStateU->bitmapInfo.bmiHeader.biBitCount = 32;
		renderStateU->bitmapInfo.bmiHeader.biPlanes = 1;
		renderStateU->bitmapInfo.bmiHeader.biCompression = BI_RGB;

		return 0;
	}break;
	default: {
		return DefWindowProcA(window, msg, wParam, lParam);
	}
	}
	return DefWindowProcA(window, msg, wParam, lParam);
}

void Window::processMessages()
{
	MSG message;
	while (PeekMessage(&message, mWindowHandle, 0, 0, PM_REMOVE)) {
		switch (message.message) {
		default: {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		}
	}
}

void Window::swapBuffers()
{
	StretchDIBits(mDeviceContextHandle,0,renderState.height-1,renderState.width,-renderState.height,0,0,renderState.width,renderState.height,renderState.screenBuffer,&renderState.bitmapInfo,DIB_RGB_COLORS,SRCCOPY);
}

void Window::addConsole() const
{
	AllocConsole();
	std::freopen("CONOUT$", "w", stdout);
}

void Window::removeConsole() const {
	FreeConsole();
	std::fclose(stdout);
}
void Window::resize(int width,int height){
	RECT rect;
	GetClientRect(mWindowHandle,&rect);
	SetWindowPos(mWindowHandle,HWND_TOP,0,0,width,height,SWP_NOMOVE | SWP_NOZORDER | SWP_NOREDRAW);
	GetClientRect(mWindowHandle,&rect);
	renderState.height = height;
	renderState.width = width;
	
	int bufferSize = height * width * sizeof(unsigned int);

	if (renderState.screenBuffer)VirtualFree(renderState.screenBuffer, 0, MEM_RELEASE);
	renderState.screenBuffer = VirtualAlloc(0, bufferSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

	renderState.bitmapInfo.bmiHeader.biSize = sizeof(renderState.bitmapInfo.bmiHeader);
	renderState.bitmapInfo.bmiHeader.biWidth = renderState.width;
	renderState.bitmapInfo.bmiHeader.biHeight = renderState.height;
	renderState.bitmapInfo.bmiHeader.biBitCount = 32;
	renderState.bitmapInfo.bmiHeader.biPlanes = 1;
	renderState.bitmapInfo.bmiHeader.biCompression = BI_RGB;
}
void Window::hideWindow(bool hide){
	if(hide){
		ShowWindow(mWindowHandle,SW_HIDE);
	}else{
		ShowWindow(mWindowHandle,SW_SHOW);
	}
}