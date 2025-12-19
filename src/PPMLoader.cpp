#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <sstream>
#include "Window.h"
#include "Renderer.h"
#include "Timer.h"
typedef unsigned int u32;
typedef unsigned char u8;
//FileStream version
// void loadPPM(const std::string filename,u32** pbuffer,size2* res){
//     Timer timer;
//     unsigned int width = 0;
//     unsigned int height = 0;
//     unsigned int channelSize = 0;
//     std::ifstream ifs;
//     ifs.open(filename);
//     if(!ifs.is_open()){
//         std::cerr << "Failed to open file "<<filename<<"\n";
//         return;
//     }
//     std::string line;
//     std::getline(ifs,line);
//     if(line.find("P3") == std::string::npos){
//         std::cerr << "The file "<<filename <<" is not a valid ppm file\n";
//         MessageBoxA(NULL,"Invalid PPM file","ERROR: INVALID PPM",MB_OK);
//         exit(0);
//         return;
//     }
//     ifs >> width >> height;
//     //std::cout << "Width:"<<width<<" Height:"<<height<<"\n";
    
//     ifs >> channelSize;
//     if (channelSize != 255){
//         std::cerr << "The file "<<filename <<" has unsupported channel size :"<<channelSize<<"\n";
//         MessageBoxA(NULL,"Unsupported PPM file","ERROR: UNSUPPORTED PPM",MB_OK);
//         exit(0);
//         return;
//     }
//     //std::cout<<"channelSize:"<<channelSize<<"\n";
//     int bufferSize = width * height;
//     *pbuffer = (u32*)malloc(bufferSize*sizeof(u32));
//     for(u32 y=0;y<height;y++){
//         for(u32 x = 0;x<width;x++){
//             u32 color=0;
//             u32 R=0,G=0,B=0;
//             ifs >> R >> G >> B;
//             color = u32((R << 16) | (G << 8) | (B));
//             (*pbuffer)[(y*width)+x] = color;
//         }
//     }
//     std::cout<< "Successfully loaded ppm : "<< filename <<"\n";
//     *res = {width,height};
//     timer.Stop();
//     std::cout << "loadPPM took : " << timer.dtms << "ms\n";
// }
//StringStream Version
void loadPPM(const std::string filename,u32** pbuffer,size2* res){
    Timer timer;
    unsigned int width = 0;
    unsigned int height = 0;
    unsigned int channelSize = 0;

    std::ifstream ifs;
    ifs.open(filename, std::ios::binary | std::ios::ate);
    if(!ifs.is_open()){
        std::cerr << "[PPMLoader] : Failed to open "<<filename << "\n";
        return;
    }

    size_t fileSize = ifs.tellg();
    ifs.seekg(0);
    
    std::string wholeFile(fileSize,(char)0);
    ifs.read(&wholeFile[0],fileSize);
    ifs.close();
    
    std::stringstream sstr;
    sstr << wholeFile;
    
    std::string header;
    sstr >> header;
    if(header.find("P3") == std::string::npos){
        std::cerr << "Invalid PPM file : " << filename << '\n';
        return;
    }
    
    sstr >> width >> height;
    sstr >> channelSize;
    
    if(channelSize != 255){
        std::cerr << "Unsupported channel : "<<channelSize << "\n";
        return;
    }

    size_t bufferSize = width * height;
    *pbuffer = (u32*)malloc(bufferSize * sizeof(u32));
    for(size_t i=0;i<bufferSize;i++){
        u32 color = 0;
        u32 R,G,B;
        sstr >> R >> G >> B;
        color = u32((R << 16) | (G << 8) | (B));
        (*pbuffer)[i] = color;
    }
    *res = {width,height};
    timer.Stop();
    std::cout << "loadPPM took : " << timer.dtms << "ms\n";

}
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
        std::this_thread::sleep_for(std::chrono::duration<double,std::milli>(5));
    }

    free(imageBuffer);
    return 0;
}