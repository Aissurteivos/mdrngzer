#ifndef ROM_H
#define ROM_H
#include <memory>
#include <cstdint>
#include <vector>
#include <random>
#include "Itemspawn.h"

class ROM {
public:
    ROM(unsigned seed);
    void open(const std::string &filePath);
    void save(const std::string &filePath);
    
    void randPokemon();
    void randAbilities(unsigned percent);
    void randTypes(unsigned percent);
    void randIQs();
    void randMusic();
    void randTerrain();
    void randMoveset();
    void randItems();
    
private:
    std::vector<uint8_t> memory;
    std::mt19937 rand;
    
    template<typename Type>
    Type& vecRand(std::vector<Type> &vec) {
        return vec[rand() % vec.size()];
    }
    
    template<typename Type>
    Type vecRandAndRemove(std::vector<Type> &vec) {
        auto loc = vec.begin() + (rand() % vec.size());
        Type val = *loc;
        vec.erase(loc);
        return val;
    }
};

#endif // ROM_H

