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
        e.offset   = offset;
        e.size     = size;
        e.activo   = activo;
        entries.push_back(e);
    }

    file.close();
}

void Index::save(std::string filename) {
    std::ofstream file(filename);

    for (int i = 0; i < entries.size(); i++) {
        file << entries[i].noCuenta << " "
             << entries[i].offset   << " "
             << entries[i].size     << " "
             << entries[i].activo   << "\n";
    }

    file.close();
}

bool Index::insert(IndexEntry entry) {
    if (find(entry.noCuenta) != nullptr) {
        std::cout << "Ya existe un alumno con ese numero de cuenta." << std::endl;
        return false;
    }

    int pos = 0;
    while (pos < entries.size() && entries[pos].noCuenta < entry.noCuenta) {
        pos++;
    }

    entries.insert(entries.begin() + pos, entry);
    return true;
}

IndexEntry* Index::find(std::string noCuenta) {
    int pos = binarySearch(noCuenta);

    if (pos == -1) return nullptr;
    if (entries[pos].activo == false) return nullptr;

    return &entries[pos];
}

bool Index::remove(std::string noCuenta) {
    int pos = binarySearch(noCuenta);

    if (pos == -1) {
        std::cout << "No se encontro ese numero de cuenta." << std::endl;
        return false;
    }

    entries[pos].activo = false;
    return true;
}

bool Index::update(std::string noCuenta, long newOffset, int newSize) {
    int pos = binarySearch(noCuenta);

    if (pos == -1) return false;

    entries[pos].offset = newOffset;
    entries[pos].size   = newSize;
    return true;
}

void Index::compact() {
    std::vector<IndexEntry> soloActivos;

    for (int i = 0; i < entries.size(); i++) {
        if (entries[i].activo == true) {
            soloActivos.push_back(entries[i]);
        }
    }

    entries = soloActivos;
}

int Index::binarySearch(std::string noCuenta) {
    int izq = 0;
    int der = entries.size() - 1;

    while (izq <= der) {
        int mid = (izq + der) / 2;

        if (entries[mid].noCuenta == noCuenta) {
            return mid;
        }

        if (entries[mid].noCuenta < noCuenta) {
            izq = mid + 1;
        } else {
            der = mid - 1;
        }
    }

    return -1;
}
