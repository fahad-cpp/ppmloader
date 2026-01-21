#include "PPMLoader.h"

void loadPPM(const std::string filename,u32** pbuffer,size2* res){
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
    if((header.find("P3") == std::string::npos) && (header.find("P6") == std::string::npos)){
        std::cerr << "Invalid PPM file : " << filename << '\n';
        return;
    }
    
    sstr >> width >> height;
    sstr >> channelSize;
    
    if((channelSize > 65536U) || (channelSize == 0)){
        std::cerr << "Unsupported channel size: "<<channelSize << "\n";
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
}
