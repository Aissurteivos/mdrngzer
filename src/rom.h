#ifndef ROM_H
#define ROM_H
#include <memory>
#include <cstdint>
#include <vector>
#include <random>

class ROM {
public:
    ROM(unsigned seed);
    void open(const std::string &filePath);
    void save(const std::string &filePath);
    
    void randPokemon();
    void randAbilities();
    void randTypes();
    void randIQs();
    
private:
    std::vector<uint8_t> memory;
    std::mt19937 rand;
};

#endif // ROM_H

