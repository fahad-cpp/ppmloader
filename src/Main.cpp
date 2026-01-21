#include "PPMLoader.h"
#include "Renderer.h"

int main(int argc,char* argv[]){
    if(argc != 2){
        std::cout << "Usage: ppmloader.exe x.ppm\n";
        return 0;
    }

    u32* imageBuffer = nullptr;
    std::string filename = argv[1];
    size2 buffersize;

    std::thread ppmThread(loadPPM,filename,&imageBuffer,&buffersize);
    std::string windowName = "PPMLoader - "+filename;
    Window window(windowName.c_str(),0,0,"icon.ico");
    window.removeConsole();
    Renderer renderer(&window);
    ppmThread.join();
    window.resize(buffersize.x,buffersize.y);
    window.hideWindow(false);
    while(window.isOpen()){
        renderer.clear(0x000000);
        window.processMessages();
        renderer.drawBuffer(imageBuffer,buffersize);
        window.swapBuffers();
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    free(imageBuffer);
    return 0;
}