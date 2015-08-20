#include "rom.h"
#include <fstream>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <QDebug>

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
    const uint16_t maxPokemonId = 0x217;
    
    const uint16_t excludedPokemon[] = {
        0x0000, //No pokemon/end of list
        0x0090, //Articuno
        0x0091, //Zapdos
        0x0092, //Moltres
        0x0096, //Mewtwo
        0x0097, //Mew
        0x00C9, //Unown A
        0x00CA, //Unown B
        0x00CB, //Unown C
        0x00CC, //Unown D
        0x00CD, //Unown E
        0x00CE, //Unown F
        0x00CF, //Unown G
        0x00D0, //Unown H
        0x00D1, //Unown I
        0x00D2, //Unown J
        0x00D3, //Unown K
        0x00D4, //Unown L
        0x00D5, //Unown M
        0x00D6, //Unown N
        0x00D7, //Unown O
        0x00D8, //Unown P
        0x00D9, //Unown Q
        0x00DA, //Unown R
        0x00DB, //Unown S
        0x00DC, //Unown T
        0x00DD, //Unown U
        0x00DE, //Unown V
        0x00DF, //Unown W
        0x00E0, //Unown X
        0x00E1, //Unown Y
        0x00E2, //Unown Z
        0x00E3, //Unown !
        0x00E4, //Unown ?
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
        0x0229  //Decoy
    };
    
    std::vector<uint16_t> choosables;
    
    for (uint16_t i = 0; i != maxPokemonId; i++)
        if (std::find(std::begin(excludedPokemon), std::end(excludedPokemon), i) == std::end(excludedPokemon))
            choosables.push_back(i);
    
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
    const uint8_t maxAbilityId = 0x7C;
    
    const uint8_t excludedAbilities[] = {
        0x00, //No ability
        0x35, //Wonder Guard, possibly gamebreaking
        0x74  //Unknown ability, named "$$$"
    };
    
    struct PokemonAbility {
        uint8_t first, second;
    };
    
    std::vector<uint8_t> choosables;
    std::vector<PokemonAbility> pokemonAbilities;
    
    //Make choosable abilitities list
    for (uint8_t i = 0; i != maxAbilityId; i++) {
        if (std::find(std::begin(excludedAbilities), std::end(excludedAbilities), i) == std::end(excludedAbilities))
            choosables.push_back(i);
    }
    
    //Map abilities to every pokemon ID
    for (unsigned i = 0; i != 600; i++) {
        pokemonAbilities.emplace_back();
        PokemonAbility &a = pokemonAbilities.back();
        a.first = choosables[rand() % choosables.size()];
        
        //50% chance for second ability
        if ((rand() % 100) < 50)
            a.second = choosables[rand() % choosables.size()];
        else
            a.second = 0;
    }

    //Iterate through all pokemon entires and assign their abilities based on pokemon ID
    for (unsigned i = 0; i != 1155; i++) {
        uint8_t *entry = memory.data() + 0x00472808 + i * 68;

        //Get pokemon ID from memory
        uint16_t ID = *(entry+4)+*(entry+5)*256;

        memcpy(entry + 0x18, &pokemonAbilities[ID].first, 1);
        memcpy(entry + 0x19, &pokemonAbilities[ID].second, 1);
    }
}

void ROM::randTypes() {
    const uint8_t maxTypeId = 0x12;
    
    const uint8_t excludedTypes[] = {
        0x00, //No type, so excluded
        0x12  //Neutral type, not used for pokemon
    };
    
    struct PokemonType {
        uint8_t first, second;
    };
    
    std::vector<uint8_t> choosables;
    std::vector<PokemonType> pokemonTypes;
    
    //Make choosable Types list
    for (uint8_t i = 0; i != maxTypeId; i++)
        if (std::find(std::begin(excludedTypes), std::end(excludedTypes), i) == std::end(excludedTypes))
            choosables.push_back(i);
    
    //Map types to every pokemon ID
    for (unsigned i = 0; i != 600; i++) {
        pokemonTypes.emplace_back();
        PokemonType &t = pokemonTypes.back();
        t.first = rand() % choosables.size();
        
        //40% chance for second Type
        if ((rand() % 100) < 40)
            t.second = rand() % choosables.size();
        else
            t.second = 0;
    }

    //Assign the values to the pokemon entries
    for (unsigned i = 0; i != 1155; i++) {
        uint8_t *entry = memory.data() + 0x00472808 + i * 68;

        //Get pokemon ID from memory
        uint16_t ID = *(entry+4)+*(entry+5)*256;

        memcpy(entry + 0x14, &pokemonTypes[ID].first, 1);
        memcpy(entry + 0x15, &pokemonTypes[ID].second, 1);
    }
}

