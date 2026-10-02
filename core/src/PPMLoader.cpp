#include "PPMLoader.h"
#include <charconv>
#include <vector>
#include <fstream>
#include <iostream>
std::vector<std::string> getTokens(char *str, size_t size) {
    std::vector<std::string> output;
    char                    *ptr = str;
    char                    *beg = ptr;
    char                    *end = ptr;

    for (size_t i = 0; i < size; i++) {
        if (*ptr == ' ' || *ptr == '\n' || *ptr == '\t') {
            if (beg < end) {
                output.emplace_back(beg, end - beg);
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
        output.emplace_back(beg, end - beg);
    }

    return output;
}

void loadPPM(const std::string filename, u32 **pbuffer, size2 *res) {
    unsigned int width       = 0;
    unsigned int height      = 0;
    unsigned int channelSize = 0;

    std::ifstream ifs;
    ifs.open(filename, std::ios::binary | std::ios::ate);
    if (!ifs.is_open()) {
        std::cerr << "[PPMLoader] : Failed to open " << filename << "\n";
        return;
    }
    size_t filesize = ifs.tellg();
    if (!filesize)
        return;
    ifs.seekg(0);

    std::vector<char> wholeFile = {};
    wholeFile.reserve(filesize);
    ifs.read(wholeFile.data(), filesize);
    ifs.close();
    std::vector<std::string> tokens = getTokens(wholeFile.data(), filesize);
    if ((tokens[0].find("P3") == std::string::npos) && (tokens[0].find("P6") == std::string::npos)) {
        std::cerr << "Invalid PPM file : " << filename << '\n';
        return;
    }
    std::from_chars(tokens[1].c_str(), tokens[1].c_str() + tokens[1].length(), width);
    std::from_chars(tokens[2].c_str(), tokens[2].c_str() + tokens[2].length(), height);

    std::from_chars(tokens[3].c_str(), tokens[3].c_str() + tokens[3].length(), channelSize);

    if ((channelSize > 65536U) || (channelSize == 0)) {
        std::cerr << "Unsupported channel size: " << channelSize << "\n";
        return;
    }

    size_t bufferSize = width * height;
    *pbuffer          = (u32 *)malloc(bufferSize * sizeof(u32));
    // std::cout << "Tokens Size:" << tokens.size() << "\n";
    for (uint32_t i = 4; i < tokens.size(); i += 3) {

        u32 color = 0;
        u32 R, G, B;
        std::from_chars(tokens[i].c_str(), tokens[i].c_str() + tokens[i].length(), R);
        std::from_chars(tokens[i + 1].c_str(), tokens[i + 1].c_str() + tokens[i + 1].length(), G);
        std::from_chars(tokens[i + 2].c_str(), tokens[i + 2].c_str() + tokens[i + 2].length(), B);
        color                   = u32((R << 16) | (G << 8) | (B));
        (*pbuffer)[(i - 4) / 3] = color;
    }
    *res = { width, height };
}
