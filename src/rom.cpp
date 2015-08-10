#include "rom.h"
#include <fstream>
#include <cstring>
#include <iostream>

const uint8_t choosablePokemon[][2] = {
    {0x10, 0x00},
};

const uint8_t unreplacablePokemon[][2] = {
    {0x29, 0x02},
    {0x00, 0x00},
};

ROM::ROM(unsigned seed) : rand(seed) {
}

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

void ROM::randPokemon() {
    for (unsigned i = 0; i != 14229; i++) {
        uint8_t *entry = memory.data() + 0x003EB1D0 + i * 8;
        //Check for every unchoosable pokemon type
        bool finish = false;
        for (auto unreplace : unreplacablePokemon) {
            //If it matches
            if (!std::memcmp(entry + 6, unreplace, 2)) {
                //Dont randomize this pokemon
                finish = true;
                break;
            }
        }
        //If this is an unreplacable, finish here
        if (finish)
            continue;
        
        //Choose the pokemon to use
        unsigned choice = rand() % (sizeof(choosablePokemon) / sizeof(choosablePokemon[0]));
        
        memcpy(entry + 6, choosablePokemon[choice], 2);
    }
}

