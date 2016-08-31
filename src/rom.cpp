#include "rom.h"
#include <fstream>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <QDebug>
#include <set>
#include <array>
#include <process.h>
#include <direct.h>
#include <stdio.h>

ROM::ROM(unsigned seed) : rand(seed) {
}

int ROM::loadFile(const std::string &filePath) {

    std::ifstream file(filePath, std::ios::binary);

    if (!file) {
        return 1;
    }

    file.seekg(0, std::ios::end);
    memory.resize(file.tellg());
    file.seekg(0, std::ios::beg);

    file.read((char*)memory.data(), memory.size());

    file.close();

    return 0;
}

int ROM::saveFile(const std::string &filePath) {

    std::ofstream file(filePath, std::ios::binary);

    file.write((char*)memory.data(), memory.size());

    file.close();

    return 0;
}

void ROM::open(const std::string &filePath) {
    if (filePath == "") {
        throw std::string("Please select a ROM before trying to randomize.");
    }

    mkdir("rom");

    std::string romPath = "\"" + filePath + "\"";

    const char *my_args[21]{
        "ndstool.exe",
        "-x",
        romPath.c_str(),
        "-9",
        "\"rom/arm9.bin\"",
        "-7",
        "\"rom/arm7.bin\"",
        "-y9",
        "\"rom/y9.bin\"",
        "-y7",
        "\"rom/y7.bin\"",
        "-d",
        "\"rom/data\"",
        "-y",
        "\"rom/overlay\"",
        "-t",
        "\"rom/banner.bin\"",
        "-h",
        "\"rom/header.bin\"",
        nullptr };

    spawnv(P_WAIT, "ndstool.exe", my_args);

    if(loadFile("rom/header.bin")) {
        throw std::string("ROM: Failed to open header.bin");
    }

    if (std::strcmp("POKEDUN SORAC2SE01", (char*)memory.data())) {
        throw std::string("ROM: ROM must be Pokemon Mystery Dungeon - Explorers of Sky (US)");
    }
}

void ROM::save(const std::string &filePath) {

    std::string romPath = "\"" + filePath + "\"";

    const char *my_args[21]{
        "ndstool.exe",
        "-c",
        romPath.c_str(),
        "-9",
        "\"rom/arm9.bin\"",
        "-7",
        "\"rom/arm7.bin\"",
        "-y9",
        "\"rom/y9.bin\"",
        "-y7",
        "\"rom/y7.bin\"",
        "-d",
        "\"rom/data\"",
        "-y",
        "\"rom/overlay\"",
        "-t",
        "\"rom/banner.bin\"",
        "-h",
        "\"rom/header.bin\"",
        nullptr };

    spawnv(P_WAIT, "ndstool.exe", my_args);

    rmdir("rom");
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
    std::vector<uint8_t> terrains;

    for (uint8_t i = 0; i != maxTerrainId; i++)
        if (std::find(std::begin(excludedTerrains), std::end(excludedTerrains), i) == std::end(excludedTerrains))
            choosables.push_back(i);

    //generate list for dungeon
    for (unsigned i = 0; i != 250; i++)
        terrains.push_back(vecRand(choosables));

    unsigned count = 0;
    uint8_t last = 0x00;

    //Assign the values to the floor entries
    for (unsigned i = 0; i != 1837; i++) {
        uint8_t *entry = memory.data() + 0x00008DB0 + i * 32;

        uint8_t current = *(entry + 0x2);

       if (last != current) {
            count++;
       }

       last = *(entry + 0x2);
       memcpy(entry + 0x2, &terrains[count], 1);

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
    std::vector<uint8_t> music;

    for (uint8_t i = 0; i != maxMusicId; i++)
        if (std::find(std::begin(excludedMusic), std::end(excludedMusic), i) == std::end(excludedMusic))
            choosables.push_back(i);

    //generate list for dungeon
    for (unsigned i = 0; i != 200; i++)
        music.push_back(vecRand(choosables));

    unsigned count = 0;
    uint8_t last = 0x01;

    //Assign the values to the floor entries
    for (unsigned i = 0; i != 1837; i++) {
        uint8_t *entry = memory.data() + 0x00008DB0 + i * 32;

        uint8_t current = *(entry + 0x3);

        if (last != current) {
            count++;
        }

        last = *(entry + 0x3);
        memcpy(entry + 0x3, &music[count], 1);

    }
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
        0x0217, //Shaymin Sky
        0x0229  //Decoy
    };
    
    std::vector<uint16_t> choosables;
    
    for (uint16_t i = 0; i != maxPokemonId; i++)
        if (std::find(std::begin(excludedPokemon), std::end(excludedPokemon), i) == std::end(excludedPokemon))
            choosables.push_back(i);

    for (unsigned i = 0; i != 14229; i++) {
        uint8_t *entry = memory.data() + 0x000177D0 + i * 8;
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
        
        memcpy(entry + 6, &vecRand(choosables), 2);
    }
}

