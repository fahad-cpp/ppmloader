#ifndef RENDERER
#define RENDERER
#include "Window.h"
#include "PPMLoader.h"

//CPU renderer
struct Renderer {
	Window* renderWindow;
	Renderer(Window* window) {
		renderWindow = window;
	}
	void clear(u32 colour);
	void putPixel(uint32_t x, uint32_t y, u32 color);
	void drawBuffer(u32* buffer,size2 dimension);
};
template<typename T>
void clamp(T& num, T min_limit, T max_limit);
#endif