#include "rom.h"

ROM::ROM(std::string filepath) {
    rom.open(filepath.c_str(), std::fstream::in | std::fstream::out | std::fstream::binary);
}

void ROM::randomizePokemon() {

}