void ROM::randIQs() {
    const uint8_t maxIQId = 0xF;
    
    const uint8_t excludedIQs[] = {
        0x08, //Unused
        0x09, //Unused
        0x0C, //Unused
        0x0D, //Unused
        0x0E, //Unused
        0x0F  //Invalid
    };
    
    std::vector<uint8_t> choosables;
    std::vector<uint8_t> pokemonIQs;
    
    //Make choosable IQ's list
    for (uint8_t i = 0; i != maxIQId; i++)
        if (std::find(std::begin(excludedIQs), std::end(excludedIQs), i) == std::end(excludedIQs))
            choosables.push_back(i);
    
    //Map IQ's to every pokemon ID
    for (unsigned i = 0; i != 600; i++)
        pokemonIQs.push_back(choosables[rand() % choosables.size()]);
    
    //Assign the values to the pokemon entries
    for (unsigned i = 0; i != 1155; i++) {
        uint8_t *entry = memory.data() + 0x00472808 + i * 68;

        //Get pokemon ID from memory
        uint16_t ID = *(entry+4)+*(entry+5)*256;

        memcpy(entry + 0x17, &pokemonIQs[ID], 1);
    }
}

void ROM::randMusic() {
    const uint8_t maxMusicId = 0x80;

    const uint8_t excludedMusic[] = {
        0x00, //No music
        0x36, //No music
        0x76, //No music
        0x77, //No music
        0x78, //No music
        0x79, //No music
        0x7A, //No music
        0x7B, //No music
        0x7C, //No music
        0x7D, //No music
        0x7E, //No music
        0x7F  //No music
    };

    std::vector<uint8_t> choosables;
    std::vector<uint8_t> Music;

    for (uint8_t i = 0; i != maxMusicId; i++)
        if (std::find(std::begin(excludedMusic), std::end(excludedMusic), i) == std::end(excludedMusic))
            choosables.push_back(i);

    //generate list for dungeon
    for (unsigned i = 0; i != 200; i++)
        Music.push_back(choosables[rand() % choosables.size()]);

    unsigned count = 0;
    uint8_t last = 0x01;

    //Assign the values to the floor entries
    for (unsigned i = 0; i != 1837; i++) {
        uint8_t *entry = memory.data() + 0x003DC7B0 + i * 32;

        uint8_t current = *(entry + 0x3);

        if (last != current) {
            count++;
        }

        last = *(entry + 0x3);
        memcpy(entry + 0x3, &Music[count], 1);

    }
}

void ROM::randTerrain() {
    const uint8_t maxTerrainId = 0x90;

    const uint8_t excludedTerrains[] = {
        0x0D, //Invalid
        0x15, //Invalid
        0x29, //Invalid
        0x2F, //Invalid
        0x3C, //Invalid
        0x46, //Invalid
        0x47, //Invalid
        0x48, //Invalid
        0x49, //Invalid
        0x4D, //Invalid
        0x56, //Invalid
        0x59, //Invalid
        0x5C, //Invalid
        0x5D, //Invalid
        0x5F, //Invalid
        0x59, //Invalid
        0x62, //Invalid
        0x64, //Invalid
        0x6B, //Invalid
        0x78, //Invalid
        0x79, //Invalid
        0x90, //Invalid
    };

    std::vector<uint8_t> choosables;
    std::vector<uint8_t> Terrains;

    for (uint8_t i = 0; i != maxTerrainId; i++)
        if (std::find(std::begin(excludedTerrains), std::end(excludedTerrains), i) == std::end(excludedTerrains))
            choosables.push_back(i);

    //generate list for dungeon
    for (unsigned i = 0; i != 250; i++)
        Terrains.push_back(choosables[rand() % choosables.size()]);

    unsigned count = 0;
    uint8_t last = 0x00;

    //Assign the values to the floor entries
    for (unsigned i = 0; i != 1837; i++) {
        uint8_t *entry = memory.data() + 0x003DC7B0 + i * 32;

        uint8_t current = *(entry + 0x2);

       if (last != current) {
            count++;
       }

       last = *(entry + 0x2);
       memcpy(entry + 0x2, &Terrains[count], 1);

    }
}

