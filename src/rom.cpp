#include "rom.h"
#include <fstream>
#include <cstring>
#include <iostream>

void ROM::open(const std::string &filePath) {
    std::ifstream file(filePath);
    file.seekg(0, std::ios::end);
    memory.resize(file.tellg());
    file.seekg(0, std::ios::beg);
    
    file.read((char*)memory.data(), memory.size());
    if (std::strcmp("POKEDUN SORAC2SE01", (char*)memory.data())) {
        throw std::string("ROM: ROM must be Pokemon Mystery Dungeon - Explorers of Sky");
    }
}

void ROM::save(const std::string &filePath) {
    std::ofstream file(filePath);
    file.write((char*)memory.data(), memory.size());
}

