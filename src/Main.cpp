#include "PPMLoader.h"
#include "Renderer.h"
#include <thread>
#include <iostream>
#include "Window.h"
int
WinMain([[maybe_unused]]HINSTANCE hInstance,[[maybe_unused]] HINSTANCE hPrevInstance,[[maybe_unused]] LPSTR lpCmdLine,[[maybe_unused]] int nShowCmd){
    int argc;

    wchar_t** argv = CommandLineToArgvW(
        GetCommandLineW(),
        &argc
    );
    if(argc != 2){
        std::cout << "Usage: ppmloader.exe x.ppm\n";
        return 0;
    }
    u32* imageBuffer = nullptr;
    std::wstring ws = argv[1];
    std::string filename = std::string(ws.begin(),ws.end());
    size2 buffersize;

    std::thread ppmThread(loadPPM,filename,&imageBuffer,&buffersize);
    
    std::string windowName = "PPMLoader - "+filename;
    Window window(windowName.c_str(),0,0,"res\\icon.ico");
    window.removeConsole();
    Renderer renderer(&window);
    ppmThread.join();
    if(imageBuffer == nullptr){
        MessageBoxA(window.get(),"Invalid ppm file","ERROR",MB_OK);
        return 1;
    }
    window.resize(buffersize.x,buffersize.y);
    window.hideWindow(false);
    while(window.isOpen()){
        renderer.clear(0x000000);
        renderer.drawBuffer(imageBuffer,buffersize);
        window.swapBuffers();
        window.processMessages();
        std::this_thread::sleep_for(std::chrono::milliseconds(9));
    }

    free(imageBuffer);
    return 0;
}