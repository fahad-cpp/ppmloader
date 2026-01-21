#ifndef WINDOW
#define WINDOW
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <iostream>
#include <string>
typedef unsigned int u32;
LRESULT CALLBACK windowProcedure(HWND window,UINT msg,WPARAM wParam,LPARAM lParam);
struct RenderState {
	int width;
	int height;
	void* screenBuffer;
	BITMAPINFO bitmapInfo;
};
//WIN32 window
class Window
{
private:
	HWND mWindowHandle;
	HDC mDeviceContextHandle;
	WNDCLASSA mWindowClass;
public:
	RenderState renderState;
	Window(const char* name, unsigned int width = 720, unsigned int height = 720,const std::string& iconfile  = "NULL");
	~Window();
	void processMessages();
	void resize(int x,int y);
	void swapBuffers();
	void addConsole() const;
	void removeConsole() const;
	void setIcon(const std::string& filename);
	void hideWindow(bool hide = true);
	inline void set(HWND window) { mWindowHandle = window; }
	inline HWND get() const { return mWindowHandle;}
	inline RenderState* getRenderState() { return  &renderState; }
	inline bool isOpen() const { return IsWindow(mWindowHandle); }
};
#endif