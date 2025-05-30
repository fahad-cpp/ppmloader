#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include "Window.h"
#include "Renderer.h"
typedef unsigned int u32;
typedef unsigned char u8;
void loadPPM(const std::string filename,u32** pbuffer,size2* res){
    
    unsigned int width = 0;
    unsigned int height = 0;
    unsigned int channelSize = 0;
    std::ifstream ifs;
    ifs.open(filename);
    if(!ifs.is_open()){
        std::cerr << "Failed to open file "<<filename<<"\n";
        return;
    }
    std::string line;
    std::getline(ifs,line);
    if(line.find("P3") == std::string::npos){
        std::cerr << "The file "<<filename <<" is not a valid ppm file\n";
        MessageBoxA(NULL,"Invalid PPM file","ERROR: INVALID PPM",MB_OK);
        exit(0);
        return;
    }
    ifs >> width >> height;
    //std::cout << "Width:"<<width<<" Height:"<<height<<"\n";
    
    ifs >> channelSize;
    //std::cout<<"channelSize:"<<channelSize<<"\n";
    int bufferSize = width * height;
    *pbuffer = (u32*)malloc(bufferSize*sizeof(u32));
    for(u32 y=0;y<height;y++){
        for(u32 x = 0;x<width;x++){
            u32 color=0;
            u32 R=0,G=0,B=0;
            ifs >> R >> G >> B;
            color = u32((R << 16) | (G << 8) | (B));
            (*pbuffer)[(y*width)+x] = color;
        }
    }
    std::cout<< "Successfully loaded ppm : "<< filename <<"\n";
    *res = {width,height};
}

int main(int argc,char* argv[]){
    if(argc != 2){
        std::cout<<"Usage : ppmloader.exe x.ppm\n";
        return 0;
    }else{
        int filecount = argc - 1;
        u32* imageBuffer = nullptr;
        std::string filename = argv[1];
        size2 buffersize;
        bool success;
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
            Sleep(5);
        }
        free(imageBuffer);
    }
}