#ifndef ITEMSPAWN_H
#define ITEMSPAWN_H

#include <cstdint>
#include <map>
#include <random>

#define MINIMUM_ITEM_TO_CATEGORY_RATIO 3

struct Category {
    Category(double probability) : probability(probability), items(0) {}
    void addItem() {
        items++;
    }
    
    double probability;
    unsigned items;
};

struct Item {
    Item(const Category &category, double probability) : category(category), probability(probability) {}
    const Category &category;
    double probability;
};

class ItemSpawn {
public:
    ItemSpawn(std::mt19937 &rand);
    //Add groups
    void addCategory(uint16_t offset, unsigned size);
    //Add items after groups
    void addItem(uint16_t offset, uint16_t categoryOffset);
    //Remove items to fit into size limit then normalize probabilities
    void normalize(unsigned sizeLimit);
    //Write items into memory
    void write(uint16_t *memory);
    
private:
    std::mt19937 &rand;
    std::map<uint16_t, Category> categories;
    std::map<uint16_t, Item> items;
    
    unsigned testSize();
};

#endif // ITEMSPAWN_H
