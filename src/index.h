#pragma once
#include <string>
#include <vector>

struct IndexEntry {
    std::string noCuenta;
    long offset;
    int size;
    bool activo;
};

class Index {
public:
    std::vector<IndexEntry> entries;

    void load(std::string filename);
    void save(std::string filename);
    bool insert(IndexEntry entry);
    IndexEntry* find(std::string noCuenta);
    bool remove(std::string noCuenta);
    bool update(std::string noCuenta, long newOffset, int newSize);
    void compact();

private:
    int binarySearch(std::string noCuenta);
};
