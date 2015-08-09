#ifndef ROM_H
#define ROM_H
#include <iostream>
#include <fstream>
#include <QString>

class ROM {
private:
    std::fstream rom;
public:
    ROM(std::string filepath);
    void randomizePokemon();
};

#endif // ROM_H
