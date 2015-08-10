#ifndef ROM_H
#define ROM_H
#include <memory>
#include <cstdint>
#include <vector>

class ROM {
public:
    ROM(const std::string &filePath);
    void save(const std::string &filePath);
    
private:
    std::vector<uint8_t> memory;
};

#endif // ROM_H
