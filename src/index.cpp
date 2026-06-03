#include "index.h"
#include <fstream>
#include <iostream>

void Index::load(std::string filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return;

    std::string cuenta;
    long offset;
    int size;
    bool activo;

    while (file >> cuenta >> offset >> size >> activo) {
        IndexEntry e;
        e.noCuenta = cuenta;
        e.offset = offset;
        e.size = size;
        e.activo = activo;
        entries.push_back(e);
    }

    file.close();
}

void Index::save(std::string filename) {
    std::ofstream file(filename);

    for (int i = 0; i < entries.size(); i++) {
        file << entries[i].noCuenta << " "
             << entries[i].offset << " "
             << entries[i].size << " "
             << entries[i].activo << "\n";
    }

    file.close();
}

bool Index::insert(IndexEntry entry) {
    if (find(entry.noCuenta) != nullptr) {
        std::cout << "Error: ya existe un registro con ese numero de cuenta." << std::endl;
        return false;
    }

    int i = 0;
    while (i < entries.size() && entries[i].noCuenta < entry.noCuenta) {
        i++;
    }

    entries.insert(entries.begin() + i, entry);
    return true;
}

IndexEntry* Index::find(std::string noCuenta) {
    int i = binarySearch(noCuenta);
    if (i == -1) return nullptr;
    if (!entries[i].activo) return nullptr;
    return &entries[i];
}

bool Index::remove(std::string noCuenta) {
    int i = binarySearch(noCuenta);
    if (i == -1) {
        std::cout << "Registro no encontrado." << std::endl;
        return false;
    }

    entries[i].activo = false;
    return true;
}

bool Index::update(std::string noCuenta, long newOffset, int newSize) {
    int i = binarySearch(noCuenta);
    if (i == -1) return false;

    entries[i].offset = newOffset;
    entries[i].size = newSize;
    return true;
}

void Index::compact() {
    std::vector<IndexEntry> activos;

    for (int i = 0; i < entries.size(); i++) {
        if (entries[i].activo) {
            activos.push_back(entries[i]);
        }
    }

    entries = activos;
}

int Index::binarySearch(std::string noCuenta) {
    int lo = 0;
    int hi = entries.size() - 1;

    while (lo <= hi) {
        int mid = (lo + hi) / 2;

        if (entries[mid].noCuenta == noCuenta) return mid;
        if (entries[mid].noCuenta < noCuenta)  lo = mid + 1;
        else                                    hi = mid - 1;
    }

    return -1;
}
