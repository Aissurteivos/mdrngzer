#include "rom.h"
#include <fstream>

ROM::ROM(const std::string &filePath) {
    std::ifstream file(filePath);
    file.seekg(0, std::ios::end);
    memory.resize(file.tellg());
    file.seekg(0, std::ios::beg);
    
    file.read((char*)memory.data(), memory.size());
}

void ROM::save(const std::string &filePath) {
    std::ofstream file(filePath);
    file.write((char*)memory.data(), memory.size());
}

