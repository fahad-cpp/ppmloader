#pragma once
#include <cmath>
#include "Window.h"
//CPU renderer
struct Renderer {
	Window* renderWindow;
	Renderer(Window* window) {
		renderWindow = window;
	}
	void clear(u32 colour);
	void putPixel(int x, int y, u32 color);
	void drawBuffer(u32* buffer,size2 dimension);
};
template<typename T>
void clamp(T& num, T min_limit, T max_limit);
