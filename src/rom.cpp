#include "rom.h"
#include "rom_tables.cpp"
#include <fstream>
#include <cstring>
#include <iostream>
#include <algorithm>

ROM::ROM(unsigned seed) : rand(seed) {
}

void ROM::open(const std::string &filePath) {
    std::ifstream file(filePath, std::ios::binary);
    file.seekg(0, std::ios::end);
    memory.resize(file.tellg());
    file.seekg(0, std::ios::beg);
    
    file.read((char*)memory.data(), memory.size());
    if (std::strcmp("POKEDUN SORAC2SE01", (char*)memory.data())) {
        throw std::string("ROM: ROM must be Pokemon Mystery Dungeon - Explorers of Sky");
    }
}

void ROM::save(const std::string &filePath) {
    std::ofstream file(filePath, std::ios::binary);
    file.write((char*)memory.data(), memory.size());
}

void ROM::randPokemon() {
    std::vector<uint16_t> choosables;
    for (uint16_t i = 0; i != maxPokemonId; i++) {
        if (std::find(std::begin(excludedPokemon), std::end(excludedPokemon), i) == std::end(excludedPokemon))
            choosables.push_back(i);
    }
    for (unsigned i = 0; i != 14229; i++) {
        uint8_t *entry = memory.data() + 0x003EB1D0 + i * 8;
        //Check for every unchoosable pokemon type
        bool finish = false;
        for (uint16_t excludable : excludedPokemon) {
            //If it matches
            if (!std::memcmp(entry + 6, &excludable, 2)) {
                //Dont randomize this pokemon
                finish = true;
                break;
            }
        }
        //If this is an unreplacable, finish here
        if (finish)
            continue;
        
        //Choose the pokemon to use
        unsigned choice = rand() % choosables.size();
        
        memcpy(entry + 6, &choosables[choice], 2);
    }
}

void ROM::randAbilities() {
    std::vector<uint8_t> choosables;
    std::vector<std::pair<unsigned,unsigned>> randAbilityIndex;
    //Make choosable abilitities list
    for (uint8_t i = 0; i != maxAbilityId; i++) {
        if (std::find(std::begin(excludedAbilities), std::end(excludedAbilities), i) == std::end(excludedAbilities))
            choosables.push_back(i);
    }
    //Map abilities to pokemon ID
    for (unsigned i = 0; i != 600; i++) {
        std::pair<unsigned,unsigned> a;
        a.first = rand() % choosables.size();
        a.second = 0;
        //55% chance for second ability
        if ((rand() % 100) < 55) {
            a.second = rand() % choosables.size();
        }
        randAbilityIndex.push_back(a);
    }
    //Assign the values to the pokemon entries
    for (unsigned i = 0; i != 1155; i++) {
        uint8_t *entry = memory.data() + 0x00472808 + i * 68;

        //Get pokemon ID from memory
        uint16_t ID = *(entry+4)+*(entry+5)*256;

        memcpy(entry + 0x18, &choosables[randAbilityIndex[ID].first], 1);
        memcpy(entry + 0x19, &choosables[randAbilityIndex[ID].second], 1);
    }
}

void ROM::randTypes() {
    std::vector<uint8_t> choosables;
    std::vector<std::pair<unsigned,unsigned>> randTypeIndex;
    //Make choosable Types list
    for (uint8_t i = 0; i != maxTypeId; i++) {
        if (std::find(std::begin(excludedTypes), std::end(excludedTypes), i) == std::end(excludedTypes))
            choosables.push_back(i);
    }
    //Map types to pokemon ID
    for (unsigned i = 0; i != 600; i++) {
        std::pair<unsigned,unsigned> a;
        a.first = rand() % choosables.size();
        a.second = 0;
        //40% chance for second Type
        if ((rand() % 100) < 40) {
            a.second = rand() % choosables.size();
        }
        randTypeIndex.push_back(a);
    }
    //Assign the values to the pokemon entries
    for (unsigned i = 0; i != 1155; i++) {
        uint8_t *entry = memory.data() + 0x00472808 + i * 68;

        //Get pokemon ID from memory
        uint16_t ID = *(entry+4)+*(entry+5)*256;

        memcpy(entry + 0x14, &choosables[randTypeIndex[ID].first], 1);
        memcpy(entry + 0x15, &choosables[randTypeIndex[ID].second], 1);
    }
}

void ROM::randIQs() {
    std::vector<uint8_t> choosables;
    std::vector<unsigned> randIQIndex;
    //Make choosable Types list
    for (uint8_t i = 0; i != maxIQId; i++) {
        if (std::find(std::begin(excludedIQs), std::end(excludedIQs), i) == std::end(excludedIQs))
            choosables.push_back(i);
    }
    //Map types to pokemon ID
    for (unsigned i = 0; i != 600; i++) {
        randIQIndex.push_back(rand() % choosables.size());
    }
    //Assign the values to the pokemon entries
    for (unsigned i = 0; i != 1155; i++) {
        uint8_t *entry = memory.data() + 0x00472808 + i * 68;

        //Get pokemon ID from memory
        uint16_t ID = *(entry+4)+*(entry+5)*256;

        memcpy(entry + 0x17, &choosables[randIQIndex[ID]], 1);
    }
}
