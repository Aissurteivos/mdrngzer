#include "itemspawn.h"
#include <random>
#include <qDebug>

ItemSpawn::ItemSpawn(std::mt19937 &rand) : rand(rand) {}

void ItemSpawn::addCategory(uint16_t offset, unsigned size) {
    categories.emplace(offset, double(size));
}

void ItemSpawn::addItem(uint16_t offset, uint16_t categoryOffset) {
    std::normal_distribution<> normRand(4, 2);
    double result;
    while ((result = normRand(rand)) < 0);
    Category &c = categories.at(categoryOffset);
    items.emplace(offset, std::move(Item(c, result)));
    c.items++;
}

void ItemSpawn::normalize(unsigned sizeLimit) {
    while (testSize() > sizeLimit) {

        //Decrease the size

        //Remove a category if the amount of categories are greater than a third of the amount of items
        if (categories.size() * 3 > items.size()) {
            unsigned selection = rand() % categories.size();
            auto i = std::next(categories.begin(), selection);
            Category &category = i->second;
            //Remove items with this category
            for (auto j = items.begin(); j != items.end(); ) {
                Item &item = j->second;
                if (&category == &item.category) {
                    j = items.erase(j);
                } else
                    j++;
            }
            categories.erase(i);
        } else {
            unsigned selection = rand() % items.size();
            auto i = std::next(items.begin(), selection);
            for (auto j = categories.begin(); j != categories.end(); j++) {
                //Category of item matches category
                if (&j->second == &i->second.category) {
                    //Derease items in category
                    j->second.items--;
                    //Lower probability of category based on how many items are in it
                    j->second.probability *= (j->second.items) / (j->second.items + 1);
                    //Erase category when no items are left
                    if (j->second.items == 0)
                        categories.erase(j);
                    break;
                }
            }
            items.erase(i);
        }
    }
    
    double catagoryTotalProbability = 0.0;
    for (auto i = categories.begin(); i != categories.end(); i++)
        catagoryTotalProbability += i->second.probability;
    
    //Normalize probabilities in categories
    for (auto i = categories.begin(); i != categories.end(); i++) {
        i->second.probability /= catagoryTotalProbability;
        if (i != categories.begin())
            i->second.probability += std::prev(i)->second.probability;
    }
    
    //Normalize all category item probabilities
    for (auto i = categories.begin(); i != categories.end(); i++) {
        double itemTotalProbability = 0.0;
        
        for (auto j = items.begin(); j != items.end(); j++)
            if (&j->second.category == &i->second)
                itemTotalProbability += j->second.probability;
        
        //The first time we find an item we dont want to add the previous probability
        bool first = true;
        auto last = items.end();
        
        for (auto j = items.begin(); j != items.end(); j++)
            if (&j->second.category == &i->second) {
                j->second.probability /= itemTotalProbability;
                
                if (first)
                    first = false;
                else
                    j->second.probability += last->second.probability;
                last = j;
            }
    }
}

unsigned ItemSpawn::testSize() {
    uint16_t offset = 0;
    unsigned size = 0;
    for (auto i = categories.begin(); i != categories.end(); i++) {
        if (offset == i->first)
            size += 2;
        else
            size += 4;
        offset = i->first + 1;
    }
    
    for (auto i = items.begin(); i != items.end(); i++) {
        if (offset == i->first)
            size += 2;
        else
            size += 4;
        offset = i->first + 1;
    }
    //Add terminator
    return size + 2;
}

void ItemSpawn::write(uint16_t *memory) {
    uint16_t offset = 0;
    for (auto i = categories.begin(); i != categories.end(); i++) {
        if (offset != i->first)
            *(memory++) = i->first - offset + 30000;
        *(memory++) = i->second.probability * 10000;
        offset = i->first + 1;
    }
    
    for (auto i = items.begin(); i != items.end(); i++) {
        if (offset != i->first)
            *(memory++) = i->first - offset + 30000;
        *(memory++) = i->second.probability * 10000;
        offset = i->first + 1;
    }
    //Add terminator
    *memory = 0x76FF;
}

