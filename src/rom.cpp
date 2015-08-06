#include "rom.h"

ROM::ROM(std::string filepath) {
    rom = fstream::open(filepath);
}

void ROM::randomizePokemon() {

}

