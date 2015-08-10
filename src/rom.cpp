#include "rom.h"
#include <fstream>
#include <cstring>
#include <iostream>
#include <algorithm>

const uint16_t maxPokemonId = 0x217;

const uint16_t excludedPokemon[] = {
    0x0000, //No pokemon/end of list
    0x0090, //Articuno
    0x0091, //Zapdos
    0x0092, //Moltres
    0x0096, //Mewtwo
    0x0097, //Mew
    0x010E, //Raikou
    0x010F, //Entei
    0x0110, //Suicune
    0x0114, //Lugia
    0x0115, //Ho-oh
    0x0116, //Celebi
    0x0117, //Celebi(shiny)
    0x017B, //Castform(form a)
    0x017C, //Castform(form b)
    0x017D, //Castform(form c)
    0x017E, //Castform(form d)
    0x017F, //Kecleon
    0x0180, //Kecleon(shiny)
    0x0199, //Regirock
    0x019A, //Regice
    0x019B, //Registeel
    0x019C, //Latias
    0x019D, //Latios
    0x019E, //Kyogre
    0x019F, //Groudon
    0x01A0, //Rayquaza
    0x01A1, //Jirachi
    0x01A2, //Deoxys(form a)
    0x01A3, //Deoxys(form b)
    0x01A4, //Deoxys(form c)
    0x01A5, //Deoxys(form d)
    0x0207, //Dusknoir
    0x0208, //Froslass
    0x0209, //Rotom
    0x020A, //Uxie
    0x020B, //Mesprit
    0x020C, //Azelf
    0x020D, //Dialga
    0x020E, //Palkia
    0x020F, //Heatran
    0x0210, //Regigigas
    0x0211, //Girarina
    0x0212, //Cresselia
    0x0213, //Phione
    0x0214, //Manaphy
    0x0215, //Darkrai
    0x0216, //Shaymin
};

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

