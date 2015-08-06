#ifndef ROM_H
#define ROM_H
#include <iostream>
#include <fstream>

class ROM {
private:
    fstream rom;
public:
    ROM(std::string filepath);
    void randomizePokemon();
};

#endif // ROM_H