void ROM::randItems() {

    const uint16_t maxItemId = 0x016B;

    const uint16_t excludedItems[] = {
        0x0000,  //No item
        0x000B,  //Null item
        0x000C,  //Null item
        0x0062,  //Null item
        0x0071,  //Null item
        0x0072,  //Null item
        0x008A,  //Null item
        0x00A6,  //Null item
        0x00AF,  //Null item
        0x00B0,  //Null item
        0x00B1,  //Null item
        0x00B5,  //Null item
        0x00B8,  //Null item
        0x00B9,  //Null item
        0x00BB,  //Used TM, not a dungeon item
        0x00C2,  //Null item
        0x00C6,  //Null item
        0x00CD,  //Null item
        0x00DB,  //Null item
        0x00E0,  //Null item
        0x00E2,  //Null item
        0x00EC,  //Null item
        0x0102,  //Null item
        0x0103,  //Null item
        0x0125,  //Null item
        0x0126,  //Null item
        0x0127,  //Null item
        0x0128,  //Null item
        0x0129,  //Null item
        0x012A,  //Null item
        0x012B,  //Null item
        0x012C,  //Null item
        0x0144,  //Null item
        0x0153,  //Null item
        0x0159,  //Null item
        0x015D,  //Null item
        0x0161,  //Null item
        0x0168,  //Null item
        0x0169,  //Null item
        0x016B   //Null item
    };

    enum Group {
        THROWN_LINE,
        THROWN_ARC,
        BERRY_SEED,
        FOOD,
        HELD,
        TM,
        COINS,
        OTHER = 8,
        ORB = 9,
        BOX = 10,
        TOTAL
    };

    std::vector<uint16_t> choosables;
    std::array<std::vector<uint16_t>, TOTAL> groupChoosables;

    for (uint16_t i = 0; i != maxItemId; i++)
        if (std::find(std::begin(excludedItems), std::end(excludedItems), i) == std::end(excludedItems)) {
            choosables.push_back(i);

            //Separate items into their respective groups

            if ((i >= 0x0001 && i <= 0x0006) || i == 0x0009 )
                groupChoosables[THROWN_LINE].push_back(i);

            if ((i >= 0x0007 && i <= 0x0008) || i == 0x000A )
                groupChoosables[THROWN_ARC].push_back(i);

            if (i >= 0x000D && i <= 0x0044)
                groupChoosables[HELD].push_back(i);

            if ((i >= 0x0045 && i <= 0x006C) || (i >= 0x0074 && i <= 0x0076))
                groupChoosables[BERRY_SEED].push_back(i);

            if ((i >= 0x006D && i <= 0x0070) || (i >= 0x0077 && i <= 0x0089) || i == 0x0073 )
                groupChoosables[FOOD].push_back(i);

            if (i >= 0x008B && i <= 0x00BA && i != 0x00B7)
                groupChoosables[OTHER].push_back(i);

            if (i >= 0x00BC && i <= 0x0124)
                groupChoosables[TM].push_back(i);

            if (i >= 0x012D && i <= 0x0167)
                groupChoosables[ORB].push_back(i);

            if (i == 0x016A)
                groupChoosables[BOX].push_back(i);

            if (i == 0x00B7)
                groupChoosables[COINS].push_back(i);
        }

    const int weights[TOTAL] { 7, 3, 142, 224, 56, 96, 1, 0, 41, 54, 1 }; //total 324


    uint8_t *ptrList = memory.data() + 0x00041A04; // list of pointers to every list
    uint8_t *valueStart = memory.data() + 0x00035A28; // start of item lists

    unsigned position = 0;

    for (unsigned i = 0; i < 216; i++) {
        //find size by subtracting next pointer from the current one

        uint32_t size = *(uint32_t*)(ptrList+(i*4)+4);
        size -= *(uint32_t*)(ptrList+(i*4));

        ItemSpawn list(rand);

        for (unsigned j = 0; j < TOTAL; j++) {
            if (weights[j] != 0) {
                list.addCategory(j, weights[j]);

                for (unsigned k = 0; k < groupChoosables[j].size();k++) {
                    list.addItem((groupChoosables[j][k]) + 0x10, j);
                }
            }
        }

        list.normalize(size);
        list.write((uint16_t*)(valueStart + position));
        position += size;
    }
}



