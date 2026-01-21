#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <sstream>


typedef unsigned int u32;
typedef unsigned char u8;
struct size2{
    u32 x,y;
};
void loadPPM(const std::string filename,u32** pbuffer,size2* res);
