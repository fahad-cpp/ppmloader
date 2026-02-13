#include "PPMLoader.h"
#include "Timer.h"
#include <algorithm>
std::vector<std::string> split(const std::string& str, char del) {
    int count = (std::count(str.begin(), str.end(), del) + 1);
    int strsize = str.length();
    std::vector<std::string> output = {};
    output.reserve(count);
    int beg = 0;
    int end = 0;
    const char* ptr = str.c_str();

    for (int i = 0;i <= strsize;i++) {
        if (*ptr == del) {
            if (beg < end)
                output.push_back(std::string(str.begin() + beg, str.begin() + end));
            beg = end + 1;
        }
        else if (i == strsize) {
            output.push_back(std::string(str.begin() + beg, str.begin() + end));
            break;
        }
        end++;
        ptr++;
    }
    return output;
}
std::vector<std::string> getTokens(char* str, size_t size) {
    std::vector<std::string> output;
    char* ptr = str;
    char* beg = ptr;
    char* end = ptr;

    for (size_t i = 0; i < size; i++) {
        if (*ptr == ' ' || *ptr == '\n' || *ptr == '\t') {
            if (beg < end) {
                output.push_back(std::string(beg, end - beg));
            }

            ptr++;
            beg = ptr;
            end = ptr;
            continue;
        }

        ptr++;
        end++;
    }
    if (beg < end) {
        output.push_back(std::string(beg, end - beg));
    }

    return output;
}

void loadPPM(const std::string filename, u32** pbuffer, size2* res) {
    unsigned int width = 0;
    unsigned int height = 0;
    unsigned int channelSize = 0;

    std::ifstream ifs;
    ifs.open(filename,std::ios::binary | std::ios::ate);
    if (!ifs.is_open()) {
        std::cerr << "[PPMLoader] : Failed to open " << filename << "\n";
        return;
    }
    size_t filesize = ifs.tellg();
    ifs.seekg(0);
    
    std::vector<char> wholeFile = {};
    wholeFile.reserve(filesize);
    ifs.read(wholeFile.data(), filesize);
    ifs.close();
    std::vector<std::string> tokens = getTokens(wholeFile.data(),filesize);
    if ((tokens[0].find("P3") == std::string::npos) && (tokens[0].find("P6") == std::string::npos)) {
        std::cerr << "Invalid PPM file : " << filename << '\n';
        pbuffer = nullptr;
        return;
    }
    width = std::stoi(tokens[1]);
    height = std::stoi(tokens[2]);

    channelSize = std::stoi(tokens[3]);


    if ((channelSize > 65536U) || (channelSize == 0)) {
        std::cerr << "Unsupported channel size: " << channelSize << "\n";
        return;
    }


    size_t bufferSize = width * height;
    *pbuffer = (u32*)malloc(bufferSize * sizeof(u32));
    //std::cout << "Tokens Size:" << tokens.size() << "\n";
    for (int i = 4;i < tokens.size();i += 3) {
        
        u32 color = 0;
        u32 R, G, B;
        R = (u32)std::atoi(tokens[i].c_str());
        G = (u32)std::atoi(tokens[i+1].c_str());
        B = (u32)std::atoi(tokens[i+2].c_str());
        color = u32((R << 16) | (G << 8) | (B));
        (*pbuffer)[(i-4)/3] = color;
    }
    *res = { width,height };
}