void ROM::randTypes(unsigned percent) {
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
        t.first = vecRand(choosables);

        //Try for second Type
        if ((rand() % 100) < percent) {
            do {
                t.second = vecRand(choosables);
            } while (t.second == t.first);
        }
        else
            t.second = 0;
    }

    //Assign the values to the pokemon entries
    for (unsigned i = 0; i != 1155; i++) {
        uint8_t *entry = memory.data() + 0x8 + i * 68;

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
        pokemonIQs.push_back(vecRand(choosables));

    //Assign the values to the pokemon entries
    for (unsigned i = 0; i != 1155; i++) {
        uint8_t *entry = memory.data() + 0x8 + i * 68;

        //Get pokemon ID from memory
        uint16_t ID = *(entry+4)+*(entry+5)*256;

        memcpy(entry + 0x17, &pokemonIQs[ID], 1);
    }
}

void ROM::randAbilities(unsigned percent) {
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
        a.first = vecRand(choosables);
        
        //Try for second ability
        if ((rand() % 100) < percent)
            do {
                a.second = vecRand(choosables);
            } while ( a.second == a.first );
        else
            a.second = 0;
    }

    //Iterate through all pokemon entires and assign their abilities based on pokemon ID
    for (unsigned i = 0; i != 1155; i++) {
        uint8_t *entry = memory.data() + 0x8 + i * 68;

        //Get pokemon ID from memory
        uint16_t ID = *(entry+4)+*(entry+5)*256;

        memcpy(entry + 0x18, &pokemonAbilities[ID].first, 1);
        memcpy(entry + 0x19, &pokemonAbilities[ID].second, 1);
    }
}