void ROM::randMoveset() {

    const uint16_t maxMoveId = 0x21E;

    const uint16_t excludedMoves[] = {
        0x0000,  //Null move
        0x0169,  //Null move
        0x016A,  //Null move
        0x016B,  //Null move
        0x016C,  //Null move
        0x016D,  //Null move
        0x01E6,  //Null move
        0x016F,  //Null move
        0x0170,  //Null move
        0x0171,  //Null move
        0x0172,  //Null move
        0x0173,  //Null move
        0x0174,  //Null move
        0x0175,  //Null move
        0x0176,  //Null move
        0x0177,  //Null move
        0x0178,  //Null move
        0x0179,  //Null move
        0x017A,  //Null move
        0x017B,  //Null move
        0x017C,  //Null move
        0x017D,  //Null move
        0x017E,  //Null move
        0x01F7,  //Null move
        0x0180,  //Null move
        0x0181,  //Null move
        0x0182,  //Null move
        0x0183,  //Null move
        0x0184,  //Null move
        0x0185,  //Null move
        0x0186,  //Null move
        0x0187,  //Null move
        0x0188,  //Null move
        0x0189,  //Null move
        0x018A,  //Null move
        0x018B,  //Null move
        0x018C,  //Null move
        0x018D,  //Null move
        0x018E,  //Null move
        0x018F,  //Null move
        0x0190,  //Null move
        0x0191,  //Null move
        0x0192,  //Null move
        0x0193,  //Null move
        0x0194,  //Null move
        0x0195,  //Null move
        0x0196,  //Null move
        0x0197,  //Null move
        0x0198,  //Null move
        0x0199,  //Null move
        0x019A,  //Null move
        0x019B,  //Null move
        0x019C,  //Null move
        0x019D,  //Null move
        0x019E,  //Null move
        0x019F,  //Null move
        0x01A0,  //Null move
        0x01A1,  //Null move
        0x01A2,  //Null move
        0x01A3,  //Null move
        0x01A4,  //Null move
        0x01A5,  //Null move
        0x01A6,  //Null move
        0x01A7,  //Null move
        0x01A8,  //Null move
        0x01A9,  //Null move
        0x01AA,  //Null move
        0x01AB,  //Null move
        0x01AC,  //Null move
        0x01AD,  //Null move
        0x01D3,  //Null move
        0x01D4   //Null move
    };
    
    class LevelMove {
    public:
        LevelMove(uint16_t move, uint8_t level) : move(move), level(level) {}
        
        LevelMove(uint8_t *data) {
            //Check if this is a terminator
            if (data[0] == 0) {
                move = 0;
                level = 0;
                return;
            }
            
            //If this is extended
            if (data[0] & 0x80) {
                move = ((data[0] & 0x7F) << 7) | (data[1] & 0x7F);
                level = data[2];
            } else {
                move = data[0] & 0x7F;
                level = data[1];
            }
        }
        
        bool isTerminating() const {
            return !move;
        }
        
        bool isLarge() const {
            return move >= 128;
        }
        
        std::size_t getSize() const {
            if (isLarge())
                return 3;
            else
                return 2;
        }

        void write(uint8_t *location) const {
            if (isLarge()) {
                location[0] = ((move >> 7) & 0x7F) | 0x80;
                location[1] = move & 0x7F;
                location[2] = level;
            } else {
                location[0] = move & 0x7F;
                location[1] = level;
            }
        }
        
    private:
        uint16_t move;
        uint8_t level;
    };

    std::vector<uint16_t> choosables;
    std::vector<uint16_t> size2Choosables;
    std::vector<uint16_t> size3Choosables;

    for (uint16_t i = 0; i != maxMoveId; i++)
        if (std::find(std::begin(excludedMoves), std::end(excludedMoves), i) == std::end(excludedMoves)) {
            choosables.push_back(i);
            LevelMove m(i, 0);
            if (m.isLarge())
                size3Choosables.push_back(i);
            else
                size2Choosables.push_back(i);
        }

    uint8_t *entry = memory.data() + 0x00487410;

    for (unsigned i = 0, position = 0; i != 563; i++) {
        uint8_t *levelList = entry + position;
        unsigned levelSpace = strlen((char*)levelList);
        
        unsigned j;
        unsigned level;
        for (j = 0, level = 0; j < levelSpace - 4; ) {
            level += rand() % 5;
            //Don't allow level above 100
            if (level > 100)
                level = 100;
            
            LevelMove lmove(choosables[rand() % choosables.size()], level);
            if (lmove.isLarge()) {
                lmove.write(levelList + j);
                j += 3;
            } else {
                lmove.write(levelList + j);
                j += 2;
            }
        }
        
        //Switch on how much space is left
        switch (levelSpace - j) {
        case 4: {
            level += rand() % 5;
            if (level > 100)
                level = 100;
            LevelMove lmove(size2Choosables[rand() % size2Choosables.size()], level);
            lmove.write(levelList + j);
            j += 2;
        } {
            level += rand() % 5;
            if (level > 100)
                level = 100;
            LevelMove lmove(size2Choosables[rand() % size2Choosables.size()], level);
            lmove.write(levelList + j);
            j += 2;
        } break;
        case 3: {
            level += rand() % 5;
            if (level > 100)
                level = 100;
            LevelMove lmove(size3Choosables[rand() % size3Choosables.size()], level);
            lmove.write(levelList + j);
            j += 2;
        } break;
        case 2: {
            level += rand() % 5;
            if (level > 100)
                level = 100;
            LevelMove lmove(size2Choosables[rand() % size2Choosables.size()], level);
            lmove.write(levelList + j);
            j += 2;
        } break;
        }
        
        

        uint8_t *TMList = entry + position + levelSpace + 1;
        unsigned TMSpace = strlen((char*)TMList);
        //TODO: Randomize TM moves
        
        uint8_t *eggList = entry + position + levelSpace + TMSpace + 2;
        unsigned eggSpace = strlen((char*)eggList);
        //TODO: Randomize egg moves
        
        position += levelSpace + TMSpace + eggSpace + 3;
    }
}
