#include "Renderer.h"
template<typename T>
void clamp(T& num, T min_limit, T max_limit) {
	if (num < min_limit) {
		num = min_limit;
		return;
	}
	else if (num > max_limit) {
		num = max_limit;
		return;
	}
	return;
}

void Renderer::putPixel(int x, int y, u32 color) {
	u32* pixel = (u32*)renderWindow->renderState.screenBuffer + x + (y * renderWindow->renderState.width);
	if((x < 0 || x > renderWindow->renderState.width) || (y < 0 || y > renderWindow->renderState.height))
		std::cout<<"Null pixel";
	else{
		*pixel = color;
	}
}
void Renderer::clear(u32 color) {
	u32* buffer = (u32*)renderWindow->renderState.screenBuffer;
	int bufferSize = (renderWindow->renderState.width * renderWindow->renderState.height);
	for(int i=0;i<bufferSize;i++){
		buffer[i] = color;
	}
}
void Renderer::drawBuffer(u32* buffer,size2 dimension){
	if((renderWindow->renderState.width < dimension.x) || (renderWindow->renderState.height < dimension.y)){
		renderWindow->resize(dimension.x,dimension.y);
	}
	for(int y=0;y<dimension.y;y++){
		for(int x=0;x<dimension.x;x++){
			if((x < 0 || x > renderWindow->renderState.width) || (y < 0 || y > renderWindow->renderState.height)){
				continue;
			}
			u32* color = buffer + x + (y * dimension.x);
			putPixel(x,y,*color);
		}
	}
}