void ROM::randMoveset() {

    const uint16_t maxMoveId = 0x21E;

    const uint16_t excludedMoves[] = {
        0x0000,  //Null move
        0x0160,  //Struggle
        0x0163,  //Regular attack, not a move
        0x0164,  //Debug move "is watching"
        0x0165,  //Debug bide
        0x0166,  //Debug revenge
        0x0167,  //Debug avalanche
        0x0169,  //Null move
        0x016A,  //Null move
        0x016B,  //Item effect move, "See-Trap"
        0x016C,  //Item effect move, "Takeaway"
        0x016D,  //Item effect move, "Rebound"
        0x016E,  //Unused move, "Bloop Slash"
        0x016F,  //Item effect move, "Switcher"
        0x0170,  //Item effect move, "Blowback"
        0x0171,  //Item effect move, "Warp"
        0x0172,  //Item effect move, "Transfer"
        0x0173,  //Item effect move, "Slow Down"
        0x0174,  //Item effect move, "Speed Boost"
        0x0175,  //Item effect move, "Searchlight"
        0x0176,  //Item effect move, "Petrify"
        0x0177,  //Item effect move, "Stay Away"
        0x0178,  //Item effect move, "Pounce"
        0x0179,  //Item effect move, "Trawl"
        0x017A,  //Item effect move, "Cleanse"
        0x017B,  //Item effect move, "Observer"
        0x017C,  //Item effect move, "Decoy Maker"
        0x017D,  //Item effect move, "Siesta"
        0x017E,  //Item effect move, "Totter"
        0x017F,  //Item effect move, "Two-Edge"
        0x0180,  //Item effect move, "No-Move"
        0x0181,  //Item effect move, "Escape"
        0x0182,  //Item effect move, "Scan"
        0x0183,  //Item effect move, "Power-Ears"
        0x0184,  //Item effect move, "Drought"
        0x0185,  //Item effect move, "Trap Buster"
        0x0186,  //Item effect move, "Wild Call"
        0x0187,  //Item effect move, "Invisify"
        0x0188,  //Item effect move, "One-Shot"
        0x0189,  //Item effect move, "HP Gauge"
        0x018B,  //Item effect move, "Reviver"
        0x018C,  //Item effect move, "Shocker"
        0x018D,  //Item effect move, "Echo"
        0x018E,  //Item effect move, "Famish"
        0x018F,  //Item effect move, "One-Room"
        0x0190,  //Item effect move, "Fill-in"
        0x0191,  //Item effect move, "Trapper"
        0x0192,  //Item effect move, "Possess"
        0x0193,  //Item effect move, "Itemize"
        0x0194,  //Null move
        0x0195,  //Item effect move, "projectile"
        0x0196,  //Item effect move, "Hurl"
        0x0197,  //Item effect move, "Mobile"
        0x0198,  //Item effect move, "Item-Toss"
        0x0199,  //Item effect move, "See Stairs"
        0x019A,  //Item effect move, "Long Toss"
        0x019B,  //Null move
        0x019C,  //Item effect move, "Pierce"
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
        0x01D3   //Null move
    };
    
    class LevelMove {
    public:
        LevelMove(uint16_t move, uint8_t level) : move(move), level(level) {
            if (level < 6)
                LevelMove::level = 1;
        }
        
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

    uint8_t *entry = memory.data() + 0x10;

    for (unsigned i = 0, position = 0; i != 553; i++) {
        uint8_t *levelList = entry + position;
        unsigned levelSpace = 0;

        while (*(entry + position + levelSpace) != 0 || *(entry + position + levelSpace-1) > 0x7F)
            levelSpace++;

        int j = 0;
        unsigned level;
        int spaceRemain = levelSpace;
        
        std::vector<uint16_t> levelChoosables(choosables);

        for (j = 0, level = 1; j < spaceRemain - 4; ) {
            LevelMove lmove(vecRandAndRemove(levelChoosables), level);

            if (lmove.isLarge()) {
                lmove.write(levelList + j);
                j += 3;
            } else {
                lmove.write(levelList + j);
                j += 2;
            }

            level += 1 + rand() % 5;
            //Don't allow level above 50
            if (level > 50)
                level = 50;
        }

        //Switch on how much space is left
        switch (levelSpace - j) {
        case 4: {
            uint16_t move;
            while (std::find(levelChoosables.begin(), levelChoosables.end(), move = vecRand(size2Choosables))
                   == levelChoosables.end());
            //Since we will get one more move after this, remove it from choosables
            levelChoosables.erase(std::find(levelChoosables.begin(), levelChoosables.end(), move));
            LevelMove lmove(move, level);
            lmove.write(levelList + j);
            j += 2;
            level += rand() % 5;
            if (level > 50)
                level = 50;
        } {
            uint16_t move;
            while (std::find(levelChoosables.begin(), levelChoosables.end(), move = vecRand(size2Choosables))
                   == levelChoosables.end());
            LevelMove lmove(move, level);
            lmove.write(levelList + j);
            j += 2;
        } break;
        case 3: {
            uint16_t move;
            while (std::find(levelChoosables.begin(), levelChoosables.end(), move = vecRand(size3Choosables))
                   == levelChoosables.end());
            LevelMove lmove(move, level);
            lmove.write(levelList + j);
            j += 3;
        } break;
        case 2: {
            uint16_t move;
            while (std::find(levelChoosables.begin(), levelChoosables.end(), move = vecRand(size2Choosables))
                   == levelChoosables.end());
            LevelMove lmove(move, level);
            lmove.write(levelList + j);
            j += 2;
        } break;
        default:
            break;
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



void ROM::randText() {

    std::vector<std::pair<std::string,unsigned>> choosables;
    std::vector<unsigned> slotLength;

    uint8_t *ptrList = memory.data();
    uint8_t *stringList = memory.data() + 0x12050;

    //read all the strings from memory, put them into a vector, along with their length

    for (unsigned i = 0;i < 18452; i++) {
        std::pair<std::string,unsigned> message;
        message.first = ((char*)((ptrList) + *(uint32_t*)(ptrList+(i*4))));

        //change anything that makes the font 2, to make it font 0, this is to prevent glitching the text really bad
        auto it = message.first.find("[FT:2]");
        if (it!=std::string::npos)
            message.first.replace(it, 6, "[FT:0]");

        message.first.push_back(0x00);
        message.second = strlen(message.first.c_str()) + 1;
        slotLength.push_back(message.second);
        choosables.push_back(message);
    }


    unsigned position = 0;

    //replace string with string of equal size
    for (unsigned i = 0;i < 18452; i++) {

        unsigned slotSize = slotLength[i];
        std::vector<std::string> subChoosables;

        //create list of strings that meet the required length

        for (unsigned k = 0; k != choosables.size(); k++) {
            if (choosables[k].second == slotSize) {
                subChoosables.push_back(choosables[k].first);
            }
        }

        std::string message = vecRand(subChoosables);

        //remove the string from choosables

        std::pair<std::string,unsigned> deleteMe;
        deleteMe.first = message;
        deleteMe.second = slotSize;

        auto it = std::find(std::begin(choosables), std::end(choosables), deleteMe);

        choosables.erase(it);

        //convert string to a char* vector

        std::vector<char> data;

        if (slotSize > 1) {
            std::copy(message.begin(), message.end(), std::back_inserter(data));
        } else if (slotSize == 1) {
            data.push_back(0x00);
        }

        //write to memory

        memcpy(stringList + position, &data[0], slotSize);
        position += slotSize;
    }
}
